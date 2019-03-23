#include "Time.h"

double Time::delta;
clock_t Time::clockTime;

void Time::StartFrame()
{
    clockTime = clock();
}

void Time::EndFrame()
{
    // TODO: use a more reliable timer approach then CLOCKS_PER_SEC
    delta = (clock() - clockTime) / (double)CLOCKS_PER_SEC;
}