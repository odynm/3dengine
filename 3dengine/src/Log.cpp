#include "Log.h"

char* Log::logBuffer;

void
Log::Init()
{
	logBuffer = (char*)Memory::AllocFixed(1024);
}

void
Log::GlfwErrorHandling(int error, const char* description)
{
	printf("GLFW ERROR: %s\n", description);
}