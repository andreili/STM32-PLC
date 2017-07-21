#include "plc_control.h"
#include "my_func.h"

plc_state_t PLC_CONTROL::m_state;

void PLC_CONTROL::init()
{
    memset((uint8_t*)&m_state, 0, sizeof(plc_state_t));
    m_state.stop = 1;
}
