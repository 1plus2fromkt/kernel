#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define KERNEL_CODE_SEGMENT 1
#define KERNEL_DATA_SEGMENT 2

typedef struct gdt_entry_t
{
    uint16_t limit;
    uint16_t base1;
    uint8_t base2;
    uint8_t access;
    uint8_t flags;
    uint8_t base3;
} gdt_entry_t;

void init_gdt_single_entry(gdt_entry_t* gdt);

void gdt_set_limit(gdt_entry_t* gdt, uint32_t lim);

uint32_t gdt_get_limit(gdt_entry_t* gdt);

void gdt_set_base(gdt_entry_t* gdt, uint32_t b);

uint32_t gdt_get_base(gdt_entry_t* gdt);

void gdt_set_access(gdt_entry_t* gdt, uint8_t access);

uint8_t gdt_get_access(gdt_entry_t* gdt);

void init_gdt_empty_segment(gdt_entry_t* gdt);

void init_gdt_kernel_code_segment(gdt_entry_t* gdt);

void init_gdt_kernel_data_segment(gdt_entry_t* gdt);

void init_gdt_user_code_segment(gdt_entry_t* gdt);

void init_gdt_user_data_segment(gdt_entry_t* gdt);

void init_gdt();

#endif
