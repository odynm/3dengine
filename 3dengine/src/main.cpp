//TODO logging:
/*
in order to know what the f is happening on execution time, use a log function
put it all over the place, not just errors
is easy enough to make a macro that will not log at runtime (make a verbose flag, maybe)
*/

#include "Engine.h"
#include "stb_image/stb_image.h"
#include "stb_vorbis/stb_vorbis.c"

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
typedef struct SSoundWave {
    uint32 sampleCount;     // Number of samples
    uint32 sampleRate;      // Frequency
    uint32 sampleSize;      // Bits per sample: 8, 16, 32 (currently locked 16bit)
    uint32 channels;        // Number of channels - 1: mono, 2: stereo
    void *data;             // Data pointer
} SoundWave;

SoundWave LoadOgg(const char* fileName)
{
    SoundWave wave = {};

    FILE* file = FS_ReadFile("sample.ogg");
    //TODO we can pass an allocation buffer to stb_vorbis_open_file. See if needed
    stb_vorbis *oggFile = stb_vorbis_open_file(file, false, NULL, NULL);

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

        // Although we opened the file, we want vorbis to close it 
        // for deinitialization reasons
        stb_vorbis_close(oggFile);
        return wave;
    }


}   




int main()
{

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