#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>

#include "SystemTypes/Typedefs.h"
#include "SystemTypes/String.h"

String* FS_ReadContent(const char* path);

internal uint64 GetFileSize(FILE* file);
internal FILE * OpenFile(const char* path, const char* mode);

#endif