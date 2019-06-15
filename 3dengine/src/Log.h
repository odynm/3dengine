#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include "Memory.h"
#include "SystemTypes/Typedefs.h"

#ifdef DEBUG_MODE
#define Assert(boolean) if (!boolean) int i = *((int*)NULL);
#else
#define Assert(boolean)
#endif

extern char* LogBuffer;

void LOG_Init();
void LOG_RawLog();
void LOG_Write(const char* text);
void LOG_Log(const char* description);
void LOG_GlfwErrorHandling(int error, const char* description);

#endif