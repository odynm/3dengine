#include "Input.h"

void
INP_Init()
{
    glfwSetKeyCallback(Window, InputCallback);
}

void
INP_SetKeyAction(int keycode, KeyActionFunc* func)
{
    InputActions action = {
        keycode = keycode,
        func = func
    };

    actions[numAddedActions] = action;
    numAddedActions++;
}

void
InputCallback(GLFWwindow* window, int keycode, int scancode, int state, int mods)
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