#include "Engine.h"

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
	WND_CreateWindow(WIDTH, HEIGHT, TITLE);

	INP_SetKeyAction(GLFW_KEY_W, callback);

	REN_Test(vertices);

	while (!WND_HasRequestedQuit())
	{
		MAIN_Tick();
	}

	WND_DestroyWindow();
}