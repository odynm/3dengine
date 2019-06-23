#ifndef AUDIO_H
#define AUDIO_H

#include <stdlib.h>
#include "audio\stb_vorbis.c"
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
    ma_pcm_converter rawPcm;                  // Required for format conversion
    float volume;
    float pitch;
    bool isPlaying;
    bool isPaused;
    bool isLooping;                           // Always true for AudioStreams
    EAudioMode playMode;                      // AudioBufferUsage type
    bool isProcessed[2];
    unsigned int frameCursor;
    unsigned int bufferSizeInFrames;
    rAudioBuffer *next;
    rAudioBuffer *prev;
    unsigned char buffer[1];
} AudioBuffer;

typedef struct SSoundWave {
    uint32 sampleCount;                       // Number of samples
    uint32 sampleRate;                        // Frequency
    uint32 sampleSize;                        // Bits per sample: 8, 16, 32 (currently locked 16bit)
    uint32 channels;                          // Number of channels - 1: mono, 2: stereo
    void *data;                               // Data pointer
} SoundWave;

typedef struct SSound {
    int format;                               // Audio format
    uint32 source;                            // Audio source id
    uint32 buffer;                            // Audio buffer id
    AudioBuffer* audioBuffer;                 // Audio buffer
} Sound;

internal ma_context gAudioContext;
internal ma_device gAudioDevice;

void AUD_InitAudioDevice();
SoundWave AUD_LoadOggWave(const char* fileName);
void AUD_UnloadWave(SoundWave wave);
Sound AUD_LoadSound(const char* fileName, EAudioFormat format);
void AUD_PlaySound(Sound sound);
AudioBuffer* AUD_CreateAudioBuffer(ma_format format, uint32 channels, uint32 sampleRate, uint32 bufferSizeInFrames, EAudioMode mode);
void AUD_PlayAudioBuffer(AudioBuffer* audioBuffer);
void AUD_StopAudioBuffer(AudioBuffer *audioBuffer);

internal void MixAudioFrames(float *framesOut, const float *framesIn, ma_uint32 frameCount, float localVolume);
internal void OnSendAudioToDevice(ma_device *pDevice, void *pFramesOut, const void *pFramesInput, ma_uint32 frameCount);
internal ma_uint32 OnAudioBufferRead(ma_pcm_converter *pDSP, void *pFramesOut, ma_uint32 frameCount, void *pUserData);

#endif