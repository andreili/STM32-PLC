#include "plc_control.h"
#include "stm32_inc.h"

int main()
{
    PLC_CONTROL::print_message("\033[2JSTM32 PLC\n");

    #if defined (DATA_IN_ExtSDRAM)
    if (STM32_SDRAM::run_tests(SDRAM_BASE_BANK1, (16 * 1024 * 1024)) != STM32_RESULT_OK)
        Error_Handler();
    #endif

    PLC_CONTROL::set_initialized(1);
    PLC_CONTROL::set_stop(0);
    PLC_CONTROL::set_run(1);

    PLC_CONTROL::print_message("Start main cycle\n");
    while (1)
    {
    }
    
    return 0;
}
