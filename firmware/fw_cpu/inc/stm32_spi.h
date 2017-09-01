#ifndef STM32_SPI_H
#define STM32_SPI_H

#include "stm32_inc.h"

#define SPI_FLAG_RXNE                   SPI_SR_RXNE   /* SPI status flag: Rx buffer not empty flag */
#define SPI_FLAG_TXE                    SPI_SR_TXE    /* SPI status flag: Tx buffer empty flag */
#define SPI_FLAG_BSY                    SPI_SR_BSY    /* SPI status flag: Busy flag */
#define SPI_FLAG_CRCERR                 SPI_SR_CRCERR /* SPI Error flag: CRC error flag */
#define SPI_FLAG_MODF                   SPI_SR_MODF   /* SPI Error flag: Mode fault flag */
#define SPI_FLAG_OVR                    SPI_SR_OVR    /* SPI Error flag: Overrun flag */
#define SPI_FLAG_FRE                    SPI_SR_FRE    /* SPI Error flag: TI mode frame format error flag */

class STM32_SPI;

typedef void(STM32_SPI::*callback_t)();

class STM32_SPI
{
public:
    static void init_all();

    void init();
    void irq();

    uint32_t wait_busy(uint32_t timeout);

    uint32_t transmit(uint8_t* data, uint32_t size, TXRX_MODE mode, uint32_t timeout);
    uint32_t recieve(uint8_t* data, uint32_t size, TXRX_MODE mode, uint32_t timeout);
    uint32_t transmit_recieve(uint8_t* tx_buf, uint8_t* rx_buf, uint32_t size, TXRX_MODE mode, uint32_t timeout);

    ENDIS_REG_FLAG_(m_spi->CR1, SPI_CR1_SPE)
    inline uint32_t is_enabled() { return (m_spi->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE; }

    ENDIS_REG_FLAG_NAME(CRC, m_spi->CR1, SPI_CR1_CRCNEXT)

    inline void enable_IT(uint32_t mask) { m_spi->CR2 |= mask; }
    inline void disable_IT(uint32_t mask) { m_spi->CR2 &= ~mask; }

    inline bool get_flag(uint32_t flag_msk) { return ((m_spi->SR & flag_msk) == flag_msk); }
    inline void clear_ovrflag() { uint32_t tmp; tmp = m_spi->DR; tmp = m_spi->SR ; (void)(tmp); }
    inline void clear_CRCerror() { m_spi->SR &= ~SPI_FLAG_CRCERR; }

    inline void tx_1line() { m_spi->CR1 |= SPI_CR1_BIDIOE; }
    inline void rx_1line() { m_spi->CR1 &= ~SPI_CR1_BIDIOE; }

    inline uint32_t get_TXE() { return m_spi->SR & SPI_FLAG_TXE; }

    inline void crc_on() { m_spi->CR1 |= SPI_CR1_CRCEN; }
    inline void crc_off() { m_spi->CR1 &= ~SPI_CR1_CRCEN; }
    inline void crc_reset() { crc_off(); crc_on(); }
private:
    SPI_TypeDef*    m_spi;
    uint32_t        m_busy;

    uint32_t        m_mode;
    uint32_t        m_direction;
    uint32_t        m_data_size;
    uint32_t        m_crc_calc;

    uint32_t        m_tx_count;
    uint32_t        m_tx_size;
    uint8_t         *m_tx_buf;
    callback_t      m_tx_call;

    uint32_t        m_rx_count;
    uint32_t        m_rx_size;
    uint8_t         *m_rx_buf;
    callback_t      m_rx_call;

    uint32_t transmit_blocked(uint8_t* data, uint32_t size, uint32_t timeout);
    uint32_t transmit_IT(uint8_t* data, uint32_t size);

    uint32_t recieve_blocked(uint8_t* data, uint32_t size, uint32_t timeout);
    uint32_t recieve_IT(uint8_t* data, uint32_t size);

    void callback_tx_8bit();
    void callback_tx_16bit();

    uint32_t close_tx_ISR();
};

extern STM32_SPI spi1;
extern STM32_SPI spi2;
extern STM32_SPI spi3;
extern STM32_SPI spi4;
extern STM32_SPI spi5;
extern STM32_SPI spi6;

#endif // STM32_SPI_H
