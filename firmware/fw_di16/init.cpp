#include "stm32_inc.h"
#include "my_func.h"

int main();

/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */

// base initialization
void base_init()
{
    const uint32_t _RAM_Start = 0x20000000;
    memset((uint8_t*)&_RAM_Start, 0, 1024*16);

    STM32_RCC::deinit();
    STM32_GPIO::init_all();
    STM32_SYSTICK::init();

    /* Configure the Vector Table location add offset address ------------------*/
    #ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
    #else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
    #endif

    #ifdef STM32F429xx
    #ifdef INSTRUCTION_CACHE_ENABLE
    STM32_FLASH::enable_instruction_cache();
    #endif
    #endif

    STM32_NVIC::init();
}

void SystemInit()
{
    base_init();

    /* GPIO initializations */

    // system initialization
    //__enable_fault_irq();
    //__enable_irq();
    STM32_RCC::init();
    STM32_SYSTICK::init();

    /* Other IO and peripheral initializations */

    /* Initialize interrupt vectors for a peripheral */
    STM32_NVIC::init_vectors();
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
