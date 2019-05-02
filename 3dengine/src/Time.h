#ifndef TIMER_H
#define TIMER_H

#include <time.h>

#if PLAT_WINDOWS
#include <Windows.h>
#endif

#include "SystemTypes/Typedefs.h"

/*
Class used to obtain the time between frames and to cap
the framerate at 60FPS max (16.6ms frames)

In debug mode, it also gives some useful information like
FPS, FrameMS and the real frame interval took by the update
*/

extern double Delta;
extern double ProtoFrameMS;
#if DEBUG_INFO
extern int DB_FPS;
extern double DB_FrameMS;
#endif

void TIME_Init();
void TIME_StartFrame();
void TIME_EndFrame();

/// I call a 'proto frame' the frames where
/// no real update is made
void TIME_EndProtoFrame();

/// When called after a frame update, will return
/// the actual frame 'working' interval
double TIME_GetRealFrameMS();

#endif