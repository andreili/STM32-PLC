#include "plcstate.h"
#include "settings.h"
#include "hps_0_arm_a9_0.h"

plc_state_t PLCState::m_state;
bool        PLCState::m_blink_on;

void PLCState::init()
{
    m_blink_on = false;

    m_state.pwr_on = true;
    m_state.pwr_ok = true;  // TODO: check ADC

    m_state.stop_load = false;
    m_state.stop_fault = false;
    m_state.stop_stop = true;

    m_state.run_load = false;
    m_state.run_run = false;

    m_state.fault = false;
}

void PLCState::to_stop()
{
    m_state.stop_stop = true;
    m_state.run_run = false;
}

void PLCState::to_full_stop()
{
    m_state.stop_load = false;
    m_state.stop_fault = false;
    m_state.stop_stop = true;

    m_state.run_load = false;
    m_state.run_run = false;

    m_state.fault = false;
}

void PLCState::to_load_fw_in_plc()
{
    m_state.stop_load = false;
}

void PLCState::to_fw_load()
{
    m_state.run_run = true;
    m_state.run_load = true;
}

void PLCState::to_run()
{
    m_state.stop_stop = false;
    m_state.run_run = true;
    m_state.run_load = false;
}

void PLCState::to_error()
{
    m_state.fault = true;
}

void PLCState::to_fault()
{
    m_state.stop_stop = true;
    m_state.run_run = false;
    m_state.fault = true;
}

#define LED_PWR_OFFSET 0
#define LED_STP_OFFSET 1
#define LED_RUN_OFFSET 2
#define LED_FLT_OFFSET 3

void PLCState::signal_1Hz()
{
    m_blink_on = !m_blink_on;

#ifdef FPGA_ALLOW
    char led_byte = 0;

    if (m_state.pwr_on)
    {
        if (m_state.pwr_ok || m_blink_on)
            led_byte |= (1 << LED_PWR_OFFSET);
    }

    if (m_state.stop_stop)
    {
        if ((m_state.stop_fault || m_state.stop_load) && m_blink_on)
            led_byte |= (1 << LED_STP_OFFSET);
    }

    if (m_state.run_run)
    {
        if (m_state.run_load && m_blink_on)
            led_byte |= (1 << LED_RUN_OFFSET);
    }

    if (m_state.fault)
        led_byte |= (1 << LED_FLT_OFFSET);

    *((char*)LEDS_BASE) = led_byte;
#endif
}
