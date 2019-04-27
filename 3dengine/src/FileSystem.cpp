#include "FileSystem.h"

String*
FileSystem::ReadContent(const char* path)
{
    FILE* file = OpenFile(path, "r");

    if (file != NULL)
    {
        uint64 fileSize = GetFileSize(file);
        char* buffer = (char*)calloc(fileSize, 1);
        fread(buffer, 1, fileSize, file);
        fclose(file);
        return String::NoAllocString(buffer);
    }
    else
    {
        return new String();
    }
}

uint64
FileSystem::GetFileSize(FILE* file)
{
    fseek(file, 0, SEEK_END);
    uint64 length = (uint64)ftell(file);
    fclose(file);
    return length;
}

FILE*
FileSystem::OpenFile(const char* path, const char* mode)
{
    FILE* file = fopen(path, mode);
    if (file == NULL)
    {
        //TODO LOG ERROR
    }
    return file;
}