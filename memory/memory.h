#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define BITSET_SIZE (1 << 20)

void init_memory(uint32_t begin, uint32_t size);
void* load_phys_page();
void free_phys_page(void* pos);

#endif