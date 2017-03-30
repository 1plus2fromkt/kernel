#include "gdt.h"

void init_gdt_single_entry(gdt_entry_t* gdt)
{
    gdt->limit = 0;
    gdt->base1 = 0;
    gdt->base2 = 0;
    gdt->access = 1 << 4;
    gdt->flags = 0;
    gdt-> base3 = 0;
}

void gdt_set_limit(gdt_entry_t* gdt, uint32_t lim)
{
    gdt->limit = lim & 0xffff;
    gdt->flags = (gdt->flags & 0xf0) | ((lim >> 16) & 0xf);
}

uint32_t gdt_get_limit(gdt_entry_t* gdt)
{
    return gdt->limit | ((gdt->flags & 0xf) << 16);
}

void gdt_set_base(gdt_entry_t* gdt, uint32_t b)
{
    gdt->base1 = b & 0xffff;
    gdt->base2 = (b >> 16) & 0xff;
    gdt->base3 = (b >> 24) & 0xff;
}

uint32_t gdt_get_base(gdt_entry_t* gdt)
{
    return gdt->base1 | (gdt->base2 << 16) | (gdt->base3 << 24);
}

void gdt_set_access(gdt_entry_t* gdt, uint8_t access)
{
    gdt->access = (gdt->access & (~((1 << 5) | (1 << 6)))) | (access << 5);
}

uint8_t gdt_get_access(gdt_entry_t* gdt)
{
    return (gdt->access & ((1 << 5) | (1 << 6))) >> 5;
}

void init_gdt_empty_segment(gdt_entry_t* gdt)
{
    init_gdt_single_entry(gdt);
    
    gdt_set_limit(gdt, 0);
    gdt_set_base(gdt, 0);
}

void init_gdt_kernel_code_segment(gdt_entry_t* gdt)
{
    init_gdt_single_entry(gdt);
    
    gdt_set_limit(gdt, 0xfffff);
    gdt_set_base(gdt, 0);
    
    gdt_set_access(gdt, 0);
    gdt->access |= (1 << 7) | (1 << 3) | (1 << 1);
    
    gdt->flags |= (1 << 6) | (1 << 7);
}

void init_gdt_kernel_data_segment(gdt_entry_t* gdt)
{
    init_gdt_single_entry(gdt);
    
    gdt_set_limit(gdt, 0xfffff);
    gdt_set_base(gdt, 0);
    
    gdt_set_access(gdt, 0);
    gdt->access |= (1 << 1) | (1 << 7);
    
    gdt->flags |= (1 << 6) | (1 << 7);
}

void init_gdt_user_code_segment(gdt_entry_t* gdt)
{
    init_gdt_single_entry(gdt);
    
    gdt_set_limit(gdt, 0xfffff);
    gdt_set_base(gdt, 0);
    
    gdt_set_access(gdt, 3);
    gdt->access |= (1 << 1) | (1 << 3) | (1 << 7);
    
    gdt->flags |= (1 << 6) | (1 << 7);
}

void init_gdt_user_data_segment(gdt_entry_t* gdt)
{
    init_gdt_single_entry(gdt);
    
    gdt_set_limit(gdt, 0xfffff);
    gdt_set_base(gdt, 0);
    
    gdt_set_access(gdt, 3);
    gdt->access |= (1 << 1) | (1 << 7);
    
    gdt->flags |= (1 << 6) | (1 << 7);
}

gdt_entry_t gdts[5];

void init_gdt()
{
    init_gdt_empty_segment(&gdts[0]);
    init_gdt_kernel_code_segment(&gdts[1]);
    init_gdt_kernel_data_segment(&gdts[2]);
    init_gdt_user_code_segment(&gdts[3]);
    init_gdt_user_data_segment(&gdts[4]);
}
