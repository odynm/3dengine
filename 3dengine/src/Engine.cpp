#include "Engine.h"

void
ENG_Init()
{
	LOG_Init();
	INP_Init();
	TIME_Init();
	REN_Init();
}

void
ENG_Tick()
{
	WND_PollEvents();
	REN_Draw();

#if DEBUG_INFO
	// Get the real time that the frame took
	double realFrameMS = TIME_GetRealFrameMS();
	// Log to console (for now)
	//printf("%fMS, %d FPS, DELTA: %f, REAL MS: %f\n", DB_FrameMS, DB_FPS, Delta, realFrameMS);
#endif

	// Cap framerate at 60FPS
	double accumulator = 0.0;
	do
	{
		TIME_EndProtoFrame();
		accumulator += ProtoFrameMS;
	} while (accumulator < MAX_MS_FRAME);
	TIME_EndFrame();
	TIME_StartFrame();
}