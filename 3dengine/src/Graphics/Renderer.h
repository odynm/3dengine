#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../SystemTypes/Typedefs.h"
#include "../SystemTypes/String.h"
#include "../FileSystem.h"
#include "../Window.h"
#include "../Log.h"

void REN_Init();
void REN_Test(float const vertexData[]);
void REN_Draw();

#endif