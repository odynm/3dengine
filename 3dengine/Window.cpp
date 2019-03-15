#include "Window.h"

GLFWwindow* Window::window;

void 
Window::CreateWindow(int width, int height, const char * title)
{
    glfwInit();
    window = glfwCreateWindow(width, height, title, NULL, NULL);
}

void
Window::DestroyWindow() 
{
    glfwDestroyWindow(window);
}

bool
Window::HasRequestedQuit()
{
    return glfwWindowShouldClose(window);
}

void Window::PollEvents() { glfwPollEvents(); }
