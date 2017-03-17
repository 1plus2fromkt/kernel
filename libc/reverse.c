#include "libc/reverse.h"

char* reverse(char* buff, uint32_t sz) 
{
	for (uint32_t i = 0, j = sz - 1; i < sz / 2; i++, j--)
	{
		char t = buff[i];
		buff[i] = buff[j];
		buff[j] = t;
	}
	return buff;
}