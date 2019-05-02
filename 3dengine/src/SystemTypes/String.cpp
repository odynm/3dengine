#include "String.h"
#include <stdio.h>

String*
STR_NewString(char* c)
{
	int length = 0;
	while (c[length]) { length++; }
	String* s = (String*)MEM_Alloc(length);
	s->text = (char*)MEM_Alloc(length);
	s->length = length;
	s->text[length] = '\0';
	memcpy(s->text, c, length);
	return s;
}

String*
STR_NewStringA(char const c[])
{
	int length = 0;
	while (c[length]) { length++; }
	String* s = (String*)MEM_Alloc(length);
	s->text = (char*)MEM_Alloc(length);
	s->length = length;
	s->text[length] = '\0';
	memcpy(s->text, c, length);
	return s;
}

void
STR_Destroy(void* address)
{
	MEM_Release(address);
}

String*
STR_NoAllocString(char* c)
{
	String* s = (String*)MEM_Alloc(sizeof(String));
	s->length = 0;
	while (c[s->length]) { s->length++; }
	s->text = c;
	return s;
}