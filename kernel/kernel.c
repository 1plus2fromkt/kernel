#include <stddef.h>
#include "kernel.h"
#include "tty.h"
#include <stdint.h>
#include <stdbool.h>
#include "boot/multiboot.h"
#include "libc/itoa.h"
#include "memory/memory.h"


char buff[32];
 
void kernel_main(uint32_t magic, multiboot_info_t* info) {
	info = (multiboot_info_t*)((int)info + KERNEL_START);
	terminal_initialize();
	terminal_writestring("Hello world");
	init_memory(info->mem_upper); // I hope that this is what I want, amount of upper memory
	// terminal_writestring("\n");
	terminal_writestring(itoa(info->mem_upper, buff, 16));
	// terminal_writestring("\n");
	// terminal_writestring(itoa(magic, buff, 16));
}
