#pragma once

#include <time.h>

class Time
{
public:
    static double delta;
    static void StartFrame();
    static void EndFrame();
private:
    static clock_t clockTime;
};

