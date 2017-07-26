#include "plc_control.h"
#include "stm32_inc.h"
#include "my_func.h"

#define MEM_SPEED_TEST

#ifdef MEM_SPEED_TEST
#define TEST_CYCLES (1000 * 100)
#define TEST_SIZE (1024)

void test_mem_speed(uint8_t *mem, const char *title)
{
    uint32_t ticks = STM32_SYSTICK::get_tick();
    for (int i=0 ; i<TEST_CYCLES ; ++i)
        memset(mem, i%100, TEST_SIZE);
    uint32_t time = STM32_SYSTICK::get_tick() - ticks;
    uint32_t speed_mb = ((TEST_SIZE * TEST_CYCLES) / time) / (1024);
    PLC_CONTROL::print_message("\t%S: %U ticks, ~%UMb/s\n",
                               title, time, speed_mb);
}
#endif

int main()
{
    PLC_CONTROL::print_message("\033[2J+-----------------------------+\n"
                                      "|          STM32 PLC          |\n"
                                      "|         System info         |\n"
                                      "| CPU Speed: %03UMHz           |\n"
                                      "| RAM size: 192kb + %03UMb     |\n"
                                      "+-----------------------------+\n",
                               STM32_PLLN, STM32_SDRAM_SIZE_MB);

    #if defined (DATA_IN_ExtSDRAM)
    if (STM32_SDRAM::run_tests(SDRAM_BASE_BANK1,
                               (STM32_SDRAM_SIZE_MB * 1024 * 1024)) != STM32_RESULT_OK)
        Error_Handler();
    #ifdef MEM_SPEED_TEST
    PLC_CONTROL::print_message("+-----------------------------+\n"
                               "Test RAM speeds\n");
    uint8_t buf_int[1024];
    test_mem_speed(buf_int, "Internal RAM");
    test_mem_speed((uint8_t*)SDRAM_BASE_BANK1, "External RAM");
    #endif
    #endif

    PLC_CONTROL::set_initialized(1);
    PLC_CONTROL::set_stop(0);
    PLC_CONTROL::set_run(1);

    PLC_CONTROL::print_message("Start main cycle\n");
    int iteration = 0;
    while (1)
    {
        PLC_CONTROL::print_message("\r\tTest iteration: %U", ++iteration);
        if (STM32_SDRAM::run_tests(SDRAM_BASE_BANK1,
                                   (STM32_SDRAM_SIZE_MB * 1024 * 1024), false) != STM32_RESULT_OK)
            Error_Handler();
    }
    
    return 0;
}
