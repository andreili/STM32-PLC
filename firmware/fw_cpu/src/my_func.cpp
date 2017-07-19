#include "my_func.h"

int strlen(const char* str)
{
	int len = 0;
	while (*str++)
		++len;
	return len;
}

void memcpy(char* dst, const char* src, int len)
{
	while (len--)
		*dst++ = *src++;
}

void memset(char* dst,char val, int len)
{
	while (len--)
		*dst++ = val;
}

int memcmp(const char *s1, const char *s2, int len)
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
