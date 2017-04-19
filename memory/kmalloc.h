#ifndef KMALLOC_H
#define KMALLOC_H
#include "v_allocator.h"
#include "kernel/user.h"

extern struct v_allocator kernel_allocator, *user_allocator;
extern struct v_allocator process_allocators[NUMBER_OF_PROCESSES];
extern struct mem_tree process_trees[NUMBER_OF_PROCESSES];

void init_mem_manager();
void kfree(void* address, uint32_t size, struct v_allocator* a);
void* kmalloc(uint32_t size, struct v_allocator* a);
void free_pages(struct v_allocator* a);

#endif