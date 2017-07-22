#include "ISRstm32f429xx.h"
#include "stm32f4xx.h"
#include "init.h"
#include "stm32_uart.h"
#include "stm32_rcc.h"
#include "stm32_gpio.h"
#include "stm32_flash.h"
#include "my_func.h"
#include "plc_io.h"
#include "stm32_systick.h"
#include "xprintf.h"
#include "stm32_sdram.h"
#include "plc_control.h"

int main();

/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */

// base initialization
void base_init()
{
    memset((uint8_t*)0x20000000, 0, 0x20000);
    STM32_RCC::deinit();
    PLC_CONTROL::init();
    STM32_FLASH::enable_instruction_cache();
    STM32_FLASH::enable_data_cache();
    STM32_FLASH::enable_prefetch_buffer();

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
}

void uart3_putc(unsigned char ch)
{
    uart3.send_char(ch);
}

void SystemInit()
{
    base_init();
    // system initialization
    __enable_fault_irq();
    __enable_irq();
    STM32_RCC::init();
    
    STM32_GPIO::init_all();
    PLC_IO::init();
    
    uart3.init(USART3);
    uart3.set_baud_rate(115200);
    xfunc_out = uart3_putc;
    xprintf("STM32 PLC\n\r");

    STM32_SYSTICK::delay(1000);

    if (STM32_SDRAM::run_tests(SDRAM_BASE_BANK1, 16 * 1024 * 1024) != STM32_RESULT_OK)
        Error_Handler();
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
