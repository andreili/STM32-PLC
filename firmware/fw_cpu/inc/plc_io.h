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

    static inline void pin_on_POWER()  { GPIOB->BSRR = PIN_PWR; }
    static inline void pin_off_POWER() { GPIOB->BSRR = (PIN_PWR << GPIO_BSRR_BR0_Pos); }
    static inline void pin_on_RUN()  { GPIOB->BSRR = PIN_RUN; }
    static inline void pin_off_RUN() { GPIOB->BSRR = (PIN_RUN << GPIO_BSRR_BR0_Pos); }
    static inline void pin_on_STOP()  { GPIOB->BSRR = PIN_STP; }
    static inline void pin_off_STOP() { GPIOB->BSRR = (PIN_STP << GPIO_BSRR_BR0_Pos); }
    static inline void pin_on_FAULT()  { GPIOB->BSRR = PIN_FLT; }
    static inline void pin_off_FAULT() { GPIOB->BSRR = (PIN_FLT << GPIO_BSRR_BR0_Pos); }

    static void timer_proc();
private:
};

#endif // PLC_IO_H
