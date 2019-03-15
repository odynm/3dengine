#pragma once

#include <glfw3.h>

class Window
{
public:
    static void CreateWindow(int width, int height, const char* title);
    static bool HasRequestedQuit();
    static void PollEvents();
    static void DestroyWindow();

private:
    Window(){ } //This classe should not be instanced
    static GLFWwindow* window;
};

