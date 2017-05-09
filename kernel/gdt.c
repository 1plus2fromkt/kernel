#include "gdt.h"
#include "tss.h"

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

tss_entry_t tss_entry;

void init_gdt_tss(gdt_entry_t *gdt) {
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = sizeof(tss_entry);

    gdt_set_base(gdt, base);
    gdt_set_limit(gdt, limit);
    gdt_set_access(gdt, 3); // 3, right?
    gdt->access |= 1 | (1 << 7);
    // g->accessed=1; //This indicates it's a TSS and not a LDT. This is a changed meaning
    // g->read_write=0; //This indicates if the TSS is busy or not. 0 for not busy
    // g->conforming_expand_down=0; //always 0 for TSS
    // g->code=1; //For TSS this is 1 for 32bit usage, or 0 for 16bit. !!!!!!!!!???????
    // g->always_1=0; //indicate it is a TSS
    // g->DPL=3; //same meaning
    // g->present=1; //same meaning
    // g->available=0;
    // g->always_0=0; //same thing
    // g->big=0; //should leave zero according to manuals. No effect
    // g->gran=0; //so that our computed GDT limit is in bytes, not pages

    for (uint32_t i = 0; i < sizeof(tss_entry); i++) {
        ((uint32_t*) &tss_entry)[i] = 0;
    }

    tss_entry.ss0  = 0x10;  // Set the kernel stack segment.
    register int stack_p asm("esp"); //???
    tss_entry.esp0 = stack_p; // Set the kernel stack pointer.
}

// need that for interrupts?
void set_kernel_stack(uint32_t stack) {
   tss_entry.esp0 = stack;
}

void flush_tss() {
    __asm volatile(
        "movw 0x2b, %ax\n\t"
        "ltr %ax\n\t"
    );
}

gdt_entry_t gdts[6];
// null
// kernel code
// kernel data
// user code
// user data
// tss

__attribute__ ((noinline)) static void reload_segments()
{
  __asm volatile(
    "ljmp $0x08, $reload_cs\n\t"
    "reload_cs:\n\t"
    "movw $0x10, %%ax\n\t"
    "movw %%ax, %%ds\n\t"
    "movw %%ax, %%es\n\t"
    "movw %%ax, %%fs\n\t"
    "movw %%ax, %%gs\n\t"
    "movw %%ax, %%ss\n\t"
    :
    :
    : "ax");
}

void set_gdtr(gdt_entry_t* addr, uint16_t size)
{
  struct {
    uint16_t size;
    uint32_t addr;
  } __attribute__((packed)) gdtr = {.size = size - 1, .addr = (uint32_t)addr};

  __asm volatile("lgdt (%0)": : "a"(&gdtr));
  reload_segments();
}

void init_gdt()
{
    init_gdt_empty_segment(&gdts[0]);
    init_gdt_kernel_code_segment(&gdts[1]);
    init_gdt_kernel_data_segment(&gdts[2]);
    init_gdt_user_code_segment(&gdts[3]);
    init_gdt_user_data_segment(&gdts[4]);
    init_gdt_tss(&gdts[5]);
    set_gdtr(gdts, sizeof(gdts));
    flush_tss();
}

void test() {
    while (1) { }
}