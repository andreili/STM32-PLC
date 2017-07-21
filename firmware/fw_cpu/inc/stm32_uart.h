#ifndef __STM32_UART__
#define __STM32_UART__

#include "stm32f4xx.h"

#define UART_BUF_SIZE 1024

class STM32_UART
{
public:
	void init(USART_TypeDef* usart);
    void set_baud_rate(uint32_t brate);
    
    void send_char(char ch);
	void send_str(const char *str);
	void send_buf(const char *buf, int size);

	void irq_proc();
private:
    USART_TypeDef*  m_usart;
    uint32_t        m_brate;
    bool            m_busy;
    
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
