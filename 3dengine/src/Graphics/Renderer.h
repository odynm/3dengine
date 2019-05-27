#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../SystemTypes/Typedefs.h"
#include "../SystemTypes/String.h"
#include "../FileSystem.h"
#include "../Window.h"
#include "../Log.h"
#include "../Math.h"
#include "RenderSet.h"

#define NUMBER_OF_LAYERS 5

/// Indices used to draw a quad (two triangles)
uint const indices[] = {
    0,1,3,
    1,2,3
};

void REN_Init();
void REN_Add(int layer, float x, float y, float w, float h);
void REN_Draw();

internal RenderSet** layers;

#endif