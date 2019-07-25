#ifndef AUDIO_H
#define AUDIO_H

/*
This audio implementation has Miniaudio as a basis.
We need to be careful because audios are played on separated threads to avoid lock,
so is important to avoid syncronization issues.

To be honest, this is the part where I've most problems developing, specially because
the docs on the subject are scarce.

I may consider moving to the Handmade Hero way of doing it directly on Windows if I get
into trouble.
*/

#include <stdlib.h>
#include "audio\stb_vorbis.h"
#include "audio\miniaudio.h"

#include "SystemTypes\Typedefs.h"
#include "Log.h"
#include "FileSystem.h"
#include "Memory.h"

#define AS_OUTPUT_FORMAT ma_format_f32
#define AS_OUTPUT_CHANNELS 2
#define AS_OUTPUT_SAMPLE_RATE  44100

enum class EAudioFormat {
    OGG
};

enum class EAudioMode {
    STATIC, STREAM
};

typedef struct SAudioBuffer {
    ma_pcm_converter rawPcm;
    byte* buffer;
    SAudioBuffer* next;
    SAudioBuffer* prev;
    EAudioMode playMode;
    uint bufferSizeInFrames;
    uint frameCursor;
    bool isLooping;
    bool isPaused;
    bool isPlaying;
    bool isProcessed[2];                    // Seems like it's good to double check because of threading issues
    float pitch;
    float volume;
} AudioBuffer;

typedef struct SSoundWave {
    uint sampleCount;                       // Number of samples
	uint sampleRate;                        // Frequency
	uint sampleSize;                        // Bits per sample: 8, 16, 32 (currently locked 16bit)
	uint channels;                          // Number of channels - 1: mono, 2: stereo
    void *data;                             // Data pointer
} SoundWave;

typedef struct SSound {
    int format;                             // Audio format
	uint source;                            // Audio source id
	uint buffer;                            // Audio buffer id
    AudioBuffer* audioBuffer;               // Audio buffer
} Sound;

internal ma_context gAudioContext;
internal ma_device gAudioDevice;

void AUD_InitAudioDevice();
SoundWave AUD_LoadOggWave(const char* fileName);
void AUD_UnloadWave(SoundWave wave);
Sound AUD_LoadSound(const char* fileName, EAudioFormat format);
void AUD_PlaySound(Sound sound);
AudioBuffer* AUD_CreateAudioBuffer(ma_format format, uint channels, uint sampleRate, uint bufferSizeInFrames, EAudioMode mode);
void AUD_PlayAudioBuffer(AudioBuffer* audioBuffer);
void AUD_StopAudioBuffer(AudioBuffer *audioBuffer);

internal void MixAudioFrames(float *framesOut, const float *framesIn, ma_uint32 frameCount, float localVolume);
internal void OnSendAudioToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput, ma_uint32 frameCount);
internal ma_uint32 OnAudioBufferRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount, void *pUserData);

#endif