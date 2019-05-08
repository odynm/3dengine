#include "Engine.h"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "3D ENGINE"

float const vertices[] = {
	10.f, 10.f, 0.f,
    10.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 10.f, 0.f,
};

uint const indices[] = {
    0,1,3,1,2,3
};

void callback(int state)
{
	printf("%d\n", state);
}

int main()
{
	WND_CreateWindow(WIDTH, HEIGHT, TITLE);

	INP_SetKeyAction(GLFW_KEY_W, callback);

	REN_Test(vertices, indices);

	while (!WND_HasRequestedQuit())
	{
		MAIN_Tick();
	}

	WND_DestroyWindow();
}