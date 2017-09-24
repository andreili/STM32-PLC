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

#define PLC_CYCLE_TIME_COUNT 10

typedef struct
{
    plc_datetime_t  start_dt;
    plc_datetime_t  current_dt;
    uint16_t        last_cycle_time;
} plc_state_t;

class PLC_STATE
{
public:
    static void init();

    static void update_ct();

    static inline plc_datetime_t* get_start_dt() { return &m_start_dt; }
    static inline plc_datetime_t* get_current_dt() { return &m_current_dt; }
    static inline uint32_t get_last_cycle_time() { return m_last_cycle_time_avg; }

private:
    static uint32_t         m_last_cycle_time[PLC_CYCLE_TIME_COUNT];
    static uint32_t         m_last_cycle_time_avg;
    static uint32_t         m_last_cycle_time_idx;

    static plc_datetime_t   m_start_dt;
    static plc_datetime_t   m_current_dt;

    static void get_dt(plc_datetime_t* dt);
};

#endif // PLC_STATE_H
