#ifndef STM32_SYSTICK_H
#define STM32_SYSTICK_H

#include "stm32_conf.h"

#define SYSTICK_CLKSOURCE_HCLK_DIV8    ((uint32_t)0x00000000U)
#define SYSTICK_CLKSOURCE_HCLK         ((uint32_t)0x00000004U)

class STM32_SYSTICK
{
public:
    static void init();
    static void update_freq();
    static void set_clock_source(uint32_t src);

    static inline uint32_t get_tick() { return m_tick; }

    static void on_tick();
    static void delay(__IO uint32_t delay_ms);

    static inline void suspend() { BIT_BAND_PER(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk) = DISABLE; }
    static inline void resume() { BIT_BAND_PER(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk) = ENABLE; }

    ENDIS_REG_FLAG(DBG_sleep_mode, DBGMCU->CR, DBGMCU_CR_DBG_SLEEP)
    ENDIS_REG_FLAG(DBG_stop_mode, DBGMCU->CR, DBGMCU_CR_DBG_SLEEP)
    ENDIS_REG_FLAG(DBG_standby_mode, DBGMCU->CR, DBGMCU_CR_DBG_SLEEP)
    #ifdef STM32F429xx
    ENDIS_REG_FLAG(compensation_cell, SYSCFG->CMPCR, SYSCFG_CMPCR_CMP_PD)
    ENDIS_REG_FLAG(memory_swapping_bank, SYSCFG->MEMRMP, SYSCFG_MEMRMP_UFB_MODE)
    #endif
private:
    static volatile uint32_t m_tick;
};

#endif
