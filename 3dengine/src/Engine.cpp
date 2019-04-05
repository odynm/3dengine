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
    Renderer::Draw();

    Time::EndFrame();
    Time::StartFrame();
}