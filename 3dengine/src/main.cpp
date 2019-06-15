//TODO logging:
/*
in order to know what the f is happening on execution time, use a log function
put it all over the place, not just errors
is easy enough to make a macro that will not log at runtime (make a verbose flag, maybe)

refactor some objects with pObject when pointer

make an "conditional assert" option that only breaks on debug but let the game run?

refactor audio code to be more my style

understand better the workings and see whats is and is not needed - attention to the threads
*/

#include "Engine.h"
#include "stb_image/stb_image.h"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "3D ENGINE"

int width, height, nrChannels;
byte *texture = stbi_load("data/duck_shades.png", &width, &height, &nrChannels, STBI_rgb_alpha);

const size_t bufferSize = 192644;

void callback(int state)
{
    printf("%d\n", state);
}

//TODO MUSIC STREAMING
#include "FileSystem.h"
#include "Memory.h"
#include "audio/stb_vorbis.c"
#include "audio/miniaudio.h"

enum class EAudioFormat {
    OGG
};

enum class EAudioMode {
    STATIC, STREAM
};

#define AS_OUTPUT_FORMAT       ma_format_f32
#define AS_OUTPUT_CHANNELS     2
#define AS_OUTPUT_SAMPLE_RATE  44100

internal ma_context gAudioContext;
internal ma_device gAudioDevice;
/*
typedef struct SAudioBuffer {
    ma_pcm_converter rawPcm;
    byte* buffer;
    SAudioBuffer* next;
    SAudioBuffer* prev;
    EAudioMode playMode;
    uint32 bufferSizeInFrames;
    uint32 frameCursor;
    bool isLooping;
    bool isPaused;
    bool isPlaying;
    bool isProcessed[2];
    float pitch;
    float volume;
} AudioBuffer;
*/
typedef struct rAudioBuffer {
    ma_pcm_converter rawPcm;                    // Required for format conversion
    float volume;
    float pitch;
    bool isPlaying;
    bool isPaused;
    bool isLooping;                   // Always true for AudioStreams
    EAudioMode playMode;                      // AudioBufferUsage type
    bool isProcessed[2];
    unsigned int frameCursor;
    unsigned int bufferSizeInFrames;
    rAudioBuffer *next;
    rAudioBuffer *prev;
    unsigned char buffer[1];
} AudioBuffer;

typedef struct SSoundWave {
    uint32 sampleCount;     // Number of samples
    uint32 sampleRate;      // Frequency
    uint32 sampleSize;      // Bits per sample: 8, 16, 32 (currently locked 16bit)
    uint32 channels;        // Number of channels - 1: mono, 2: stereo
    void *data;             // Data pointer
} SoundWave;

typedef struct SSound {
    int format;             // Audio format
    uint32 source;    // Audio source id
    uint32 buffer;    // Audio buffer id
    AudioBuffer* audioBuffer;      // Audio buffer
} Sound;

AudioBuffer* firstAudioBuffer;

void StopAudioBuffer(AudioBuffer *audioBuffer)
{
    if (audioBuffer == NULL)
    {
        //TODO log
    }

    // Don't do anything if the audio buffer is already stopped.
    if (!audioBuffer->isPlaying && !audioBuffer->isPaused) return;

    audioBuffer->isPlaying = false;
    audioBuffer->isPaused = false;
    audioBuffer->frameCursor = 0;
    audioBuffer->isProcessed[0] = true;
    audioBuffer->isProcessed[1] = true;
}

static void MixAudioFrames(float *framesOut, const float *framesIn, ma_uint32 frameCount, float localVolume)
{
    for (ma_uint32 iFrame = 0; iFrame < frameCount; ++iFrame)
    {
        for (ma_uint32 iChannel = 0; iChannel < gAudioDevice.playback.channels; ++iChannel)
        {
            float *frameOut = framesOut + (iFrame*gAudioDevice.playback.channels);
            const float *frameIn = framesIn + (iFrame*gAudioDevice.playback.channels);

            frameOut[iChannel] += (frameIn[iChannel] * 1 * localVolume);
        }
    }
}

