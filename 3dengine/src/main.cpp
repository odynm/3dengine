#include "Engine.h"
#include "stb_image/stb_image.h"
#include "OpenAL/al.h"
#include "OpenAL/alc.h"
#include "stb_vorbis/stb_vorbis.c"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "3D ENGINE"

int width, height, nrChannels;
byte *texture = stbi_load("data/duck_shades.png", &width, &height, &nrChannels, STBI_rgb_alpha);

ALCdevice* device;
ALCcontext* context;

ALuint audioSource;
ALuint audioBuffer;

stb_vorbis* vBuffer;
stb_vorbis_info vInfo;

const size_t bufferSize = 192644;

void callback(int state)
{
    printf("%d\n", state);
}

int main()
{
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);

    alGenSources((ALuint)1, &audioSource);
    alSourcef(audioSource, AL_PITCH, 1);
    alSourcef(audioSource, AL_GAIN, 1);
    alSource3f(audioSource, AL_POSITION, 0, 0, 0);
    alSource3f(audioSource, AL_VELOCITY, 0, 0, 0);
    alSourcei(audioSource, AL_LOOPING, AL_FALSE);

    alGenBuffers((ALuint)1, &audioBuffer);

    char filename[] = "data/sample.ogg";
    vBuffer = stb_vorbis_open_filename(filename, NULL, NULL);
    vInfo = stb_vorbis_get_info(vBuffer);
    ALenum format;
    if (vInfo.channels == 2) format = AL_FORMAT_STEREO16;
    else format = AL_FORMAT_MONO16;
    ALshort* pcm = (ALshort*)MEM_Alloc(bufferSize);
    ALshort* pcm2 = (ALshort*)MEM_Alloc(bufferSize);

    stb_vorbis_get_samples_short_interleaved(vBuffer, vInfo.channels, pcm, bufferSize);

    alBufferData(audioBuffer, format, vBuffer, bufferSize, vInfo.sample_rate);

    alSourcei(audioSource, AL_BUFFER, audioBuffer);

    alSourcePlay(audioSource);
    for (int i = 0; i < 30000000; i++);

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