#include "Memory.h"

void*
MEM_Alloc(uint64 size)
{
    return calloc(size, 1);
}

void*
MEM_AllocFixed(uint64 size)
{
    return calloc(size, 1);
}

void
MEM_Release(void* address)
{
    free(address);
}

void*
MEM_Set(void* address, int value, uint size)
{
	return memset(address, value, size);
}