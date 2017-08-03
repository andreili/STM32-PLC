#include "my_func.h"

#define BIG_BLOCK_SIZE (sizeof(uint32_t) << 2)
#define LITTLE_BLOCK_SIZE (sizeof(uint32_t))
#define TOO_SMALL1(LEN) ((LEN) < LITTLE_BLOCK_SIZE)
#define TOO_SMALL4(LEN) ((LEN) < BIG_BLOCK_SIZE)

#define UNALLIGNED1(X) ((uint32_t)X & (LITTLE_BLOCK_SIZE - 1))
#define UNALLIGNED2(X, Y) \
    (((uint32_t)X & (sizeof(uint32_t) - 1)) | ((uint32_t)Y & (sizeof(uint32_t) - 1)))

int strlen(const char* str)
{
	int len = 0;
	while (*str++)
		++len;
	return len;
}




void memcpy(uint8_t* dst, const uint8_t* src, uint32_t len)
{
    if (TOO_SMALL4(len) && (!UNALLIGNED2(src, dst)))
    {
        uint32_t *al_dst = (uint32_t*)dst;
        uint32_t *al_src = (uint32_t*)src;
        while (len >= BIG_BLOCK_SIZE)
        {
            *al_dst++ = *al_src++;
            *al_dst++ = *al_src++;
            *al_dst++ = *al_src++;
            *al_dst++ = *al_src++;
            len -= BIG_BLOCK_SIZE;
        }
        while (len >= LITTLE_BLOCK_SIZE)
        {
            *al_dst++ = *al_src++;
            len -= LITTLE_BLOCK_SIZE;
        }
        dst = (uint8_t*)al_dst;
        src = (uint8_t*)al_src;
    }
	while (len--)
		*dst++ = *src++;
}

void memset(uint8_t* dst,char val, uint32_t len)
{
    while (UNALLIGNED1(val))
    {
        if (len--)
            *dst++ = val;
        else
            return;
    }

    if (!TOO_SMALL1(len))
    {
        uint32_t *al_dst = (uint32_t*)dst;
        uint32_t buf = (val << 8) | val;
        buf |= (buf << 16);
        for (uint32_t i=32 ; i< LITTLE_BLOCK_SIZE*8 ; i<<=1)
            buf = (buf << i) | buf;
        while (len >= BIG_BLOCK_SIZE)
        {
            *al_dst++ = buf;
            *al_dst++ = buf;
            *al_dst++ = buf;
            *al_dst++ = buf;
            len -= BIG_BLOCK_SIZE;
        }
        while (len >= LITTLE_BLOCK_SIZE)
        {
            *al_dst++ = buf;
            len -= LITTLE_BLOCK_SIZE;
        }
        dst = (uint8_t*)al_dst;
    }

	while (len--)
		*dst++ = val;
}

int memcmp(const uint8_t *s1, const uint8_t *s2, uint32_t len)
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
