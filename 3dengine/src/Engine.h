#ifndef ENGINE_H
#define ENGINE_H

#define MAX_INPUT_ACTIONS 30

#include <stdint.h>
#include <stdio.h>

typedef int64_t int64;
typedef uint64_t uint64;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int8_t int8;
typedef uint8_t uint8;
typedef uint8_t byte;

#include "SystemTypes/String.h"

#include "Window.h"
#include "Time.h"
#include "Input.h"
#include "Renderer.h"

class Engine
{
public:
    static void Init();
    static void Tick();
private:
    Engine() {}
};

#endif