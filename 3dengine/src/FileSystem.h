#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>

#include "SystemTypes/Typedefs.h"
#include "SystemTypes/String.h"

String* FS_ReadContent(const char* path);
FILE * FS_ReadFile(const char* path);
FILE * FS_OpenFile(const char* path, const char* mode);

internal uint64 GetFileSize(FILE* file);

#endif