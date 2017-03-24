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

char buff[32];


 
void kernel_main(uint32_t magic, multiboot_info_t* info) {

	info = (multiboot_info_t*)((int)info + KERNEL_START);
	terminal_initialize();
	// terminal_writestring("Hello world\n");
	init_memory(info->mem_upper); // I hope that this is what I want, amount of upper memory
	// terminal_writestring("Memory initialized\n");
	init_mem_manager();
	// terminal_writestring("Memory manager initialized\n");
	uint32_t* a = (uint32_t*)kmalloc(1, &kernel_allocator);
	uint32_t* b = (uint32_t*)kmalloc(1, &kernel_allocator);
	uint32_t* c = (uint32_t*)kmalloc(1, &kernel_allocator);
	*a = 5;
	*b = 6;
	*c = 7;
	// write_num(*a, "a", 10);
	// write_num(*b, "b", 10);
	// write_num(*c, "c", 10);
	kfree(b, 1, &kernel_allocator);
	kfree(c, 1, &kernel_allocator);
	kfree(a, 1, &kernel_allocator);
	// terminal_writestring("\n");
	// terminal_writestring(itoa(info->flags, buff, 16));
	// terminal_writestring("\n");
	// terminal_writestring(itoa(magic, buff, 16));
}
