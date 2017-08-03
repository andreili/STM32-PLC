#ifndef STM32_INC_H
#define STM32_INC_H

#include <stdint.h>
#include "stm32_conf.h"
#include "stm32_gpio.h"
#include "stm32_flash.h"
#include "stm32_nvic.h"
#include "stm32_pwr.h"
#include "stm32_rcc.h"
#include "stm32_systick.h"

#ifdef STM32F10X_MD
#include "stm32f1xx.h"

#include "ISRstm32f10x_md.h"
#endif

#ifdef STM32F429xx
#include "stm32f4xx.h"
#include "stm32_sd.h"
#include "stm32_sdram.h"

#include "stm32_uart.h"
#include "stm32_spi.h"

#include "ISRstm32f429xx.h"
#endif

#include "bitbanding.h"

#endif // STM32_INC_H
