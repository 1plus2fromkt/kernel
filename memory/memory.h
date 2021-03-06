#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "kernel/kernel.h"

#define BITSET_SIZE (1 << 20)
#define LOW_MEMORY 0x1000000
#define PAGE_SIZE 4096
#define PAGE_DIRECTORY_NUMBER 1024
#define PAGE_TABLE_NUMBER 1024
#define KERNEL_PAGES PAGE_TABLE_NUMBER * (PAGE_DIRECTORY_NUMBER - KERNEL_START_INDEX)
#define USER_PAGES PAGE_TABLE_NUMBER * KERNEL_START_INDEX
#define USER_DIRECTORIES (KERNEL_START_INDEX)
#define TOTAL_AMOUNT_OF_PAGES PAGE_DIRECTORY_NUMBER * PAGE_TABLE_NUMBER
#define SYSTEM_BYTES 12
#define META_INF_OFFSET 5 
#define PAGE_BITS 12
#define PAGE_TABLE_BITS 10
#define PAGE_DIR_BITS 10
#define PAGE_DIR_OFFSET (PAGE_TABLE_BITS + PAGE_BITS)
#define PAGE_TABLE_OFFSET  PAGE_BITS

#define PT_PRESENT 1
#define PT_RW 2

void init_memory(uint32_t size);
void* load_phys_page();
void free_phys_page(void* pos);

#endif