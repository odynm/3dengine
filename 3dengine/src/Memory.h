#ifndef MEMORY_H
#define MEMORY_H

/*
 This class was created just to centralize all allocations,
 so it will be easy to make a custom allocator if needed
*/

#include <stdlib.h>

class Memory
{
public:
	static void* Alloc(int size);
	static void* AllocFixed(int size);
	static void Release(void* address);
private:
};

#endif