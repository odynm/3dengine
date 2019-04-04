#include "String.h"
#include <stdio.h>

String::String(char* c)
{
    length = 0;
    while (c[length]) { length++; }
    text = new char[length];
    text[length] = '\0';
    text = c;
}

String::String(char const c[])
{
    length = 0;
    while (c[length]) { length++; }
    text = new char[length + 1];
    text[length] = '\0';
    memcpy(text, c, length);
}

String::~String()
{
    free(text);
}