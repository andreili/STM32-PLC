#ifndef STM32_CONF_H
#define STM32_CONF_H

#include <stdint.h>
#include "ISRstm32f429xx.h"
#include "stm32f4xx.h"
#include "bitbanding.h"

#define STM32_RESULT_OK         0
#define STM32_RESULT_FAIL       1
#define STM32_RESULT_TIMEOUT    2

#define INSTRUCTION_CACHE_ENABLE
#define DATA_CACHE_ENABLE
#define PREFETCH_ENABLE

#define FORCE_INLINE __attribute__((always_inline))

//#define STM32_USE_UART1
//#define STM32_USE_UART2
#define STM32_USE_UART3
#define STM32_BRATE_UART3 115200
//#define STM32_USE_UART4
//#define STM32_USE_UART5
//#define STM32_USE_UART6
//#define STM32_USE_UART7
//#define STM32_USE_UART8

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
#define STM32_PLLN 220
#define STM32_PLLP RCC_PLLP_DIV2
#define STM32_PLLQ 7
#define STM32_CLOCK_TYPE (RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2)
#define STM32_CLOCK_AHB_DIV RCC_CFGR_HPRE_DIV1
#define STM32_CLOCK_SYSCLK_SOURCE RCC_CFGR_SW_PLL
#define STM32_CLOCK_APB1_DIV RCC_CFGR_PPRE1_DIV4
#define STM32_CLOCK_APB2_DIV RCC_CFGR_PPRE2_DIV2

/* UART settings */
#define STM32_UART_STOPBITS UART_STOPBITS_1
#define STM32_UART_WORDLENGTH UART_WORDLENGTH_8B
#define STM32_UART_PARITY UART_PARITY_NONE
#define STM32_UART_MODE_IT_ENABLE
#define STM32_UART_MODE_DMA_ENABLE

/* Power settings */
#define STM32_PVD_MODE (PVD_MODE_IT | PVD_RISING_EDGE | PVD_FALLING_EDGE)

/* Priority groups */
#define STM32_PRIORITY_SYSCLK NVIC_PRIORITYGROUP_4

/* SDRAM settings */
#define STM32_SDRAM_SIZE_MB 16

#define STM32_SDRAM_BANK1
//#define STM32_SDRAM_BANK2
#define STM32_SDRAM_COLUMNS FMC_SDRAM_COLUMN_BITS_NUM_9
#define STM32_SDRAM_ROWS FMC_SDRAM_ROW_BITS_NUM_12
#define STM32_SDRAM_BUS_WIDTH FMC_SDRAM_MEM_BUS_WIDTH_16
#define STM32_SDRAM_BANKS FMC_SDRAM_INTERN_BANKS_NUM_4
#define STM32_SDRAM_CAS_LAT FMC_SDRAM_CAS_LATENCY_3
#define STM32_SDRAM_CAS_LAT_MR SDRAM_MODEREG_CAS_LATENCY_3
#define STM32_SDRAM_WP FMC_SDRAM_WRITE_PROTECTION_DISABLE
#define STM32_SDRAM_CLOCK FMC_SDRAM_CLOCK_PERIOD_2
#define STM32_SDRAM_BURST FMC_SDRAM_RBURST_ENABLE
#define STM32_SDRAM_PIPE_DELAY FMC_SDRAM_RPIPE_DELAY_0
#define STM32_SDRAM_LOAD_TO_ACTIVE_DELAY 2
#define STM32_SDRAM_EXIT_SELF_REFRESH_DELAY 7
#define STM32_SDRAM_SELF_REFRESH_TIME 4
#define STM32_SDRAM_ROW_CYCLE_DELAY 6
#define STM32_SDRAM_WRITE_RECOVERY_TIME 2
#define STM32_SDRAM_RP_DELAY 2
#define STM32_SDRAM_RCD_DELAY 2
#define STM32_SDRAM_REFRESH_RATE 0x077d   /* SDRAM refresh counter (90MHz SD clock) */
#define STM32_SDRAM_BURST_LENGTH SDRAM_MODEREG_BURST_LENGTH_2

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

void Error_Handler();

class STM32_SDRAM;
extern STM32_SDRAM sdram;

#endif
