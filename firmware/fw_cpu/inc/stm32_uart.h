#ifndef __STM32_UART__
#define __STM32_UART__

#include "stm32_inc.h"

#define UART_BUF_SIZE 1024

enum class UART_MODE
{
    DIRECT,
    INTERRUPT,
    DMA,
};

class STM32_UART
{
public:
    static void init_all();
    void init_base(USART_TypeDef* uart);
    void init(uint32_t brate);
    
    void send_char(char ch);
    void send_str(const char *str, TXRX_MODE mode);
    void send_buf(const char *buf, int size, TXRX_MODE mode);

	void irq_proc();
private:
    USART_TypeDef*  m_uart;
    uint32_t        m_brate;
    volatile bool   m_busy;

    void set_baud_rate(uint32_t brate);
    void set_config();
    
	void recv_data();
	void send_data();

	char	m_rx_buf[UART_BUF_SIZE];
	int		m_rx_size;
	int		m_rx_pos;

	char	m_tx_buf[UART_BUF_SIZE];
	int		m_tx_size;
	int		m_tx_pos;
};

extern STM32_UART uart1;
extern STM32_UART uart2;
extern STM32_UART uart3;
extern STM32_UART uart4;
extern STM32_UART uart5;
extern STM32_UART uart6;
extern STM32_UART uart7;
extern STM32_UART uart8;

#endif // __UART__
