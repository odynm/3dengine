#include "Window.h"
#include "Engine.h"

GLFWwindow* Window::window;

void
Window::CreateWindow(int width, int height, const char * title)
{
    glfwInit();

    // Init glad and glfw context
    // This could be done on the rendering, but
    // I ultimately decided to leave here
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);

    // Init engine modules
    Engine::Init();
}

void
Window::DestroyWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool
Window::HasRequestedQuit()
{
    return glfwWindowShouldClose(window);
}

void
Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void 
Window::PollEvents() { glfwPollEvents(); }