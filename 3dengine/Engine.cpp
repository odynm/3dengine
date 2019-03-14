#include "Engine.h"

int main() 
{
    Window::CreateWindow(WIDTH, HEIGHT, TITLE);

    while (!Window::HasRequestedQuit()) 
    {
        Window::PollEvents();
    }
}