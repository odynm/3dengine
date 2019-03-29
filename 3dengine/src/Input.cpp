#include "Input.h"

bool Input::GetKeyDown(int keycode)
{
    //return glfwGetKey(window, keycode) == GLFW_PRESS;
}

bool Input::GetKeyDown(string keyName)
{
    return false;
}

bool Input::GetKeyUp(int keycode)
{
    return false;
}

bool Input::GetKeyUp(string keyName)
{
    return false;
}
