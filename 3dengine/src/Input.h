#ifndef INPUT_H
#define INPUT_H

/*
 I've decided to use polling. An event approach could be nice.
*/

#include "SystemTypes/String.h"

#include "Window.h"

class Input
{
public:
    bool GetKeyDown(int keycode);
    bool GetKeyDown(string keyName);
    bool GetKeyUp(int keycode);
    bool GetKeyUp(string keyName);
private:
    Input() {};
};

#endif