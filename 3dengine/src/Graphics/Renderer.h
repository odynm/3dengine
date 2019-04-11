#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Engine.h"
#include "../Window.h"

class Renderer
{
public:
    static void Init();
    static void Test(float const vertexData[]);
    static void Draw();
private:
    Renderer() { }
    static uint vertexBufferObject;
};

#endif