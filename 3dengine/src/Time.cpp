#include "Time.h"

double Delta;
double ProtoFrameMS;
#if DEBUG_INFO
double DB_FrameMS;
int DB_FPS;
#endif

LARGE_INTEGER frequency;
LARGE_INTEGER counterValueAtStart;
LARGE_INTEGER protoFrameCounter;

void
TIME_Init()
{
#if PLAT_WINDOWS
	QueryPerformanceFrequency(&frequency);
#endif
}

void
TIME_StartFrame()
{
#if PLAT_WINDOWS
	QueryPerformanceCounter(&counterValueAtStart);
	protoFrameCounter = counterValueAtStart;
#endif
}

void
TIME_EndProtoFrame()
{
#if PLAT_WINDOWS
	LARGE_INTEGER currentCounterValue;
	QueryPerformanceCounter(&currentCounterValue);
	ProtoFrameMS = (((double)currentCounterValue.QuadPart - protoFrameCounter.QuadPart) / frequency.QuadPart) * 1000;
	protoFrameCounter = currentCounterValue;
#endif
}

void
TIME_EndFrame()
{
#if PLAT_WINDOWS
	LARGE_INTEGER currentCounterValue;
	QueryPerformanceCounter(&currentCounterValue);
	Delta = (((double)currentCounterValue.QuadPart - counterValueAtStart.QuadPart) / frequency.QuadPart);

#if DEBUG_INFO
	DB_FrameMS = Delta * 1000.0;
	DB_FPS = 1000 / (int)DB_FrameMS;
#endif

#endif
}

double
TIME_GetRealFrameMS()
{
	LARGE_INTEGER currentCounterValue;
	QueryPerformanceCounter(&currentCounterValue);
	return (((double)currentCounterValue.QuadPart - counterValueAtStart.QuadPart) / frequency.QuadPart) * 1000;
}