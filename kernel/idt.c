#include <stdint.h>
#include "idt.h"

struct idt_entry idt[IDT_NUMBER];

void init_idtr(struct idt_entry* addr, uint32_t size)
{
	struct
	{
		uint16_t limit;
		uint32_t base;
	} __attribute__ ((packed)) idtr;
	idtr.limit = size;
	idtr.base = (uint32_t) addr;
	__asm volatile("lidt (%0)" :: "a"(&idtr));
}

