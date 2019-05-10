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

	INP_SetKeyAction(GLFW_KEY_W, callback);

    REN_Add(0, 0, 0, 50, 100, 0);

	while (!WND_HasRequestedQuit())
	{
		MAIN_Tick();
	}

	WND_DestroyWindow();
}