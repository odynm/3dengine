#ifndef ENGINE_H
#define ENGINE_H

#define MAX_FPS 60
#define MAX_MS_FRAME ((1.0/MAX_FPS) * 1000.0) - 0.01
#define MAX_INPUT_ACTIONS 30

#include <stdio.h>

#include "SystemTypes/Typedefs.h"
#include "SystemTypes/String.h"

#include "Memory.h"
#include "FileSystem.h"
#include "Graphics/Renderer.h"
#include "Input.h"
#include "Log.h"
#include "Time.h"
#include "Window.h"

void ENG_Init();
void ENG_Tick();

#endif