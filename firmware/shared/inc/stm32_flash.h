#ifndef STM32_FLASH_H
#define STM32_FLASH_H

#include "stm32_conf.h"

#define FLASH_LATENCY_0            ((uint32_t)0x00000000)    /*!< FLASH Zero Latency cycle */
#define FLASH_LATENCY_1            FLASH_ACR_LATENCY_0       /*!< FLASH One Latency cycle */
#define FLASH_LATENCY_2            FLASH_ACR_LATENCY_1       /*!< FLASH Two Latency cycles */

class STM32_FLASH
{
public:
    static inline void set_latency(uint8_t latency) { *((__IO uint8_t *)(FLASH_R_BASE + 0)) = latency; }
    static inline uint8_t get_latency() { return FLASH->ACR & FLASH_ACR_LATENCY; }

    #ifdef STM32F10X_MD
    ENDIS_REG_FLAG(prefetch_buffer, FLASH->ACR, FLASH_ACR_PRFTBE)
    #endif
    #ifdef STM32F429xx
    ENDIS_REG_FLAG(prefetch_buffer, FLASH->ACR, FLASH_ACR_PRFTEN)
    ENDIS_REG_FLAG(instruction_cache, FLASH->ACR, FLASH_ACR_ICEN)
    ENDIS_REG_FLAG(data_cache, FLASH->ACR, FLASH_ACR_DCEN)

    static void instruction_cache_reset();
    static void data_cache_reset();
    #endif
};

#endif
