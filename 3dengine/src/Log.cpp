#include "Log.h"

char* LogBuffer;

void
LOG_Init()
{
	LogBuffer = (char*)MEM_AllocFixed(Kb(1));
}

void
LOG_RawLog()
{
	printf(LogBuffer);
}

void
LOG_Log(const char* description)
{
	printf("%s: %s\n", description, LogBuffer);
}

void
LOG_GlfwErrorHandling(int error, const char* description)
{
	printf("GLFW ERROR: %s\n", description);
}