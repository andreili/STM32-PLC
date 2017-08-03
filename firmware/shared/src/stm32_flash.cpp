#include "stm32_flash.h"

void STM32_FLASH::instruction_cache_reset()
{
    FLASH->ACR |= FLASH_ACR_ICRST;
    FLASH->ACR &= ~FLASH_ACR_ICRST;
}

void STM32_FLASH::data_cache_reset()
{
    FLASH->ACR |= FLASH_ACR_DCRST;
    FLASH->ACR &= ~FLASH_ACR_DCRST;
}
