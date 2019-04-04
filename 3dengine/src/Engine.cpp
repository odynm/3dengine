#include "Engine.h"

void
Engine::Init()
{
    Input::Init();
    Time::StartFrame();
}

void Engine::Tick()
{
    Window::PollEvents();

    Time::EndFrame();
    Time::StartFrame();
}