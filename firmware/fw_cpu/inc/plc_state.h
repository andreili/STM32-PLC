#ifndef PLC_STATE_H
#define PLC_STATE_H

#include "stm32_rtc.h"

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t msec;
} plc_datetime_t;

typedef struct
{
    plc_datetime_t  start_dt;
    plc_datetime_t  current_dt;
    uint16_t        last_cycle_time;    /* 0..8191, 8191 = 1sec */
} plc_state_t;

class PLC_STATE
{
public:
    static void init();

    static inline void update_ct() { get_dt(&m_current_dt); }

    static inline plc_datetime_t* get_start_dt() { return &m_start_dt; }
    static inline plc_datetime_t* get_current_dt() { return &m_current_dt; }

private:
    static uint16_t         m_last_cycle_time;

    static plc_datetime_t   m_start_dt;
    static plc_datetime_t   m_current_dt;

    static void get_dt(plc_datetime_t* dt);
};

#endif // PLC_STATE_H
