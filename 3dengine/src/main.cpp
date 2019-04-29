#include "Engine.h"

#include <Windows.h>

#define WIDTH 800
#define HEIGHT 600
#define TITLE "3D ENGINE"

float const vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, -0.5f, 0.0f
};

void callback(int state)
{
	printf("%d\n", state);
}

int main()
{
	Window::CreateWindow(WIDTH, HEIGHT, TITLE);

	Input::SetKeyAction(GLFW_KEY_W, callback);

	Renderer::Test(vertices);

	while (!Window::HasRequestedQuit())
	{
		Engine::Tick();
	}

	Window::DestroyWindow();
}