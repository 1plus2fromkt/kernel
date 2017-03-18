#include "libc/itoa.h"
#include "libc/reverse.h"
#include <stdbool.h>

char digits[16] = "0123456789abcdef";
char* itoa(int32_t x, char* buff, uint8_t rad)
{
	if (x < 0 && rad == 10)
		x *= -1;
	return uitoa(x, buff, rad); // yeah, it prints always only positive. 
	// if (neg)
	// 	buff[i++] = '-';
	// reverse(buff, i);
	// buff[i] = '\0';
	// return buff;
}

char* uitoa(uint32_t x, char* buff, uint8_t rad) 
{
	if (!x)
	{
		buff = "0\0";
		return buff;
	}
	uint32_t i = 0;
	while (x)
	{
		int r = x % rad;
		buff[i++] = digits[r];
		x /= rad;
	}
	reverse(buff, i);
	buff[i] = '\0';
	return buff;
}