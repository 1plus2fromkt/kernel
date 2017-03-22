#ifndef TTY_H
#define TTY_H

#include <stdint.h>

void terminal_writestring(const char* data);
void terminal_initialize(void);
void write_num(uint32_t t, char* str, uint32_t rad);

#endif