#include "kernel/kernel.h"
#include "memory/memory.h"

uint32_t page_num;
int32_t free_mem[BITSET_SIZE]; // points to the next free page index
int32_t curr;
uint32_t early_part; // in bytes, or smth like that
extern char end_of_kernel;

void init_memory(uint32_t begin, uint32_t size)
{
	early_part = begin;
	page_num = size / PAGE_SIZE;
	int phys_kern = (end_of_kernel - KERNEL_START) / PAGE_SIZE;
	for (uint32_t i = 0; i < BITSET_SIZE; i++)
		free_mem[i] = -1;
	for (uint32_t i = phys_kern / PAGE_SIZE + 1; i < page_num; i++)
		free_mem[i] = i + 1;
	curr = phys_kern + 1;
}

void* load_phys_page() 
{
	if (curr == -1) 
		return (void*)-1; // PANIC HERE
	uint32_t next = free_mem[curr], ret = PAGE_SIZE * curr + early_part;
	free_mem[curr] = -1;
	curr = next;
	return (void*) ret;
}

void free_phys_page(void* pos)
{
	uint32_t t = (int) pos;
	uint32_t ind = (t - early_part) / PAGE_SIZE;
	if (free_mem[ind] != -1)
		return; //ALSO PANIC
	free_mem[ind] = curr;
	curr = ind;
}