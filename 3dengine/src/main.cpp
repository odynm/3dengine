#include "Engine.h"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "3D ENGINE"

void callback(int state)
{
    printf("%d\n", state);
}

int main()
{
    WND_CreateWindow(WIDTH, HEIGHT, TITLE);
    // Init engine modules
    ENG_Init();

    INP_SetKeyAction(GLFW_KEY_W, callback);

    REN_Add(0, 100.f, 0.f, 200.f, 200.f);
    REN_Add(0, 0.f, 50., 200.f, 20.f);
    //REN_Add(1, .5f, 0.5f, 100, 100);
    //REN_Add(4, 0, 0, 200, 30);

    while (!WND_HasRequestedQuit())
    {
        ENG_Tick();
    }

    WND_DestroyWindow();
}