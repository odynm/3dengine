#include "Window.h"
#include "Engine.h"

void
WND_CreateWindow(int width, int height, const char * title)
{
    glfwInit();

    glfwSetErrorCallback(LOG_GlfwErrorHandling);

    // Init glad and glfw context
    // This could be done on the rendering, but
    // I ultimately decided to leave here
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);

    // Init engine modules
    MAIN_Init();
}

void
WND_DestroyWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool
WND_HasRequestedQuit()
{
    return glfwWindowShouldClose(window);
}

void 
WND_PollEvents() { glfwPollEvents(); }

internal
void
FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}