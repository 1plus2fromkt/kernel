#include "tty.h"
#include "pic.h"
#include "idt.h"
#include "io.h"

void kbd_interrupt_impl()
{
    int x = {inb(0x60)};
    write_num(x, "kbd code", 16);
    pic_send_eoi(KBD_IRQ);
}

void syscall_impl()
{
	terminal_writestring("not implemented yet\n");
}