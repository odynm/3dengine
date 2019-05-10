#ifndef INPUT_H
#define INPUT_H

#include "SystemTypes/String.h"

#include "Engine.h"
#include "Window.h"
#include "SystemTypes/Typedefs.h"

typedef void KeyActionFunc(int state);

typedef struct sInputActions {
	int keycode;
	KeyActionFunc* func;
} InputActions;

void INP_Init();
void INP_SetKeyAction(int keycode, KeyActionFunc* func);

internal int numAddedActions;
internal InputActions actions[MAX_INPUT_ACTIONS];

internal void InputCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods);

#endif