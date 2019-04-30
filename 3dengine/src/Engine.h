#ifndef ENGINE_H
#define ENGINE_H

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

void MAIN_Init();
void MAIN_Tick();

#endif