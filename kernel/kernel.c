#include <stddef.h>
#include "kernel.h"
#include "tty.h"
#include <stdint.h>
#include <stdbool.h>
#include "boot/multiboot.h"
#include "libc/itoa.h"
#include "memory/memory.h"
#include "paging.h"

char buff[32];

extern uint32_t pd[1024] __attribute__((aligned(4096)));
extern uint32_t pt[PAGE_NUMBER][1024] __attribute__((aligned(4096)));
 
void kernel_main(uint32_t magic, multiboot_info_t* info) {
	info = (multiboot_info_t*)((int)info + KERNEL_START);
	terminal_initialize();
	terminal_writestring("Hello world\n");
	init_memory(info->mem_upper); // I hope that this is what I want, amount of upper memory
	terminal_writestring(uitoa(pt[0][1023], buff, 16));
	terminal_writestring("\n");
	for (int i = 0; i < 10; i++) 
	{
		// terminal_writestring(uitoa(pd[i], buff, 16));
		// terminal_writestring("\n");
		// terminal_writestring(uitoa(pd[i + 768], buff, 16));
		// terminal_writestring("\n");
		// terminal_writestring(uitoa(pt[i], buff, 16));
		// terminal_writestring("\n");
		terminal_writestring(uitoa(pt[1][i], buff, 16));
		terminal_writestring("\n");
	}
	terminal_writestring("\n");
	terminal_writestring(itoa(info->flags, buff, 16));
	// terminal_writestring("\n");
	// terminal_writestring(itoa(magic, buff, 16));
}
