#ifndef PLC_IO_H
#define PLC_IO_H

#ifdef PLATFORM_STM32
#include "stm32_gpio.h"

#define PIN_PWR GPIO_PIN_12
#define PIN_RUN GPIO_PIN_11
#define PIN_STP GPIO_PIN_9
#define PIN_RSB GPIO_PIN_13
#define PIN_ERR GPIO_PIN_14
#define PIN_FLT GPIO_PIN_15
#define PIN_CFL GPIO_PIN_13
#endif

class PLC_IO
{
public:
    static void init();

    static void pin_on_POWER();
    static void pin_off_POWER();

    static void pin_on_RUN();
    static void pin_off_RUN();

    static void pin_on_STOP();
    static void pin_off_STOP();

    static void pin_on_RS_BLINK();
    static void pin_off_RS_BLINK();

    static void pin_on_ERROR();
    static void pin_off_ERROR();

    static void pin_on_FAULT();
    static void pin_off_FAULT();

    static void pin_on_COM_FAULT();
    static void pin_off_COM_FAULT();

    static void timer_proc();
private:
};

#endif // PLC_IO_H
