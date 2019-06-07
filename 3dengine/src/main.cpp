#include "Engine.h"
#include "stb_image/stb_image.h"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "3D ENGINE"

int width, height, nrChannels;
byte *texture = stbi_load("data/duck_shades.png", &width, &height, &nrChannels, STBI_rgb_alpha);

void callback(int state)
{
    printf("%d\n", state);
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