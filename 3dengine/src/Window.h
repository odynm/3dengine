#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "SystemTypes/Typedefs.h"

#undef CreateWindow // 'Someone' defined this

extern GLFWwindow* Window;

void WND_CreateWindow(int width, int height, const char* title);
bool WND_HasRequestedQuit();
void WND_PollEvents();
void WND_DestroyWindow();

internal void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

#endif