#ifndef __MY_FUNC__
#define __MY_FUNC__

#include <stdint.h>

int strlen(const char* str);
void memcpy(uint8_t* dst, const uint8_t* src, uint32_t len);
void memset(uint8_t* dst,char val, uint32_t len);
int memcmp(const uint8_t *s1, const uint8_t *s2, uint32_t len);

#endif //__MY_FUNC__
