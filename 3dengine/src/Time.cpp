#include "Time.h"

double delta;

void
TIME_StartFrame()
{
	clockTime = clock();
}

void
TIME_EndFrame()
{
	// TODO: use a more reliable timer approach then CLOCKS_PER_SEC
	delta = (clock() - clockTime) / (double)CLOCKS_PER_SEC;
}