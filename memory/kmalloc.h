#ifndef KMALLOC_H
#define KMALLOC_H
#include "v_allocator.h"

extern struct v_allocator user, kernel;

void init_mem_manager();
void kfree(void* address, uint32_t size, struct v_allocator* a);
void* kmalloc(uint32_t size, struct v_allocator* a);

#endif