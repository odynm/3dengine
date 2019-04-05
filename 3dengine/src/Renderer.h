#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

class Renderer
{
public:
    static void Init();
    static void Draw();
private:
    Renderer() { }
};

#endif