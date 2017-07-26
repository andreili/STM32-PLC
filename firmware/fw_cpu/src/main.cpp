#include "plc_control.h"
#include "stm32_inc.h"
#include "my_func.h"

#define MEM_SPEED_TEST

int main()
{
    PLC_CONTROL::print_message("\033[2JSTM32 PLC\n");

    #if defined (DATA_IN_ExtSDRAM)
    if (STM32_SDRAM::run_tests(SDRAM_BASE_BANK1, (16 * 1024 * 1024)) != STM32_RESULT_OK)
        Error_Handler();
    #ifdef MEM_SPEED_TEST
    PLC_CONTROL::print_message("Test RAM speeds\n");
    uint8_t buf_int[1024];
    uint32_t ticks = STM32_SYSTICK::get_tick();
    for (int i=0 ; i<1000*1000 ; ++i)
        memset(buf_int, i%100, 1024);
    PLC_CONTROL::print_message("Internal RAM: %U ticks\n", STM32_SYSTICK::get_tick() - ticks);
    ticks = STM32_SYSTICK::get_tick();
    uint8_t* buf_ext = (uint8_t*)SDRAM_BASE_BANK1;
    for (volatile int i=0 ; i<1000*1000 ; ++i)
        memset(buf_ext, i%100, 1024);
    PLC_CONTROL::print_message("External RAM: %U ticks\n", STM32_SYSTICK::get_tick() - ticks);
    #endif
    #endif

    PLC_CONTROL::set_initialized(1);
    PLC_CONTROL::set_stop(0);
    PLC_CONTROL::set_run(1);

    PLC_CONTROL::print_message("Start main cycle\n");
    int iteration = 0;
    while (1)
    {
        PLC_CONTROL::print_message("Test iteration: %U\n", ++iteration);
        if (STM32_SDRAM::run_tests(SDRAM_BASE_BANK1, (16 * 1024 * 1024)) != STM32_RESULT_OK)
            Error_Handler();
    }
    
    return 0;
}
