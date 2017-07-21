#include "plc_io.h"
#include "ISRstm32f429xx.h"
#include "stm32_rcc.h"
#include "stm32_systick.h"
#include "plc_control.h"

void PLC_IO::init()
{
    STM32_RCC::enable_clk_GPIOB();
    gpiob.set_config(PIN_PWR | PIN_RUN | PIN_STP | PIN_FLT,
                     GPIO_MODE_OUTPUT_PP, 0, GPIO_SPEED_FREQ_LOW,
                     GPIO_NOPULL);
    pin_off_POWER();
    pin_off_RUN();
    pin_off_STOP();
    pin_off_FAULT();
}

void PLC_IO::timer_proc()
{
    pin_on_POWER();
    PLC_CONTROL::in_run() ? pin_on_RUN() : pin_off_RUN();
    switch (STM32_SYSTICK::get_tick() % 1000)
    {
    case 0:
        if (PLC_CONTROL::in_stop())
            pin_on_STOP();
        if (PLC_CONTROL::in_fault())
            pin_on_FAULT();
        break;
    case 500:
        pin_off_STOP();
        pin_off_FAULT();
        break;
    }
}

void ISR::HardFault()
{
    PLC_IO::pin_on_FAULT();
    PLC_IO::pin_on_STOP();
    PLC_CONTROL::set_fault(1);
    PLC_CONTROL::set_stop(1);
    while (1) {}
}

void Error_Handler()
{
    PLC_IO::pin_on_FAULT();
    PLC_IO::pin_on_STOP();
    PLC_CONTROL::set_fault(1);
    PLC_CONTROL::set_stop(1);
    while (1) {}
}
