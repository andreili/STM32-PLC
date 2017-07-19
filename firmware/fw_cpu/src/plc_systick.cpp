#include "inc/plc_systick.h"
#include "ISRstm32f429xx.h"
#include "plc_io.h"
#include "stm32_rcc.h"

uint32_t PLC_SYSTICK::m_tick;

void PLC_SYSTICK::init()
{
    SysTick_Config(STM32_RCC::get_HCLK_freq() / 1000 - 1);
    m_tick = 0;
}

void PLC_SYSTICK::on_tick()
{
    ++m_tick;
    PLC_IO::timer_proc();
}

void ISR::SysTickTimer()
{
    PLC_SYSTICK::on_tick();
}
