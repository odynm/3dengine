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
    Window::CreateWindow(WIDTH, HEIGHT, TITLE);

    Input::SetKeyAction(GLFW_KEY_W, callback);

    while (!Window::HasRequestedQuit())
    {
        Engine::Tick();
        
    }

    Window::DestroyWindow();
}