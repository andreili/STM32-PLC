#ifndef __MY_FUNC__
#define __MY_FUNC__

#include <stdint.h>

int strlen(const char* str);
void memcpy(uint8_t* dst, const uint8_t* src, int len);
void memset(uint8_t* dst,char val, int len);
int memcmp(const uint8_t *s1, const uint8_t *s2, int len);

#endif //__MY_FUNC__
