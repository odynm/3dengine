#include "Engine.h"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "3D ENGINE"

int main()
{
    Window::CreateWindow(WIDTH, HEIGHT, TITLE);

    String str = String("Teste");
    printf(str);

    while (!Window::HasRequestedQuit())
    {
        Engine::Tick();
    }

    Window::DestroyWindow();
}