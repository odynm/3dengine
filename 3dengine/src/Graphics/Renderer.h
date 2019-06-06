#ifndef RENDERER_H
#define RENDERER_H

/*
(Un)Supported features:
* size is fixed
* only supported quads for sprite rendering
* dynamic position and texture coord

*/

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
void REN_AddTexture(byte* textureBuffer, int width, int height);
void REN_AddSprite(int layer, float x, float y, float w, float h, float tx, float ty);
void REN_Draw();

internal RenderSet** layers;

#endif