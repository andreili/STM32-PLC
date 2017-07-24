#ifndef PLC_IO_H
#define PLC_IO_H

#include "stm32_gpio.h"

#define PIN_PWR GPIO_PIN_12
#define PIN_RUN GPIO_PIN_13
#define PIN_STP GPIO_PIN_14
#define PIN_FLT GPIO_PIN_15

class PLC_IO
{
public:
    static void init();

    static inline void pin_on_POWER()  { gpiob.pin_ON(PIN_PWR); }
    static inline void pin_off_POWER() { gpiob.pin_OFF(PIN_PWR); }
    static inline void pin_on_RUN()  { gpiob.pin_ON(PIN_RUN); }
    static inline void pin_off_RUN() { gpiob.pin_OFF(PIN_RUN); }
    static inline void pin_on_STOP()  { gpiob.pin_ON(PIN_STP); }
    static inline void pin_off_STOP() { gpiob.pin_OFF(PIN_STP); }
    static inline void pin_on_FAULT()  { gpiob.pin_ON(PIN_FLT); }
    static inline void pin_off_FAULT() { gpiob.pin_OFF(PIN_FLT); }

    static void timer_proc();
private:
};

#endif // PLC_IO_H
