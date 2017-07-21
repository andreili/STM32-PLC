#include "my_func.h"

int strlen(const char* str)
{
	int len = 0;
	while (*str++)
		++len;
	return len;
}

void memcpy(uint8_t* dst, const uint8_t* src, int len)
{
	while (len--)
		*dst++ = *src++;
}

void memset(uint8_t* dst,char val, int len)
{
	while (len--)
		*dst++ = val;
}

int memcmp(const uint8_t *s1, const uint8_t *s2, int len)
{
	int d = 0;

	while (len--)
	{
		d = (int)*s1++ - (int)*s2++;
		if (d)
			break;
	}

	return d;
}
