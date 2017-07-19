#include "stm32_uart.h"
#include "init.h"
#include "ISRstm32f429xx.h"
#include "bitbanding.h"
#include "peripheral_def.h"
#include "stm32_rcc.h"
#include "stm32_gpio.h"
#include "my_func.h"

#define UART_BRR (F_CPU / BRATE)

#define USART_DISABLE() BIT_BAND_PER(m_usart->CR1, USART_CR1_UE) = 0
#define USART_ENABLE()  BIT_BAND_PER(m_usart->CR1, USART_CR1_UE) = 1

#define UART_STOPBITS_1                     ((uint32_t)0x00000000U)
#define UART_STOPBITS_2                     ((uint32_t)USART_CR2_STOP_1)

#define UART_WORDLENGTH_8B                  ((uint32_t)0x00000000U)
#define UART_WORDLENGTH_9B                  ((uint32_t)USART_CR1_M)

#define UART_PARITY_NONE                    ((uint32_t)0x00000000U)
#define UART_PARITY_EVEN                    ((uint32_t)USART_CR1_PCE)
#define UART_PARITY_ODD                     ((uint32_t)(USART_CR1_PCE | USART_CR1_PS)) 

#define UART_HWCONTROL_NONE                  ((uint32_t)0x00000000U)
#define UART_HWCONTROL_RTS                   ((uint32_t)USART_CR3_RTSE)
#define UART_HWCONTROL_CTS                   ((uint32_t)USART_CR3_CTSE)
#define UART_HWCONTROL_RTS_CTS               ((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE))

#define UART_MODE_RX                        ((uint32_t)USART_CR1_RE)
#define UART_MODE_TX                        ((uint32_t)USART_CR1_TE)
#define UART_MODE_TX_RX                     ((uint32_t)(USART_CR1_TE |USART_CR1_RE))

#define UART_OVERSAMPLING_16                    ((uint32_t)0x00000000U)
#define UART_OVERSAMPLING_8                     ((uint32_t)USART_CR1_OVER8)

void STM32_UART::init(USART_TypeDef* usart)
{
    m_usart = usart;
    m_busy = false;
    USART_DISABLE();
    set_config();
    m_usart->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
    m_usart->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
    
    switch ((uint32_t)m_usart)
    {
    case USART1_BASE:
        //RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        break;
    case USART2_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        break;
    case USART3_BASE:
        BIT_BAND_PER(RCC->APB1ENR, RCC_APB1ENR_USART3EN) = 1;
        STM32_RCC::clk_enable_GPIOB();
        gpiob.set_config(GPIO_PIN_10|GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_AF7_USART3, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
        break;
    case UART4_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
        break;
    case UART5_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
        break;
    case USART6_BASE:
        break;
    case UART7_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_UART7EN;
        break;
    case UART8_BASE:
        RCC->APB1ENR |= RCC_APB1ENR_UART8EN;
        break;
    }
    USART_ENABLE();
}

#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            (((_PCLK_)*25U)/(4U*(_BAUD_)))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        (((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U + 50U) / 100U)
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            (((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U)) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))

void STM32_UART::set_baud_rate(uint32_t brate)
{
    m_brate = brate;
    if((m_usart == USART1) || (m_usart == USART6))
    {
        m_usart->BRR = UART_BRR_SAMPLING16(STM32_RCC::get_PCLK2_freq(), brate);
    }
    else
    {
        m_usart->BRR = UART_BRR_SAMPLING16(STM32_RCC::get_PCLK1_freq(), brate);
    }
}

void STM32_UART::send_str(const char *str)
{
    while (m_busy) {}
    m_busy = true;
	send_buf(str, strlen(str) + 1);
}

void STM32_UART::send_buf(const char *buf, int size)
{
	m_tx_size = size;
	m_tx_pos = 0;
    while (m_tx_pos < m_tx_size)
    {
        while ((m_usart->SR & USART_SR_TXE) != USART_SR_TXE);
        m_usart->DR = buf[m_tx_pos++];
    }
    //memcpy(m_tx_buf, buf, size);
    //m_usart->CR1 |= USART_CR1_TXEIE;
    //BIT_BAND_PER(m_usart->CR1, USART_CR1_TXEIE) = 1;
}

