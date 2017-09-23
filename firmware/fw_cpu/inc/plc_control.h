#ifndef PLC_CONTROL_H
#define PLC_CONTROL_H

#include <stdint.h>
#include "stm32_rtc.h"

#define PLC_TEXT_BUF_SIZE 1024

class PLC_CONTROL
{
public:
    static void init();
    static void init_hw();
    static void print_message(const char* fmt, ...);

    static void init_seq();
    static void main();

    static void fault_state_on();

    static inline uint8_t in_run() { return m_state_run; }
    static inline uint8_t in_stop() { return m_state_stop; }
    static inline uint8_t in_rs_blink() { return m_state_rs_blink; }
    static inline uint8_t in_error() { return m_state_error; }
    static inline uint8_t in_fault() { return m_state_fault; }
    static inline uint8_t is_initialized() { return m_state_initialized; }
private:
    static uint8_t      m_state_run;
    static uint8_t      m_state_stop;
    static uint8_t      m_state_rs_blink;
    static uint8_t      m_state_error;
    static uint8_t      m_state_fault;
    static uint8_t      m_state_com_fault;
    static uint8_t      m_state_initialized;
    static char         m_text_buf[PLC_TEXT_BUF_SIZE];
    static STM32_RTC_Date   m_start_date;
    static STM32_RTC_Time   m_start_time;

    static inline void set_run(uint8_t val) { m_state_run = val; }
    static inline void set_stop(uint8_t val) { m_state_stop = val; }
    static inline void set_rs_blink(uint8_t val) { m_state_rs_blink = val; }
    static inline void set_error(uint8_t val) { m_state_error = val; }
    static inline void set_fault(uint8_t val) { m_state_fault = val; }
    static inline void set_com_fault(uint8_t val) { m_state_com_fault = val; }
    static inline void set_initialized(uint8_t val) { m_state_initialized = val; }

    static void scheck_RTC();
    static void init_fs();

    static void test_RAM(bool print_debug);
    static void test_RAM_speed();
};

#endif // PLC_CONTROL_H
