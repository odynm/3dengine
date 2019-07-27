#include "Audio.h"

//TODO MUSIC STREAMING

//TODO not needed later on when we will have our way to keep track of them
AudioBuffer* firstAudioBuffer;

void
AUD_InitAudioDevice()
{
    ma_context_config contextConfig = ma_context_config_init();
    //contextConfig.logCallback = OnLog;
    ma_result result = ma_context_init(NULL, 0, &contextConfig, &gAudioContext);

    if (result != MA_SUCCESS)
    {
        //TODO LOG
    }

    ma_device_config config = ma_device_config_init(ma_device_type::ma_device_type_playback);

    config.playback.pDeviceID = NULL; // Use default
    config.playback.format = AS_OUTPUT_FORMAT;
    config.playback.channels = AS_OUTPUT_CHANNELS;
    config.sampleRate = AS_OUTPUT_SAMPLE_RATE;
    config.dataCallback = OnSendAudioToDevice;

    // We'll probably not use capture...
    config.capture.format = ma_format_s16;
    config.capture.channels = 1;

    // Init device (driver stuff)
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

SoundWave
AUD_LoadOggWave(const char* fileName)
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
        wave.data = (int16*)MEM_Alloc(wave.sampleCount * sizeof(int16)); //TODO we may need to multiply by channels too (i dont know why)

        stb_vorbis_get_samples_short_interleaved(oggFile, wave.channels, (int16*)wave.data, wave.sampleCount);//TODO we may need to multiply sample count by channels too

        stb_vorbis_close(oggFile);
    }

    return wave;
}

void
AUD_UnloadWave(SoundWave wave)
{
    if (wave.data != NULL)
        MEM_Release(wave.data);
}

