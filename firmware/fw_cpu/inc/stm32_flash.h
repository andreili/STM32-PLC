#ifndef STM32_FLASH_H
#define STM32_FLASH_H

#include "stm32f4xx.h"
#include "bitbanding.h"

/**
  * @brief   ACR register byte 0 (Bits[7:0]) base address
  */
#define ACR_BYTE0_ADDRESS           ((uint32_t)0x40023C00U)
/**
  * @brief   OPTCR register byte 0 (Bits[7:0]) base address
  */
#define OPTCR_BYTE0_ADDRESS         ((uint32_t)0x40023C14U)
/**
  * @brief   OPTCR register byte 1 (Bits[15:8]) base address
  */
#define OPTCR_BYTE1_ADDRESS         ((uint32_t)0x40023C15U)
/**
  * @brief   OPTCR register byte 2 (Bits[23:16]) base address
  */
#define OPTCR_BYTE2_ADDRESS         ((uint32_t)0x40023C16U)
/**
  * @brief   OPTCR register byte 3 (Bits[31:24]) base address
  */
#define OPTCR_BYTE3_ADDRESS         ((uint32_t)0x40023C17U)

#define FLASH_ENDIS(name, bits) \
    static inline void enable_ ## name () { BIT_BAND_PER(FLASH->ACR, FLASH_ACR_ ## bits ## EN) = 1; } \
    static inline void disable_ ## name () { BIT_BAND_PER(FLASH->ACR, FLASH_ACR_ ## bits ## EN) = 0; }

class STM32_FLASH
{
public:
    static inline void set_latency(uint8_t latency) { *(__IO uint8_t *)ACR_BYTE0_ADDRESS = latency; }
    static inline uint8_t get_latency() { return FLASH->ACR & FLASH_ACR_LATENCY; }

    FLASH_ENDIS(prefetch_buffer, PRFT)
    FLASH_ENDIS(instruction_cache, IC)
    FLASH_ENDIS(data_cache, DC)

    static void instruction_cache_reset();
    static void data_cache_reset();
};

#endif
