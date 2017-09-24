#include "plc_state.h"
#include "stm32f4xx.h"
#include "my_func.h"

uint32_t       PLC_STATE::m_last_cycle_time[PLC_CYCLE_TIME_COUNT];
uint32_t       PLC_STATE::m_last_cycle_time_idx;
uint32_t       PLC_STATE::m_last_cycle_time_avg;
plc_datetime_t PLC_STATE::m_start_dt;
plc_datetime_t PLC_STATE::m_current_dt;

void PLC_STATE::init()
{
    get_dt(&m_start_dt);
    m_current_dt = m_start_dt;
    memset((uint8_t*)m_last_cycle_time, 0, sizeof(uint32_t) * PLC_CYCLE_TIME_COUNT);
    m_last_cycle_time_idx = 0;
    m_last_cycle_time_avg = 0;
}

#define MSEC_MINUTE (1000*60)

void PLC_STATE::update_ct()
{
    uint32_t last_msec = ((((m_current_dt.hour * 60) + m_current_dt.min) * 60) + m_current_dt.sec) * 1000 + m_current_dt.msec;
    get_dt(&m_current_dt);
    uint32_t new_msec = ((((m_current_dt.hour * 60) + m_current_dt.min) * 60) + m_current_dt.sec) * 1000 + m_current_dt.msec;

    int32_t ct = new_msec - last_msec;
    if (ct >= 0)
    {
        if ((++m_last_cycle_time_idx) >= PLC_CYCLE_TIME_COUNT)
            m_last_cycle_time_idx = 0;
        m_last_cycle_time[m_last_cycle_time_idx] = ct;

        m_last_cycle_time_avg = 0;
        for (int i=0 ; i<PLC_CYCLE_TIME_COUNT ; ++i)
            m_last_cycle_time_avg += m_last_cycle_time[i];
        m_last_cycle_time_avg /= PLC_CYCLE_TIME_COUNT;
    }
}

void PLC_STATE::get_dt(plc_datetime_t* dt)
{
    uint32_t ssr = RTC->SSR;
    uint32_t tmpreg_tr = RTC->TR;
    uint32_t tmpreg_dr = RTC->DR;

    tmpreg_tr &= RTC_TR_RESERVED_MASK;
    dt->hour = RTC_Bcd2ToByte((tmpreg_tr & (RTC_TR_HT | RTC_TR_HU)) >> 16U);
    dt->min = RTC_Bcd2ToByte((tmpreg_tr & (RTC_TR_MNT | RTC_TR_MNU)) >>8U);
    dt->sec = RTC_Bcd2ToByte(tmpreg_tr & (RTC_TR_ST | RTC_TR_SU));
    dt->msec = (1000 * (STM32_RTC_SYNC_PREDIV - ssr)) / (STM32_RTC_SYNC_PREDIV + 1);

    tmpreg_dr &= RTC_DR_RESERVED_MASK;
    dt->year = RTC_Bcd2ToByte((tmpreg_dr & (RTC_DR_YT | RTC_DR_YU)) >> 16U);
    dt->month = RTC_Bcd2ToByte((tmpreg_dr & (RTC_DR_MT | RTC_DR_MU)) >> 8U);
    dt->day = RTC_Bcd2ToByte(tmpreg_dr & (RTC_DR_DT | RTC_DR_DU));
}
