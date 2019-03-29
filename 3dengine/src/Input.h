#ifndef INPUT_H
#define INPUT_H

#include "Window.h"

class Input
{
public:
    bool GetKeyDown(int keycode);
    bool GetKeyDown();
private:
    Input() {};
};

#endif