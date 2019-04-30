#include "Log.h"

char* logBuffer;

void
LOG_Init()
{
	logBuffer = (char*)MEM_AllocFixed(1024);
}

void
LOG_GlfwErrorHandling(int error, const char* description)
{
	printf("GLFW ERROR: %s\n", description);
}