#include "plc_control.h"
#include "xprintf.h"
#include "stm32_inc.h"

void uart3_putc(unsigned char ch)
{
    uart3.send_char(ch);
}

int main()
{
    xfunc_out = uart3_putc;
    STM32_SYSTICK::delay(1000);
    xprintf("STM32 PLC\n\r");
    //STM32_SYSTICK::delay(1000);

    PLC_CONTROL::set_initialized(1);
    PLC_CONTROL::set_stop(0);
    PLC_CONTROL::set_run(1);
    while (1)
    {
    }
    
    return 0;
}
