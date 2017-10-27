#ifndef STM32_FLASH_H
#define STM32_FLASH_H

#include "stm32_conf.h"

class STM32_FLASH
{
public:
    static inline void set_latency(uint8_t latency) { *((__IO uint8_t *)(FLASH_R_BASE + 0)) = latency; }
    static inline uint8_t get_latency() { return FLASH->ACR & FLASH_ACR_LATENCY; }

    ENDIS_REG_FLAG(prefetch_buffer, FLASH->ACR, FLASH_ACR_PRFTEN)
    ENDIS_REG_FLAG(instruction_cache, FLASH->ACR, FLASH_ACR_ICEN)
    ENDIS_REG_FLAG(data_cache, FLASH->ACR, FLASH_ACR_DCEN)

    static void instruction_cache_reset();
    static void data_cache_reset();
};

#endif
