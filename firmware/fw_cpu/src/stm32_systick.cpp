#include "stm32_systick.h"
#include "ISRstm32f429xx.h"
#include "plc_io.h"
#include "stm32_rcc.h"

uint32_t STM32_SYSTICK::m_tick;

void STM32_SYSTICK::init()
{
    SysTick_Config(STM32_RCC::get_HCLK_freq() / 1000 - 1);
    m_tick = 0;
}

void STM32_SYSTICK::on_tick()
{
    ++m_tick;
    PLC_IO::timer_proc();
}

void ISR::SysTickTimer()
{
    STM32_SYSTICK::on_tick();
}
