#include "libc/itoa.h"
#include "libc/reverse.h"
#include <stdbool.h>

char digits[16] = "0123456789abcdef";
char* itoa(int32_t x, char* buff, uint8_t rad)
{
	uint32_t i = 0;
	bool neg = false;
	if (!x)
	{
		buff = "0\0";
		return buff;
	}
	if (x < 0 && rad == 10)
	{
		neg = true;
		x *= -1;
	}
	while (x)
	{
		int r = x % rad;
		buff[i++] = digits[r];
		x /= rad;
	}
	if (neg)
		buff[i++] = '-';
	reverse(buff, i);
	buff[i] = '\0';
	return buff;
}
