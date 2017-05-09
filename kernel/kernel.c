#include <stddef.h>
#include "kernel.h"
#include "tty.h"
#include "aout/loader.h"
#include <stdint.h>
#include <stdbool.h>
#include "boot/multiboot.h"
#include "memory/v_allocator.h"
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
	__asm volatile("int $128");
	
	// int* x = kmalloc(20000, &kernel_allocator);
	// for (int i = 0; i < 10000000; i++)
	// 	x[i] = i;
	// for (int i = 0; i < 10000000; i++)
	// {
	// 	if (x[i] != i)
	// 	{
	// 		write_num(i, "i", 10);
	// 		write_num(x[i], "x[i]", 10);
	// 		break;
	// 	}
	// }  WORKS
	terminal_writestring("starting execution\n");
	// exec(0);
	terminal_writestring("execution finished\n");
	// jump_usermode();
	__asm volatile("int $128");
	while (1)
    {
      __asm volatile("hlt");
    }
}
