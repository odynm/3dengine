#include "Memory.h"

void* Memory::Alloc(int size)
{
	return calloc(size, 1);
}

void* Memory::AllocFixed(int size)
{
	return calloc(size, 1);
}

void Memory::Release(void* address)
{
	free(address);
}