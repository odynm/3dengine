#ifndef TIMER_H
#define TIMER_H

#include <time.h>

#include "SystemTypes/Typedefs.h"

extern double Delta;

void TIME_StartFrame();
void TIME_EndFrame();

internal clock_t clockTime;

#endif