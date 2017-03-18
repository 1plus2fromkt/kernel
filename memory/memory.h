#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define BITSET_SIZE (1 << 20)
#define LOW_MEMORY 0x1000000
#define PAGE_SIZE 4096

void init_memory(uint32_t size);
void* load_phys_page();
void free_phys_page(void* pos);

#endif