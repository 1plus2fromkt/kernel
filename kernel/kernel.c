#include <stddef.h>
#include "kernel.h"
#include "tty.h"
#include <stdint.h>
#include <stdbool.h>
#include "boot/multiboot.h"
#include "libc/itoa.h"
#include "memory/kmalloc.h"
#include "memory/memory.h"
#include "paging.h"
#include "gdt.h"
#include "idt.h"

char buff[32];

void kernel_main(uint32_t magic, multiboot_info_t* info)
{
	disable_interrupts();
	info = (multiboot_info_t*)((int)info + KERNEL_START);
	terminal_initialize();
	terminal_writestring("Hello world\n");
	init_memory(info->mem_upper); 
	terminal_writestring("Memory initialized\n");
	init_mem_manager();
	terminal_writestring("Memory manager initialized\n");
	init_gdt();
    terminal_writestring("GDT initialized\n");
	init_interrupts();
	terminal_writestring("IDT initialized\n");
	enable_interrupts();
	__asm volatile("int $116");
	// tempor();
	// terminal_writestring("\n");
	// terminal_writestring(itoa(info->flags, buff, 16));
	// terminal_writestring("\n");
	// terminal_writestring(itoa(magic, buff, 16));
}
