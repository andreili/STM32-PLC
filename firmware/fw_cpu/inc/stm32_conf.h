#ifndef STM32_CONF_H
#define STM32_CONF_H

#include <stdint.h>
#include "ISRstm32f429xx.h"
#include "stm32f4xx.h"
#include "bitbanding.h"

#define STM32_RESULT_OK         0
#define STM32_RESULT_FAIL       1
#define STM32_RESULT_TIMEOUT    2

/* Clock settings */
#define STM32_USE_HSE
#define STM32_HSE_STATE RCC_CR_HSEON
//#define STM32_USE_HSI
//#define STM32_HSI_STATE RCC_CR_HSION
//#define STM32_HSI_CALIBRATION 0
//#define STM32_USE_LSI
//#define STM32_LSI_STATE RCC_CSR_LSION
//#define STM32_USE_LSE
#define STM32_LSE_STATE RCC_BDCR_LSEOFF
#define STM32_USE_PLL
#define STM32_PLL_STATE RCC_PLL_ON
#define STM32_PLL_SOURCE RCC_PLLSOURCE_HSE
#define STM32_PLLM 4
#define STM32_PLLN 168
#define STM32_PLLP RCC_PLLP_DIV2
#define STM32_PLLQ 7
#define STM32_CLOCK_TYPE (RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2)
#define STM32_CLOCK_AHB_DIV RCC_CFGR_HPRE_DIV1
#define STM32_CLOCK_SYSCLK_SOURCE RCC_CFGR_SW_PLL
#define STM32_CLOCK_APB1_DIV RCC_CFGR_PPRE1_DIV4
#define STM32_CLOCK_APB2_DIV RCC_CFGR_PPRE2_DIV2

/* Power settings */
#define STM32_PVD_MODE (PVD_MODE_IT | PVD_RISING_EDGE | PVD_FALLING_EDGE)

/* Priority groups */
#define STM32_PRIORITY_SYSCLK NVIC_PRIORITYGROUP_4

#define ENDIS_REG_FLAG(name, reg, mask) \
    static inline void enable_ ## name() { BIT_BAND_PER(reg, mask) = ENABLE; } \
    static inline void disable_ ## name() { BIT_BAND_PER(reg, mask) = DISABLE; }

#define CLK_ENDIS(enr, name) \
    ENDIS_REG_FLAG(clk_ ## name, RCC->enr ## ENR, RCC_ ## enr ## ENR_ ## name ## EN) \
    static inline bool check_enable_clk_ ## name() { return BIT_BAND_PER(RCC->enr ## ENR, RCC_ ## enr ## ENR_ ## name ## EN); }

#define PER_RESET_SLEEP(enr, name) \
    static inline void force_reset_ ## name() { BIT_BAND_PER(RCC-> enr ## RSTR, RCC_ ## enr ## RSTR_ ## name ## RST) = ENABLE; } \
    static inline void release_reset_ ## name() { BIT_BAND_PER(RCC-> enr ## RSTR, RCC_ ## enr ## RSTR_ ## name ## RST) = DISABLE; } \
    static inline void sleep_enable_ ## name() { BIT_BAND_PER(RCC-> enr ## LPENR, RCC_ ## enr ## LPENR_ ## name ## LPEN) = ENABLE; }

#define WAIT_TIMEOUT(condition, timeout) \
    { \
        uint32_t tickstart = STM32_SYSTICK::get_tick(); \
        while (condition) \
        { \
            if ((STM32_SYSTICK::get_tick() - tickstart) > timeout) \
                return STM32_RESULT_TIMEOUT; \
        } \
    }

#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007U) /*!< 0 bits for pre-emption priority
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006U) /*!< 1 bits for pre-emption priority
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005U) /*!< 2 bits for pre-emption priority
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004U) /*!< 3 bits for pre-emption priority
                                                                 1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003U) /*!< 4 bits for pre-emption priority
                                                                 0 bits for subpriority */

void Error_Handler();

#endif
