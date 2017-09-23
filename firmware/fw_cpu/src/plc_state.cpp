#include "plc_state.h"
#include "stm32f4xx.h"

uint32_t       PLC_STATE::m_last_cycle_time;
plc_datetime_t PLC_STATE::m_start_dt;
plc_datetime_t PLC_STATE::m_current_dt;

void PLC_STATE::init()
{
    get_dt(&m_start_dt);
    m_current_dt = m_start_dt;
}

#define MSEC_MINUTE (1000*60)

void PLC_STATE::update_ct()
{
    uint32_t last_msec = ((((m_current_dt.hour * 60) + m_current_dt.min) * 60) + m_current_dt.sec) * 1000 + m_current_dt.msec;
    get_dt(&m_current_dt);
    uint32_t new_msec = ((((m_current_dt.hour * 60) + m_current_dt.min) * 60) + m_current_dt.sec) * 1000 + m_current_dt.msec;

    new_msec -= last_msec;
    if (new_msec >= MSEC_MINUTE)
        m_last_cycle_time = 1;
    else
        m_last_cycle_time = new_msec;
}

void PLC_STATE::get_dt(plc_datetime_t* dt)
{
    uint32_t ssr = RTC->SSR;

    uint32_t tmpreg = (RTC->TR & RTC_TR_RESERVED_MASK);
    dt->hour = RTC_Bcd2ToByte((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16U);
    dt->min = RTC_Bcd2ToByte((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >>8U);
    dt->sec = RTC_Bcd2ToByte(tmpreg & (RTC_TR_ST | RTC_TR_SU));
    dt->msec = (1000 * (STM32_RTC_SYNC_PREDIV - ssr)) / (STM32_RTC_SYNC_PREDIV + 1);

    tmpreg = (RTC->DR & RTC_DR_RESERVED_MASK);

    /* Fill the structure fields with the read parameters */
    dt->year = RTC_Bcd2ToByte((tmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16U);
    dt->month = RTC_Bcd2ToByte((tmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8U);
    dt->day = RTC_Bcd2ToByte(tmpreg & (RTC_DR_DT | RTC_DR_DU));
}
