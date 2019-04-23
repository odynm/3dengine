#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

// We are going to do dangerous stuff in here
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>

#include "SystemTypes/Typedefs.h"
#include "SystemTypes/String.h"

class FileSystem
{
public:
    static String* ReadContent(const char* path);
private:
    FileSystem() {};
    static uint64 GetFileSize(FILE* file);
	static FILE * OpenFile(const char* path, const char* mode);
};

#endif