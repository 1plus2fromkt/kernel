#ifndef ITOA_H
#define ITOA_H
#include <stdint.h>

char* itoa(int32_t x, char* buff, uint8_t rad);
char* uitoa(uint32_t x, char* buff, uint8_t rad);

#endif