static void OnSendAudioToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput, ma_uint32 frameCount)
{
    // This is where all of the mixing takes place.
    (void)pDevice;

    // Mixing is basically just an accumulation. We need to initialize the output buffer to 0.
    memset(pFramesOut, 0, frameCount*pDevice->playback.channels*ma_get_bytes_per_sample(pDevice->playback.format));

    // Using a mutex here for thread-safety which makes things not real-time. This is unlikely to be necessary for this project, but may
    // want to consider how you might want to avoid this.
    {
        for (AudioBuffer *audioBuffer = firstAudioBuffer; audioBuffer != NULL; audioBuffer = audioBuffer->next)
        {
            // Ignore stopped or paused sounds.
            if (!audioBuffer->isPlaying || audioBuffer->isPaused) continue;

            ma_uint32 framesRead = 0;
            for (;;)
            {
                if (framesRead > frameCount)
                {
                    break;
                }

                if (framesRead == frameCount) break;

                // Just read as much data as we can from the stream.
                ma_uint32 framesToRead = (frameCount - framesRead);
                while (framesToRead > 0)
                {
                    float tempBuffer[1024]; // 512 frames for stereo.

                    ma_uint32 framesToReadRightNow = framesToRead;
                    if (framesToReadRightNow > sizeof(tempBuffer) / sizeof(tempBuffer[0]) / AS_OUTPUT_CHANNELS)
                    {
                        framesToReadRightNow = sizeof(tempBuffer) / sizeof(tempBuffer[0]) / AS_OUTPUT_CHANNELS;
                    }

                    ma_uint32 framesJustRead = (ma_uint32)ma_pcm_converter_read(&audioBuffer->rawPcm, tempBuffer, framesToReadRightNow);
                    if (framesJustRead > 0)
                    {
                        float *framesOut = (float *)pFramesOut + (framesRead*gAudioDevice.playback.channels);
                        float *framesIn = tempBuffer;
                        MixAudioFrames(framesOut, framesIn, framesJustRead, audioBuffer->volume);

                        framesToRead -= framesJustRead;
                        framesRead += framesJustRead;
                    }

                    // If we weren't able to read all the frames we requested, break.
                    if (framesJustRead < framesToReadRightNow)
                    {
                        if (!audioBuffer->isLooping)
                        {
                            StopAudioBuffer(audioBuffer);
                            break;
                        }
                        else
                        {
                            // Should never get here, but just for safety,
                            // move the cursor position back to the start and continue the loop.
                            audioBuffer->frameCursor = 0;
                            continue;
                        }
                    }
                }

                // If for some reason we weren't able to read every frame we'll need to break from the loop.
                // Not doing this could theoretically put us into an infinite loop.
                if (framesToRead > 0) break;
            }
        }
    }
}

static ma_uint32 OnAudioBufferRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount, void *pUserData)
{
    AudioBuffer *audioBuffer = (AudioBuffer *)pUserData;

    ma_uint32 subBufferSizeInFrames = (audioBuffer->bufferSizeInFrames > 1) ? audioBuffer->bufferSizeInFrames / 2 : audioBuffer->bufferSizeInFrames;
    ma_uint32 currentSubBufferIndex = audioBuffer->frameCursor / subBufferSizeInFrames;

    if (currentSubBufferIndex > 1)
    {
        //TODO log
    }

    // Another thread can update the processed state of buffers so we just take a copy here to try and avoid potential synchronization problems.
    bool isSubBufferProcessed[2];
    isSubBufferProcessed[0] = audioBuffer->isProcessed[0];
    isSubBufferProcessed[1] = audioBuffer->isProcessed[1];

    ma_uint32 frameSizeInBytes = ma_get_bytes_per_sample(audioBuffer->rawPcm.formatConverterIn.config.formatIn)*audioBuffer->rawPcm.formatConverterIn.config.channels;

    // Fill out every frame until we find a buffer that's marked as processed. Then fill the remainder with 0.
    ma_uint32 framesRead = 0;
    for (;;)
    {
        // We break from this loop differently depending on the buffer's usage. For static buffers, we simply fill as much data as we can. For
        // streaming buffers we only fill the halves of the buffer that are processed. Unprocessed halves must keep their audio data in-tact.
        if (audioBuffer->playMode == EAudioMode::STATIC)
        {
            if (framesRead >= frameCount) break;
        }
        else
        {
            if (isSubBufferProcessed[currentSubBufferIndex]) break;
        }

        ma_uint32 totalFramesRemaining = (frameCount - framesRead);
        if (totalFramesRemaining == 0) break;

        ma_uint32 framesRemainingInOutputBuffer;
        if (audioBuffer->playMode == EAudioMode::STATIC)
        {
            framesRemainingInOutputBuffer = audioBuffer->bufferSizeInFrames - audioBuffer->frameCursor;
        }
        else
        {
            ma_uint32 firstFrameIndexOfThisSubBuffer = subBufferSizeInFrames * currentSubBufferIndex;
            framesRemainingInOutputBuffer = subBufferSizeInFrames - (audioBuffer->frameCursor - firstFrameIndexOfThisSubBuffer);
        }

        ma_uint32 framesToRead = totalFramesRemaining;
        if (framesToRead > framesRemainingInOutputBuffer) framesToRead = framesRemainingInOutputBuffer;

        memcpy((unsigned char *)pFramesOut + (framesRead*frameSizeInBytes), audioBuffer->buffer + (audioBuffer->frameCursor*frameSizeInBytes), framesToRead*frameSizeInBytes);
        audioBuffer->frameCursor = (audioBuffer->frameCursor + framesToRead) % audioBuffer->bufferSizeInFrames;
        framesRead += framesToRead;

        // If we've read to the end of the buffer, mark it as processed.
        if (framesToRead == framesRemainingInOutputBuffer)
        {
            audioBuffer->isProcessed[currentSubBufferIndex] = true;
            isSubBufferProcessed[currentSubBufferIndex] = true;

            currentSubBufferIndex = (currentSubBufferIndex + 1) % 2;

            // We need to break from this loop if we're not looping.
            if (!audioBuffer->isLooping)
            {
                StopAudioBuffer(audioBuffer);
                break;
            }
        }
    }

    // Zero-fill excess.
    ma_uint32 totalFramesRemaining = (frameCount - framesRead);
    if (totalFramesRemaining > 0)
    {
        memset((unsigned char *)pFramesOut + (framesRead*frameSizeInBytes), 0, totalFramesRemaining*frameSizeInBytes);

        // For static buffers we can fill the remaining frames with silence for safety, but we don't want
        // to report those frames as "read". The reason for this is that the caller uses the return value
        // to know whether or not a non-looping sound has finished playback.
        if (audioBuffer->playMode != EAudioMode::STATIC) framesRead += totalFramesRemaining;
    }

    return framesRead;
}

