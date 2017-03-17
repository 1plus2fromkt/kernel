#include "kernel/kernel.h"
#include "memory/memory.h"

int page_num;
size_t free_mem[BITSET_SIZE];
size_t curr;
extern char end_of_kernel;

void init_memory(uint32_t size)
{
	page_num = size / PAGE_SIZE;
	int phys_kern = (end_of_kernel - KERNEL_START) / PAGE_SIZE;
	for (size_t i = 0; i < phys_kern; i++)
	{
		free_mem[i] = 1;
	}
	curr = phys_kern;
	
}