#ifndef PLC_IO_H
#define PLC_IO_H

#include "stm32_gpio.h"

#define PIN_PWR GPIO_PIN_12
#define PIN_RUN GPIO_PIN_11
#define PIN_STP GPIO_PIN_9
#define PIN_RSB GPIO_PIN_13
#define PIN_ERR GPIO_PIN_14
#define PIN_FLT GPIO_PIN_15
#define PIN_CFL GPIO_PIN_13

class PLC_IO
{
public:
    static void init();

    static inline void pin_on_POWER()  { gpiob.pin_ON(PIN_PWR); }
    static inline void pin_off_POWER() { gpiob.pin_OFF(PIN_PWR); }

    static inline void pin_on_RUN()  { gpioa.pin_OFF(PIN_RUN); }
    static inline void pin_off_RUN() { gpioa.pin_ON(PIN_RUN); }

    static inline void pin_on_STOP()  { gpioa.pin_OFF(PIN_STP); }
    static inline void pin_off_STOP() { gpioa.pin_ON(PIN_STP); }

    static inline void pin_on_RS_BLINK()  { gpiob.pin_ON(PIN_RSB); }
    static inline void pin_off_RS_BLINK() { gpiob.pin_OFF(PIN_RSB); }

    static inline void pin_on_ERROR()  { gpiob.pin_ON(PIN_ERR); }
    static inline void pin_off_ERROR() { gpiob.pin_OFF(PIN_ERR); }

    static inline void pin_on_FAULT()  { gpiob.pin_ON(PIN_FLT); }
    static inline void pin_off_FAULT() { gpiob.pin_OFF(PIN_FLT); }

    static inline void pin_on_COM_FAULT()  { gpiod.pin_ON(PIN_CFL); }
    static inline void pin_off_COM_FAULT() { gpiod.pin_OFF(PIN_CFL); }

    static void timer_proc();
private:
};

#endif // PLC_IO_H