void InitAudioDevice()
{
    ma_context_config contextConfig = ma_context_config_init();
    //contextConfig.logCallback = OnLog;
    ma_result result = ma_context_init(NULL, 0, &contextConfig, &gAudioContext);

    if (result != MA_SUCCESS)
    {
        //TODO LOG
    }

    ma_device_config config = ma_device_config_init(ma_device_type::ma_device_type_playback);

    config.playback.pDeviceID = NULL; // Default
    config.playback.format = AS_OUTPUT_FORMAT;
    config.playback.channels = AS_OUTPUT_CHANNELS;
    config.capture.pDeviceID = NULL; // Default
    config.capture.format = ma_format_s16;
    config.capture.channels = 1;
    config.sampleRate = AS_OUTPUT_SAMPLE_RATE;
    config.dataCallback = OnSendAudioToDevice;
    //config.pUserData = &decoder;

    result = ma_device_init(&gAudioContext, &config, &gAudioDevice);
    if (result != MA_SUCCESS)
    {
        //TODO log
    }

    result = ma_device_start(&gAudioDevice);
    if (result != MA_SUCCESS)
    {
        //TODO log
    }
}

AudioBuffer* CreateAudioBuffer(ma_format format, uint32 channels, uint32 sampleRate, uint32 bufferSizeInFrames, EAudioMode mode)
{
    //AudioBuffer* audioBuffer = (AudioBuffer*)MEM_Alloc(sizeof(AudioBuffer));
    //audioBuffer->buffer = (byte*)MEM_Alloc(bufferSizeInFrames*channels*ma_get_bytes_per_sample(format));

    AudioBuffer* audioBuffer = (AudioBuffer *)calloc(sizeof(*audioBuffer) + (bufferSizeInFrames*channels*ma_get_bytes_per_sample(format)), 1);

    ma_pcm_converter_config pcmConverter;
    memset(&pcmConverter, 0, sizeof(pcmConverter)); //TODO is memset needed here? why?
    pcmConverter.formatIn = format;
    pcmConverter.formatOut = AS_OUTPUT_FORMAT;
    pcmConverter.channelsIn = channels;
    pcmConverter.channelsOut = AS_OUTPUT_CHANNELS;
    pcmConverter.sampleRateIn = sampleRate;
    pcmConverter.sampleRateOut = AS_OUTPUT_SAMPLE_RATE;
    pcmConverter.onRead = OnAudioBufferRead;
    pcmConverter.pUserData = audioBuffer;
    pcmConverter.allowDynamicSampleRate = MA_TRUE;
    ma_result result = ma_pcm_converter_init(&pcmConverter, &audioBuffer->rawPcm);

    if (result != MA_SUCCESS)
    {
        //TODO log
    }

    audioBuffer->volume = 1.f;
    audioBuffer->pitch = 1.f;
    audioBuffer->isPlaying = false;
    audioBuffer->isPaused = false;
    audioBuffer->isLooping = false;
    audioBuffer->playMode = mode;
    audioBuffer->bufferSizeInFrames = bufferSizeInFrames;

    //linked list related stuff
    //TrackAudioBuffer(audioBuffer);

    firstAudioBuffer = audioBuffer;

    return audioBuffer;
}

