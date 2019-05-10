#ifndef STRING_H
#define STRING_H

/*
 Very simple string class
 Can be further improved with string builders and concat
*/

#include <stdlib.h>
#include <string.h>

#include "../Memory.h"

// String with size tracking
typedef struct sString {
	int length;
	char* text;
} String;

//String(char const[]);
String* STR_NewString(char*);
String* STR_NewStringA(const char[]);

/// Create a string but not allocates a array - it uses the same
/// Despite the name, an allocation is indeed made for the string struct
String* STR_NoAllocString(char* c);
void STR_Destroy(void* address);

#endif