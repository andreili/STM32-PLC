#include "plc_control.h"
#include "stm32_inc.h"

int main()
{
    PLC_CONTROL::print_message("STM32 PLC\n");

    PLC_CONTROL::set_initialized(1);
    PLC_CONTROL::set_stop(0);
    PLC_CONTROL::set_run(1);

    while (1)
    {
    }
    
    return 0;
}
