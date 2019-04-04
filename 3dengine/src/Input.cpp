#include "Input.h"

void
Input::Init()
{
    glfwSetKeyCallback(Window::window, InputCallback);
}

void
Input::SetKeyAction(int keycode, KeyActionFunc* func)
{
}

void
Input::InputCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
}