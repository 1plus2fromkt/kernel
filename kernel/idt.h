#ifndef IDT_H
#define IDT_H

#define IDT_NUMBER 256

#define PIC_OFFSET 0x20
#define KBD_IRQ 0x1

void init_interrupts();

struct idt_entry {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
} __attribute__ ((packed));

void enable_interrupts();

void disable_interrupts();

#endif