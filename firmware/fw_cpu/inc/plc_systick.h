#ifndef PLC_SYSTICK_H
#define PLC_SYSTICK_H

#include <stdio.h>

class PLC_SYSTICK
{
public:
    static void init();

    static inline uint32_t get_tick() { return m_tick; }

    static void on_tick();
private:
    static uint32_t m_tick;
};

#endif // PLC_SYSTICK_H
