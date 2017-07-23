#include "stm32_inc.h"
#include "init.h"
#include "my_func.h"
#include "plc_control.h"
#include "plc_io.h"

int main();

/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */

// base initialization
void base_init()
{
    memset((uint8_t*)0x20000000, 0, 0x20000);
    STM32_RCC::deinit();

    /* Configure the Vector Table location add offset address ------------------*/
    #ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
    #else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
    #endif

    PLC_CONTROL::init();

    #if defined (DATA_IN_ExtSDRAM)
    if (STM32_SDRAM::init() != STM32_RESULT_OK)
        Error_Handler();
    #endif

    #ifdef INSTRUCTION_CACHE_ENABLE
    STM32_FLASH::enable_instruction_cache();
    #endif
    #ifdef DATA_CACHE_ENABLE
    STM32_FLASH::enable_data_cache();
    #endif
    #ifdef PREFETCH_ENABLE
    STM32_FLASH::enable_prefetch_buffer();
    #endif

    STM32_NVIC::set_priority_grouping(NVIC_PRIORITYGROUP_4);
    STM32_SYSTICK::init();
}

void SystemInit()
{
    base_init();

    /* GPIO initializations */
    STM32_GPIO::init_all();
    PLC_IO::init();
    uart3.init_base(USART3);

    // system initialization
    //__enable_fault_irq();
    //__enable_irq();
    STM32_RCC::init();

    /* Other IO and peripheral initializations */
    uart3.init();
    uart3.set_baud_rate(115200);


    #if defined (DATA_IN_ExtSDRAM)
    if (STM32_SDRAM::run_tests(SDRAM_BASE_BANK1, 16 * 1024 * 1024) != STM32_RESULT_OK)
        Error_Handler();
    #endif
}

#define INIT_SP() \
    { \
        __ASM volatile("mov r3, 0x08000000\n\r" \
                       "ldr sp, [r3, #0]\n\r" : : ); \
    } \

void ISR::Reset()
{
    INIT_SP();
    SystemInit();
    main();
}
