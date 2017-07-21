#include "plc_control.h"
#include "my_func.h"

plc_state_t PLC_CONTROL::m_state;

void PLC_CONTROL::init()
{
    m_state.run = 0;
    m_state.stop = 1;
    m_state.fault = 0;
    m_state.initialized = 0;
    //memset((uint8_t*)&m_state, 0, sizeof(plc_state_t));
    //m_state.stop = 1;
}
