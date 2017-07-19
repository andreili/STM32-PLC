#ifndef STM32_SYSTICK_H
#define STM32_SYSTICK_H

#include <stdint.h>

class STM32_SYSTICK
{
public:
    static void init();

    static inline uint32_t get_tick() { return m_tick; }

    static void on_tick();
private:
    static uint32_t m_tick;
};

#endif
