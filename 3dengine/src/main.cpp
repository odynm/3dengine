#include "Engine.h"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "3D ENGINE"

int main()
{
    Window::CreateWindow(WIDTH, HEIGHT, TITLE);

    while (!Window::HasRequestedQuit())
    {
        Time::StartFrame();

        Window::PollEvents();

        int var = 0;
        for (int i = 0; i < 900000; i++)
        {
            var++;
        }

        Time::EndFrame();
    }

    Window::DestroyWindow();
}