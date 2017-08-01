#include "plc_control.h"
#include "stm32_uart.h"
#include "xprintf.h"
#include <stdarg.h>

plc_state_t PLC_CONTROL::m_state;
char PLC_CONTROL::m_text_buf[PLC_TEXT_BUF_SIZE];

void xfunc_out(unsigned char ch)
{
    uart3.send_char(ch);
}

void PLC_CONTROL::init()
{
    m_state.run = 0;
    m_state.stop = 1;
    m_state.fault = 0;
    m_state.initialized = 0;
}

void xvprintf(const char* fmt, va_list arp);
extern char* outptr;

void PLC_CONTROL::print_message(const char* fmt, ...)
{
    va_list arp;

    outptr = m_text_buf;

    va_start(arp, fmt);
    xvprintf(fmt, arp);
    va_end(arp);

    *outptr = 0;
    outptr = 0;
    uart3.send_str(m_text_buf, TXRX_MODE::INTERRUPT);
}
