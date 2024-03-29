#include "FileSystem.h"

String*
FS_ReadContent(const char* path)
{
    FILE* file = FS_OpenFile(path, "r");

    if (file != NULL)
    {
        uint64 fileSize = GetFileSize(file);
        char* buffer = (char*)MEM_Alloc(fileSize + 1);
        fread(buffer, 1, fileSize, file);
        buffer[fileSize] = NULL;
        fclose(file);
        return STR_NoAllocString(buffer);
    }
    else
    {
        return NULL;
    }
}

FILE*
FS_ReadFile(const char* path)
{
    FILE* file = FS_OpenFile(path, "r");
    return file;
}

FILE*
FS_OpenFile(const char* path, const char* mode)
{
    FILE* file = fopen(path, mode);
    if (file == NULL)
    {
        //TODO LOG ERROR
    }
    return file;
}

internal
uint64
GetFileSize(FILE* file)
{
    fseek(file, 0, SEEK_END);
    uint64 length = (uint64)ftell(file);
    fseek(file, 0, SEEK_SET);
    return length;
}