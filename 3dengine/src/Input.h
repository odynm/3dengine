#ifndef INPUT_H
#define INPUT_H

#include "SystemTypes/String.h"

#include "Engine.h"
#include "Window.h"

typedef void KeyActionFunc(int state);

typedef struct {
    int keycode;
    KeyActionFunc* func;
} InputActions;

class Input
{
public:
    static void Init();
    static void SetKeyAction(int keycode, KeyActionFunc* func);
private:
    Input() {};
    static void InputCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods);

    static InputActions actions[MAX_ACTIONS];
};

#endif