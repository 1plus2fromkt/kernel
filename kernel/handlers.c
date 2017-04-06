#include "tty.h"
#include "pic.h"
#include "idt.h"

void kbd_interrupt_impl()
{
    terminal_writestring("h\n");
    pic_send_eoi(KBD_IRQ);
}

void tempor_impl()
{
	terminal_writestring("h\n");
}