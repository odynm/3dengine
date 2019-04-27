#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#undef CreateWindow // 'Someone' defined this

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
    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

    static void error_callback(int error, const char* description);
};

#endif