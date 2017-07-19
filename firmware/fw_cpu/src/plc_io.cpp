#include "plc_io.h"
#include "ISRstm32f429xx.h"
#include "stm32_rcc.h"
#include "stm32_systick.h"

void PLC_IO::init()
{
    STM32_RCC::clk_enable_GPIOB();
    gpiob.set_config(PIN_PWR | PIN_RUN | PIN_STP | PIN_FLT,
                     GPIO_MODE_OUTPUT_PP, 0, GPIO_SPEED_FREQ_LOW,
                     GPIO_NOPULL);
    pin_on_POWER();
    pin_off_RUN();
    pin_off_STOP();
    pin_off_FAULT();
}

void PLC_IO::timer_proc()
{
    switch (STM32_SYSTICK::get_tick() % 1000)
    {
    case 0:
        pin_on_STOP();
        break;
    case 500:
        pin_off_STOP();
        break;
    }
}

void ISR::HardFault()
{
    PLC_IO::pin_on_FAULT();
}
