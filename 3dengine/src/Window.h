#ifndef WINDOW_H
#define WINDOW_H

#include <glfw\glfw3.h>

class Window
{
public:
    static GLFWwindow* window;

    static void CreateWindow(int width, int height, const char* title);
    static bool HasRequestedQuit();
    static void PollEvents();
    static void DestroyWindow();

private:
    Window() { }
};

#endif