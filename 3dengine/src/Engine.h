#ifndef ENGINE_H
#define ENGINE_H

#define MAX_INPUT_ACTIONS 30

#include <stdio.h>

#include "SystemTypes/Typedefs.h"
#include "SystemTypes/String.h"

#include "FileSystem.h"
#include "Graphics/Renderer.h"
#include "Input.h"
#include "Log.h"
#include "Time.h"
#include "Window.h"

class Engine
{
public:
    static void Init();
    static void Tick();
private:
    Engine() {}
};

#endif