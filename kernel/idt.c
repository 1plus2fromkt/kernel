#include <stdint.h>
#include "idt.h"
#include "gdt.h"
#include "handlers.h"
#include "tty.h"
#include "pic.h"

#define PRESENT_BIT 0x80
#define INTERRUPT_GATE 0xe

void kbd_wrapper();
void tempor();

struct idt_entry idt[IDT_NUMBER];

void add_entry(void* function, uint32_t num)
{
	struct idt_entry* en = &idt[num];
	en->offset_1 = ((uint32_t)function) & 0xffff;
	en->offset_2 = (((uint32_t)function) >> 16) & 0xffff; // don't know why & 0xffff, maybe not needed
	write_num((uint32_t) function, "function", 16);
	write_num(en->offset_1, "first", 16);
	write_num(en->offset_2, "second", 16);
	en->zero = 0;
	en->selector = KERNEL_CODE_SEGMENT << 3; // according to http://wiki.osdev.org/Selector
	write_num(en->selector, "selector", 16);
	en->type_attr = PRESENT_BIT | INTERRUPT_GATE;
	write_num(en->type_attr, "type_attr", 16);
}

void init_idtr(struct idt_entry* addr, uint32_t size)
{
	struct
	{
		uint16_t limit;
		uint32_t base;
	} __attribute__ ((packed)) idtr;
	idtr.limit = size - 1;
	idtr.base = (uint32_t) addr;
	__asm volatile("lidt (%0)" :: "a"(&idtr));
}

void init_interrupts()
{
    pic_remap(PIC1, PIC2);
    write_num((uint32_t)&tempor, "tempor", 16);
	add_entry((void*)&kbd_wrapper, PIC_OFFSET + KBD_IRQ);
	add_entry((void*)&tempor, 116);
	init_idtr(idt, sizeof(idt));
}

void enable_interrupts()
{
  __asm volatile("sti");
}

void disable_interrupts()
{
  __asm volatile("cli");
}