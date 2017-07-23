#include "stm32_systick.h"
#include "plc_io.h"
#include "stm32_rcc.h"
#include "stm32_nvic.h"

volatile uint32_t STM32_SYSTICK::m_tick;

void STM32_SYSTICK::init()
{
    SysTick_Config(STM32_RCC::get_HCLK_freq() / 1000 - 1);
    STM32_NVIC::set_priority(SysTick_IRQn,
                     STM32_NVIC::encode_priority(STM32_NVIC::get_priority_grouping(),
                                                 STM32_PRIORITY_SYSCLK, 0));
    m_tick = 0;
}

void STM32_SYSTICK::on_tick()
{
    ++m_tick;
    PLC_IO::timer_proc();
}

void STM32_SYSTICK::delay(__IO uint32_t delay_ms)
{
    uint32_t tick_end = m_tick + delay_ms;
    while(m_tick < tick_end) {}
}

void ISR::SysTickTimer()
{
    STM32_SYSTICK::on_tick();
}
