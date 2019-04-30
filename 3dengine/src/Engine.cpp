#include "Engine.h"

void
MAIN_Init()
{
	LOG_Init();
	INP_Init();
	TIME_StartFrame();
	REN_Init();
}

void
MAIN_Tick()
{
	WND_PollEvents();
	REN_Draw();

	TIME_EndFrame();
	TIME_StartFrame();
}