Sound
AUD_LoadSound(const char* fileName, EAudioFormat format)
{
    SoundWave wave;

    switch (format)
    {
    case EAudioFormat::OGG:
        wave = AUD_LoadOggWave(fileName);
    }

    Sound sound = {};
    if (wave.data != NULL)
    {
        if (wave.sampleSize == 16)
        {
            ma_format format = ma_format_s16;
            uint frameCount = wave.sampleCount / wave.channels;

            uint frameCountOut = ma_convert_frames(
                NULL, AS_OUTPUT_FORMAT, AS_OUTPUT_CHANNELS, AS_OUTPUT_SAMPLE_RATE, NULL, format, wave.channels, wave.sampleRate, frameCount);

            // Create buffer
            AudioBuffer* audioBuffer = AUD_CreateAudioBuffer(
                AS_OUTPUT_FORMAT, AS_OUTPUT_CHANNELS, AS_OUTPUT_SAMPLE_RATE, frameCountOut, EAudioMode::STATIC);

            // Convert from format
            frameCount = (uint)ma_convert_frames(
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

    AUD_UnloadWave(wave);

    return sound;
}

void
AUD_PlaySound(Sound sound)
{
    AUD_PlayAudioBuffer(sound.audioBuffer);
}

AudioBuffer*
AUD_CreateAudioBuffer(ma_format format, uint channels, uint sampleRate, uint bufferSizeInFrames, EAudioMode mode)
{
    AudioBuffer* audioBuffer = (AudioBuffer*)MEM_Alloc(sizeof(AudioBuffer));
    audioBuffer->buffer = (byte*)MEM_Alloc(bufferSizeInFrames*channels*ma_get_bytes_per_sample(format));

    //AudioBuffer* audioBuffer = (AudioBuffer *)calloc(sizeof(*audioBuffer) + (bufferSizeInFrames*channels*ma_get_bytes_per_sample(format)), 1);

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

void
AUD_PlayAudioBuffer(AudioBuffer* audioBuffer)
{
    Assert(audioBuffer != NULL);

    audioBuffer->isPlaying = true;
    audioBuffer->isPaused = false;
    audioBuffer->frameCursor = 0;
}

void
AUD_StopAudioBuffer(AudioBuffer *audioBuffer)
{
    if (audioBuffer == NULL)
    {
        //TODO log
    }

    // Don't do anything if the audio has already stopped
    if (!audioBuffer->isPlaying && !audioBuffer->isPaused) return;

    audioBuffer->isPlaying = false;
    audioBuffer->isPaused = false;
    audioBuffer->frameCursor = 0;
    audioBuffer->isProcessed[0] = true;
    audioBuffer->isProcessed[1] = true;
}

// This is where we can mix the sound, but we'll not do anything special
internal void
MixAudioFrames(float *framesOut, const float *framesIn, uint frameCount, float volume)
{
    for (uint iFrame = 0; iFrame < frameCount; ++iFrame)
    {
        for (uint iChannel = 0; iChannel < gAudioDevice.playback.channels; ++iChannel)
        {
            // Increment pointers...
            float *frameOut = framesOut + (iFrame*gAudioDevice.playback.channels);
            const float *frameIn = framesIn + (iFrame*gAudioDevice.playback.channels);

            // ...and we just pass it back
            frameOut[iChannel] += (frameIn[iChannel] * 1 * volume);
        }
    }
}

// This is where we recieve a callback from sending data to the audio device
internal void
OnSendAudioToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesIn, uint frameCount)
{
    Assert(pDevice);

    // Init the output buffer to 0
    memset(pFramesOut, 0, frameCount*pDevice->playback.channels*ma_get_bytes_per_sample(pDevice->playback.format));

    // Locking this for thread-safety which makes things NOT real-time. Don't know yet how to avoid this - or if we need to do so
    ma_mutex_lock(&gAudioLock);
    {
        //TODO linked chain
        for (AudioBuffer *audioBuffer = firstAudioBuffer; audioBuffer != NULL; audioBuffer = audioBuffer->next)
        {
            if (!audioBuffer->isPlaying || audioBuffer->isPaused) continue;

            uint framesRead = 0;
            while (true)
            {
                if (framesRead >= frameCount) break;

                // We will read as much data as possible from the input stream
                uint framesToRead = (frameCount - framesRead);
                while (framesToRead > 0)
                {
                    float tempBuffer[1024]; // 512 for stereo

                    uint framesToReadNow = framesToRead;

                    // If the buffer is not big enough, read as much as possible
                    if (framesToReadNow > sizeof(tempBuffer) / sizeof(tempBuffer[0]) / AS_OUTPUT_CHANNELS)
                    {
                        framesToReadNow = sizeof(tempBuffer) / sizeof(tempBuffer[0]) / AS_OUTPUT_CHANNELS;
                    }

                    uint framesJustReaded = (uint)ma_pcm_converter_read(&audioBuffer->rawPcm, tempBuffer, framesToReadNow);

                    if (framesJustReaded > 0)
                    {
                        float *framesOut = (float*)pFramesOut + (framesRead*gAudioDevice.playback.channels); // Go to the next still available space on out buffer
                        float *framesIn = tempBuffer;
                        MixAudioFrames(framesOut, framesIn, framesJustReaded, audioBuffer->volume);

                        framesToRead -= framesJustReaded;
                        framesRead += framesJustReaded;
                    }

                    // If we can't complete the read, we are probably done
                    if (framesJustReaded < framesToReadNow)
                    {
                        if (!audioBuffer->isLooping)
                        {
                            AUD_StopAudioBuffer(audioBuffer);
                            break;
                        }
                        else
                        {
                            // If we ARE looping and done, something went wrong. Reset the cursor and keep going
                            audioBuffer->frameCursor = 0;
                            continue;
                        }
                    }
                }

                // If we don't read anything, break off
                if (framesToRead > 0) break;
            }
        }

        ma_mutex_unlock(&gAudioLock);
    }
}

// Called when the audio buffer is read
// Here we make a distintion between STREAM and STATIC playing
internal uint
OnAudioBufferRead(ma_pcm_converter *pDSP, void *pFramesOut, uint frameCount, void *pUserData)
{
    // The audio buffer was stored on the userData pointer
    AudioBuffer *audioBuffer = (AudioBuffer *)pUserData;

    // Take the position of the frame cursor to fit inside the sub buffer
    // A sub buffer has half the size of the buffer (a processed and a unprocessed - index 0 and 1)
    uint subBufferSizeInFrames = (audioBuffer->bufferSizeInFrames > 1) ?
        audioBuffer->bufferSizeInFrames / 2 : audioBuffer->bufferSizeInFrames;
    uint currentSubBufferIndex = audioBuffer->frameCursor / subBufferSizeInFrames;

    if (currentSubBufferIndex > 1)
    {
        //TODO log - sub buffer index is outside the sub buffer size
    }

    // Sub buffers processed flags - we take a copy to avoid sync problems
    bool isSubBufferProcessed[2];
    isSubBufferProcessed[0] = audioBuffer->isProcessed[0];
    isSubBufferProcessed[1] = audioBuffer->isProcessed[1];

    uint frameSizeInBytes = ma_get_bytes_per_sample(audioBuffer->rawPcm.formatConverterIn.config.formatIn)*audioBuffer->rawPcm.formatConverterIn.config.channels;

    // Fill all frames until we find a processed buffer
    uint framesRead = 0;
    while (true)
    {
        if (audioBuffer->playMode == EAudioMode::STATIC)
        {   // If STATIC, we fill as much of the buffer as we can
            if (framesRead >= frameCount) break;
        }
        else
        {   // If STREAM, we fill only the half of the buffer that's processed. The unprocessed we don't touch
            if (isSubBufferProcessed[currentSubBufferIndex]) break;
        }

        // Check if we are done reading the frames
        uint totalFramesRemaining = (frameCount - framesRead);
        if (totalFramesRemaining == 0) break;

        uint framesRemainingInOutputBuffer;
        if (audioBuffer->playMode == EAudioMode::STATIC)
        {   // If STATIC, how much until we reach the end of the buffer with the frame cursor
            framesRemainingInOutputBuffer = audioBuffer->bufferSizeInFrames - audioBuffer->frameCursor;
        }
        else
        {   //If STREAM, where we are at this sub buffer - and how much until the frame cursor reach the end
            uint firstFrameIndexOfThisSubBuffer = subBufferSizeInFrames * currentSubBufferIndex;
            framesRemainingInOutputBuffer = subBufferSizeInFrames - (audioBuffer->frameCursor - firstFrameIndexOfThisSubBuffer);
        }

        // Cap the number of frames to read to the remaining space on the output buffer
        uint framesToRead = totalFramesRemaining;
        if (framesToRead > framesRemainingInOutputBuffer) framesToRead = framesRemainingInOutputBuffer;

        memcpy((byte*)pFramesOut + (framesRead*frameSizeInBytes), audioBuffer->buffer + (audioBuffer->frameCursor*frameSizeInBytes), framesToRead*frameSizeInBytes);
        audioBuffer->frameCursor = (audioBuffer->frameCursor + framesToRead) % audioBuffer->bufferSizeInFrames;
        framesRead += framesToRead;

        // If wea are at the end of the buffer, mark it as processed
        if (framesToRead == framesRemainingInOutputBuffer)
        {
            // Again, avoiding sync issues
            audioBuffer->isProcessed[currentSubBufferIndex] = true;
            isSubBufferProcessed[currentSubBufferIndex] = true;

            currentSubBufferIndex = (currentSubBufferIndex + 1) % 2;

            // If we're done and not looping, we'are done
            if (!audioBuffer->isLooping)
            {
                AUD_StopAudioBuffer(audioBuffer);
                break;
            }
        }
    }

    // Fill the rest with zeros
    uint totalFramesRemaining = (frameCount - framesRead);
    if (totalFramesRemaining > 0)
    {
        memset((unsigned char *)pFramesOut + (framesRead*frameSizeInBytes), 0, totalFramesRemaining*frameSizeInBytes);

        // The silence doesn't count as sound in STATIC because we can then say that the audio has finished
        if (audioBuffer->playMode != EAudioMode::STATIC)
            framesRead += totalFramesRemaining;
    }

    return framesRead;
}