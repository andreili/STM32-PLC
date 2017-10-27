#include "plc_fw.h"
#include "plc_control.h"
#include "plc_platform.h"

PLCFW plc_fw;

void PLCFW::init()
{
    m_fw_ok = false;
}

bool PLCFW::load()
{
    PLC_CONTROL::print_message("+-----------------------------+\n"
                               "Loading firmware\n");
    m_fw_size = load_file("fw.bin");
    PLC_CONTROL::print_message("+-----------------------------+\n");
    return true;
}
