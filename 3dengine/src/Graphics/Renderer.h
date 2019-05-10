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

#define NUMBER_OF_LAYERS 10

/// Indeices used to draw a quad (two triangles)
uint const indices[] = {
    0,1,3,
    1,2,3
};

//TODO think on a better name for this
typedef struct sRenderingObject {
    uint VAO;
    uint VBO;
} RenderingObject;

void REN_Init();
void REN_Add(int layer, int x, int y, int w, int h, int index);
void REN_Draw();

#endif