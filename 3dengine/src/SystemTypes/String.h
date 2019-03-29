/*
 Very simple string class
 Can be further improved with string builders and concat
*/

#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <string.h>

typedef struct String {
    int length;
    char* text;

public:
    String(char const[]);
    String(char*);
    ~String();
    operator char*() const { return text; }
} String;

#endif