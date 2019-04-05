#include "Input.h"

InputActions Input::actions[MAX_INPUT_ACTIONS];
int Input::numAddedActions = 0;

void
Input::Init()
{
    glfwSetKeyCallback(Window::window, InputCallback);
}

void
Input::SetKeyAction(int keycode, KeyActionFunc* func)
{
    InputActions action = {
        keycode = keycode,
        func = func
    };

    actions[numAddedActions] = action;
    numAddedActions++;
}

void
Input::InputCallback(GLFWwindow* window, int keycode, int scancode, int state, int mods)
{
    // This can be made more efficient later,
    // but will probably not matter much because of the
    // function call that will wipe the cache anyway
    for (int i = 0; i < numAddedActions + 1; i++)
    {
        if (actions[i].keycode == keycode)
            actions[i].func(state);
    }
}