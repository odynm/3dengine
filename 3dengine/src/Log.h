#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include "Memory.h"

class Log
{
public:
	static char* logBuffer;

	static void Init();
	static void GlfwErrorHandling(int error, const char* description);
private:
	Log() {};
};

#endif