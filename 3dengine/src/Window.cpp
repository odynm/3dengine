#include "Window.h"
#include "Engine.h"

GLFWwindow* Window;

void
WND_CreateWindow(int width, int height, const char * title)
{
	glfwInit();

	glfwSetErrorCallback(LOG_GlfwErrorHandling);

	// Init glad and glfw context
	// This could be done on the rendering, but
	// I ultimately decided to leave here
	Window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(Window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(Window, FramebufferResizeCallback);
}

void
WND_DestroyWindow()
{
	glfwDestroyWindow(Window);
	glfwTerminate();
}

bool
WND_HasRequestedQuit()
{
	return glfwWindowShouldClose(Window);
}

void
WND_PollEvents() { glfwPollEvents(); }

internal void
FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}