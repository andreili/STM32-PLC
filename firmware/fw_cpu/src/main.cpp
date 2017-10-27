#include "plc_control.h"
#ifdef PLATFORM_STM32
#include "stm32_inc.h"
#include "my_func.h"
#endif

#ifdef PLATFORM_STM32
//#define MEM_SPEED_TEST

#ifdef MEM_SPEED_TEST
#define TEST_CYCLES (1000 * 100)
#define TEST_SIZE (1024)

void test_mem_speed(uint8_t *mem, const char *title)
{
    uint32_t ticks = STM32_SYSTICK::get_tick();
    for (int i=0 ; i<TEST_CYCLES ; ++i)
        memset(mem, i%100, TEST_SIZE);
    uint32_t time = STM32_SYSTICK::get_tick() - ticks;
    uint32_t speed_mb = ((TEST_SIZE * TEST_CYCLES) / time) / (TEST_SIZE);
    PLC_CONTROL::print_message("\t%S: %U ticks, ~%UMb/s\n",
                               title, time, speed_mb);
}
#endif
#endif

int main()
{
    PLC_CONTROL::init_seq();
    PLC_CONTROL::main();
    
    return 0;
}
