#include "kernel/kernel.h"
#include "memory/memory.h"
#include "libc/itoa.h"
#include "kernel/tty.h"

uint32_t page_num;
int32_t free_mem[BITSET_SIZE]; // points to the next free page index
int32_t curr;
extern char end_of_kernel;
char buff[32];

void init_memory(uint32_t size)
{
	page_num = size * 0x400 / PAGE_SIZE; // size -> bytes -> num_of_pages
	int phys_kern = (end_of_kernel - KERNEL_START) / PAGE_SIZE;
	for (uint32_t i = 0; i < BITSET_SIZE; i++) {
		free_mem[i] = -1;
	}
	uint32_t start = phys_kern / PAGE_SIZE + 1;
	for (uint32_t i = start; i < page_num + start; i++)
		free_mem[i] = i + 1;
	curr = start;
}

void* load_phys_page() 
{
	if (curr == -1) 
		return (void*)-1; // PANIC HERE
	uint32_t next = free_mem[curr], ret = PAGE_SIZE * curr + LOW_MEMORY;
	free_mem[curr] = -1;
	curr = next;
	return (void*) ret;
}

void free_phys_page(void* pos)
{
	uint32_t t = (int) pos;
	uint32_t ind = (t - LOW_MEMORY) / PAGE_SIZE;
	if (free_mem[ind] != -1)
		return; //ALSO PANIC
	free_mem[ind] = curr;
	curr = ind;
}