#include "plc_io.h"
#include "ISRstm32f429xx.h"
#include "stm32_rcc.h"
#include "stm32_systick.h"
#include "plc_control.h"

void PLC_IO::init()
{
    STM32_RCC::enable_clk_GPIOA();
    gpioa.set_config(PIN_RUN | PIN_STP,
                     GPIO_MODE_OUTPUT_PP, 0, GPIO_SPEED_FREQ_LOW,
                     GPIO_NOPULL);
    STM32_RCC::enable_clk_GPIOB();
    gpiob.set_config(PIN_PWR | PIN_ERR | PIN_FLT | PIN_CFL,
                     GPIO_MODE_OUTPUT_PP, 0, GPIO_SPEED_FREQ_LOW,
                     GPIO_NOPULL);
    STM32_RCC::enable_clk_GPIOD();
    gpiod.set_config(PIN_RSB,
                     GPIO_MODE_OUTPUT_PP, 0, GPIO_SPEED_FREQ_LOW,
                     GPIO_NOPULL);
    pin_off_POWER();
    pin_off_RUN();
    pin_off_STOP();
    pin_off_RS_BLINK();
    pin_off_ERROR();
    pin_off_FAULT();
    pin_off_COM_FAULT();
}

void PLC_IO::timer_proc()
{
    pin_on_POWER();

    uint32_t ticks = STM32_SYSTICK::get_tick();
    if (PLC_CONTROL::in_rs_blink())
    {
        switch (ticks % 1000)
        {
        case 0:
            pin_on_RS_BLINK();
            break;
        case 500:
            pin_off_RS_BLINK();
            break;
        }
    }
    else
        pin_on_RS_BLINK();

    if (!PLC_CONTROL::is_initialized())
    {
        pin_on_STOP();
        pin_on_FAULT();
        return;
    }

    if (PLC_CONTROL::in_run())
        pin_on_RUN();
    else
        pin_off_RUN();

    if (PLC_CONTROL::in_stop())
        pin_on_STOP();
    else
        pin_off_STOP();

    switch (ticks % 1000)
    {
    case 0:
        if (PLC_CONTROL::in_fault())
            pin_on_FAULT();
        break;
    case 500:
        pin_off_FAULT();
        break;
    }
}
