#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "tty.h"
#include "paging.h"
#include "libc/itoa.h"
#include "kernel.h"

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const size_t VIDEO_ADDRESS = 0xB8000 + KERNEL_START;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

size_t get_index(size_t x, size_t y) {
	return y * VGA_WIDTH + x;
}
 
void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) VIDEO_ADDRESS;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = get_index(x, y);
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_putchar(char c) {
	if (c != '\n')
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH || c == '\n') {
		terminal_column = 0;
		terminal_row++;
		if (terminal_row == VGA_HEIGHT) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				for (size_t y = 1; y < VGA_WIDTH; y++) {
					const size_t index1 = get_index(x, y), index2 = get_index(x, y - 1);
					terminal_buffer[index2] = terminal_buffer[index1];
				}
			}
			terminal_row--;
		}
	}
}
 
void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void write_num(uint32_t t, char* str, uint32_t rad)
{
	char buff[16];
	terminal_writestring(str);
	terminal_writestring(": ");
	terminal_writestring(uitoa(t, buff, rad));
	terminal_writestring("\n");
}