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
    STM32_RCC::deinit();
    STM32_GPIO::init_all();
    STM32_SYSTICK::init();

    #if defined (DATA_IN_ExtSDRAM)
    if (STM32_SDRAM::init() != STM32_RESULT_OK)
        Error_Handler();
    #endif

    /* Configure the Vector Table location add offset address ------------------*/
    #ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
    #else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
    #endif

    PLC_CONTROL::init();

    #ifdef INSTRUCTION_CACHE_ENABLE
    STM32_FLASH::enable_instruction_cache();
    #endif
    #ifdef DATA_CACHE_ENABLE
    STM32_FLASH::enable_data_cache();
    #endif
    #ifdef PREFETCH_ENABLE
    STM32_FLASH::enable_prefetch_buffer();
    #endif

    STM32_NVIC::init();
}

void SystemInit()
{
    base_init();

    /* GPIO initializations */
    PLC_IO::init();

    // system initialization
    //__enable_fault_irq();
    //__enable_irq();
    STM32_RCC::init();
    STM32_SYSTICK::init();

    /* Other IO and peripheral initializations */
    STM32_UART::init_all();

    /* Initialize interrupt vectors for a peripheral */
    STM32_NVIC::init_vectors();

    uart3.init(STM32_BRATE_UART3);
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
