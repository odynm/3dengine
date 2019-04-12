#ifndef STRING_H
#define STRING_H

/*
 Very simple string class
 Can be further improved with string builders and concat
*/

#include <stdlib.h>
#include <string.h>

// Basic string - char aray
typedef char* string;

// String with size tracking
typedef struct String {
    int length;
    char* text;

public:
    String() {}
    String(char const[]);
    String(char*);
    ~String();

    /// Create a string but not allocates a array - it uses the same
    static String* NoAllocString(char* c);

    operator char*() const { return text; }
} String;

#endif