void inline STM32_UART::irq_proc()
{
	if (m_usart->SR & USART_SR_RXNE)
		recv_data();
	else if (m_usart->SR & USART_SR_TXE)
		send_data();
}

void STM32_UART::set_config()
{
    uint32_t tmpreg;
    
    /*-------------------------- USART CR2 Configuration -----------------------*/
    tmpreg = m_usart->CR2;

    /* Clear STOP[13:12] bits */
    tmpreg &= (uint32_t)~((uint32_t)USART_CR2_STOP);

    /* Configure the UART Stop Bits: Set STOP[13:12] bits according to huart->Init.StopBits value */
    tmpreg |= UART_STOPBITS_1;

    /* Write to USART CR2 */
    m_usart->CR2 = tmpreg;

    /*-------------------------- USART CR1 Configuration -----------------------*/
    tmpreg = m_usart->CR1;

    /* Clear M, PCE, PS, TE and RE bits */
    tmpreg &= (uint32_t)~((uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | \
                            USART_CR1_RE | USART_CR1_OVER8));

    /* Configure the UART Word Length, Parity and mode: 
    Set the M bits according to huart->Init.WordLength value 
    Set PCE and PS bits according to huart->Init.Parity value
    Set TE and RE bits according to huart->Init.Mode value
    Set OVER8 bit according to huart->Init.OverSampling value */
    tmpreg |= UART_WORDLENGTH_8B | UART_PARITY_NONE | UART_MODE_TX_RX | UART_OVERSAMPLING_16;

    tmpreg |= USART_CR1_RXNEIE;

    /* Write to USART CR1 */
    m_usart->CR1 = tmpreg;

    /*-------------------------- USART CR3 Configuration -----------------------*/  
    tmpreg = m_usart->CR3;

    /* Clear CTSE and RTSE bits */
    tmpreg &= (uint32_t)~((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE));

    /* Configure the UART HFC: Set CTSE and RTSE bits according to huart->Init.HwFlowCtl value */
    tmpreg |= UART_HWCONTROL_NONE;

    /* Write to USART CR3 */
    m_usart->CR3 = tmpreg;
}

void STM32_UART::recv_data()
{
	m_rx_buf[m_rx_pos++] = m_usart->DR;
	m_rx_size = m_rx_pos;
}

void STM32_UART::send_data()
{
	m_usart->DR = m_tx_buf[m_tx_pos++];
	if (m_tx_pos >= m_tx_size)
    {
    	BIT_BAND_PER(m_usart->CR1, USART_CR1_TXEIE) = 0;
        m_busy = false;
    }
		//m_usart->CR1 &= ~(USART_CR1_TXEIE);
}



#ifdef USE_USART1
STM32_UART uart1;

void ISR::USART1_IRQ()
{
	uart1.irq_proc();
}
#endif

#ifdef USE_USART2
STM32_UART uart2;

void ISR::USART2_IRQ()
{
	uart2.irq_proc();
}
#endif

#ifdef USE_USART3
STM32_UART uart3;

void ISR::USART3_IRQ()
{
    uart3.irq_proc();
}
#endif

#ifdef USE_USART4
STM32_UART uart4;

void ISR::UART4_IRQ()
{
	uart4.irq_proc();
}
#endif

#ifdef USE_USART5
STM32_UART uart5;

void ISR::UART5_IRQ()
{
	uart5.irq_proc();
}
#endif

#ifdef USE_USART6
STM32_UART uart6;

void ISR::USART6_IRQ()
{
	uart6.irq_proc();
}
#endif

#ifdef USE_USART7
STM32_UART uart7;

void ISR::UART7_IRQ()
{
	uart7.irq_proc();
}
#endif

#ifdef USE_USART8
STM32_UART uart8;

void ISR::UART8_IRQ()
{
	uart8.irq_proc();
}
#endif
