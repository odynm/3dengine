#include "FileSystem.h"

String*
FileSystem::ReadContent(const char* path)
{
    uint64 fileSize = GetFileSize(path);
    char* buffer = (char*)calloc(fileSize, 1);

    FILE* file = fopen(path, "r");
    fread(buffer, 1, fileSize, file);
    fclose(file);

    return String::NoAllocString(buffer);
}

uint64
FileSystem::GetFileSize(const char* path)
{
    FILE* file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    uint64 length = (uint64)ftell(file);
    fclose(file);
    return length;
}