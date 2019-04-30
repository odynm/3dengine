#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include "Memory.h"
#include "SystemTypes/Typedefs.h"

void LOG_Init();
void LOG_GlfwErrorHandling(int error, const char* description);

#endif