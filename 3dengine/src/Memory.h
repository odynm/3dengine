#ifndef MEMORY_H
#define MEMORY_H

/*
 This class was created just to centralize all allocations,
 so it will be easy to make a custom allocator if needed
*/

#include <string.h> //(memset)
#include <stdlib.h>

#include "SystemTypes/Typedefs.h"

void* MEM_Alloc(uint64 size);
void* MEM_AllocFixed(uint64 size);
void MEM_Release(void* address);
void* MEM_Set(void* address, int value, uint size);

#endif