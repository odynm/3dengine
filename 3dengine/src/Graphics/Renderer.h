#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../SystemTypes/Typedefs.h"
#include "../SystemTypes/String.h"
#include "../FileSystem.h"
#include "../Window.h"

class Renderer
{
public:
    static void Init();
    static void Test(float const vertexData[]);
    static void Draw();
private:
    Renderer() { }
    static uint VBO;
    static uint VAO;
    static uint shaderProgram;
};

#endif