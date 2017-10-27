#include "plc_io.h"
#ifdef PLATFORM_STM32
#include "ISRstm32f429xx.h"
#include "stm32_rcc.h"
#include "stm32_systick.h"
#else
#include <ctime>
#endif
#include "plc_control.h"

void PLC_IO::init()
{
#ifdef PLATFORM_STM32
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
#else
#warning Init GPIO
#endif
    pin_off_POWER();
    pin_off_RUN();
    pin_off_STOP();
    pin_off_RS_BLINK();
    pin_off_ERROR();
    pin_off_FAULT();
    pin_off_COM_FAULT();
}

#ifdef PLATFORM_STM32
void PLC_IO::pin_on_POWER()  { gpiob.pin_ON(PIN_PWR); }
void PLC_IO::pin_off_POWER() { gpiob.pin_OFF(PIN_PWR); }
void PLC_IO::pin_on_RUN()  { gpioa.pin_OFF(PIN_RUN); }
void PLC_IO::pin_off_RUN() { gpioa.pin_ON(PIN_RUN); }
void PLC_IO::pin_on_STOP()  { gpioa.pin_OFF(PIN_STP); }
void PLC_IO::pin_off_STOP() { gpioa.pin_ON(PIN_STP); }
void PLC_IO::pin_on_RS_BLINK()  { gpiob.pin_ON(PIN_RSB); }
void PLC_IO::pin_off_RS_BLINK() { gpiob.pin_OFF(PIN_RSB); }
void PLC_IO::pin_on_ERROR()  { gpiob.pin_ON(PIN_ERR); }
void PLC_IO::pin_off_ERROR() { gpiob.pin_OFF(PIN_ERR); }
void PLC_IO::pin_on_FAULT()  { gpiob.pin_ON(PIN_FLT); }
void PLC_IO::pin_off_FAULT() { gpiob.pin_OFF(PIN_FLT); }
void PLC_IO::pin_on_COM_FAULT()  { gpiod.pin_ON(PIN_CFL); }
void PLC_IO::pin_off_COM_FAULT() { gpiod.pin_OFF(PIN_CFL); }
#else
#warning GPIO proc
void PLC_IO::pin_on_POWER()  {  }
void PLC_IO::pin_off_POWER() {  }
void PLC_IO::pin_on_RUN()  {  }
void PLC_IO::pin_off_RUN() {  }
void PLC_IO::pin_on_STOP()  {  }
void PLC_IO::pin_off_STOP() {  }
void PLC_IO::pin_on_RS_BLINK()  {  }
void PLC_IO::pin_off_RS_BLINK() {  }
void PLC_IO::pin_on_ERROR()  {  }
void PLC_IO::pin_off_ERROR() {  }
void PLC_IO::pin_on_FAULT()  {  }
void PLC_IO::pin_off_FAULT() {  }
void PLC_IO::pin_on_COM_FAULT()  {  }
void PLC_IO::pin_off_COM_FAULT() {  }
#endif

void PLC_IO::timer_proc()
{
    pin_on_POWER();

#ifdef PLATFORM_STM32
    uint32_t ticks = STM32_SYSTICK::get_tick();
#else
    struct timespec ticks_str;
    clock_gettime(CLOCK_MONOTONIC, &ticks_str);
    uint32_t ticks = ticks_str.tv_sec * 1000 + ticks_str.tv_nsec / 1000000;
#endif
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