SoundWave LoadOgg(const char* fileName)
{
    SoundWave wave = {};

    //TODO see why stb_vorbis_open_file is not working

    //FILE* file = FS_ReadFile(fileName);
    //TODO we can pass an allocation buffer to stb_vorbis_open_file. See if needed
    //stb_vorbis *oggFile = stb_vorbis_open_file(file, false, NULL, NULL);

    stb_vorbis *oggFile = stb_vorbis_open_filename(fileName, 0, 0);

    if (oggFile == NULL)
    {
        //TODO: log error
    }

    else
    {
        stb_vorbis_info info = stb_vorbis_get_info(oggFile);
        wave.sampleRate = info.sample_rate;
        wave.sampleSize = 16;   //locked 16 bit sample size
        wave.channels = info.channels;
        wave.sampleCount = stb_vorbis_stream_length_in_samples(oggFile)*info.channels;
        wave.data = (int16*)MEM_Alloc(wave.sampleCount * sizeof(int16) *  wave.channels); //TODO we may need to multiply by channels too (i dont know why)

        stb_vorbis_get_samples_short_interleaved(oggFile, wave.channels, (int16*)wave.data, wave.sampleCount *  wave.channels);//TODO we may need to multiply sample count by channels too

        // Although we opened the file, we want vorbis to close it
        // for deinitialization reasons
        stb_vorbis_close(oggFile);
    }

    return wave;
}

void UnloadWave(SoundWave wave)
{
    if (wave.data != NULL)
        MEM_Release(wave.data);
}

Sound LoadSound(const char* fileName, EAudioFormat format)
{
    SoundWave wave;

    switch (format)
    {
    case EAudioFormat::OGG:
        wave = LoadOgg(fileName);
    }

    Sound sound = {};
    if (wave.data != NULL)
    {
        if (wave.sampleSize == 16)
        {
            ma_format format = ma_format_s16;
            uint32 frameCount = wave.sampleCount / wave.channels;

            uint32 frameCountOut = ma_convert_frames(
                NULL, AS_OUTPUT_FORMAT, AS_OUTPUT_CHANNELS, AS_OUTPUT_SAMPLE_RATE, NULL, format, wave.channels, wave.sampleRate, frameCount);

            AudioBuffer* audioBuffer = CreateAudioBuffer(
                AS_OUTPUT_FORMAT, AS_OUTPUT_CHANNELS, AS_OUTPUT_SAMPLE_RATE, frameCountOut, EAudioMode::STATIC);

            frameCount = (uint32)ma_convert_frames(
                audioBuffer->buffer,
                audioBuffer->rawPcm.formatConverterIn.config.formatIn,
                audioBuffer->rawPcm.formatConverterIn.config.channels,
                audioBuffer->rawPcm.src.config.sampleRateIn,
                wave.data,
                format,
                wave.channels,
                wave.sampleRate,
                frameCount);

            sound.audioBuffer = audioBuffer;
        }
        else
        {
            //TODO erro or assert : only supports 16bits for now
        }
    }
    else
    {
        //TODO erro or assert : data not inited
    }

    UnloadWave(wave);

    return sound;
}

void PlayAudioBuffer(AudioBuffer* audioBuffer)
{
    Assert(audioBuffer != NULL);

    audioBuffer->isPlaying = true;
    audioBuffer->isPaused = false;
    audioBuffer->frameCursor = 0;
}

void PlaySound(Sound sound)
{
    PlayAudioBuffer(sound.audioBuffer);
}

int main()
{
    InitAudioDevice();
    Sound s = LoadSound("data/sample.ogg", EAudioFormat::OGG);
    PlaySound(s);

    printf("%d\n", width);

    WND_CreateWindow(WIDTH, HEIGHT, TITLE);

    // Init engine modules
    ENG_Init();

    INP_SetKeyAction(GLFW_KEY_W, callback);

    REN_AddTexture(texture, width, height);

    REN_AddSprite(0, 50.f, 0.f, 200, 200, 0.f, 1.f);
    REN_AddSprite(0, 0.f, 50., 100.f, 100.f, 0.f, 1.f);

    //REN_Add(1, .5f, 0.5f, 100, 100);
    //REN_Add(4, 0, 0, 200, 30);

    while (!WND_HasRequestedQuit())
    {
        ENG_Tick();
    }
    WND_DestroyWindow();
}

/*
#include "audio/miniaudio.h"

#include <stdio.h>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

    (void)pInput;
}

int main(int argc, char** argv)
{
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file_wav("data/sample.wav", NULL, &decoder);
    if (result != MA_SUCCESS) {
        return -2;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    printf("Press Enter to quit...");
    getchar();

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    return 0;
}*/