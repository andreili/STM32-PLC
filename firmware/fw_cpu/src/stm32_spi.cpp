#include "stm32_spi.h"

#define SPI_DEFAULT_TIMEOUT             100U

#define SPI_MODE_SLAVE                  ((uint32_t)0x00000000U)
#define SPI_MODE_MASTER                 (SPI_CR1_MSTR | SPI_CR1_SSI)

#define SPI_DIRECTION_2LINES            ((uint32_t)0x00000000U)
#define SPI_DIRECTION_2LINES_RXONLY     SPI_CR1_RXONLY
#define SPI_DIRECTION_1LINE             SPI_CR1_BIDIMODE

#define SPI_DATASIZE_8BIT               ((uint32_t)0x00000000U)
#define SPI_DATASIZE_16BIT              SPI_CR1_DFF

#define SPI_POLARITY_LOW                ((uint32_t)0x00000000U)
#define SPI_POLARITY_HIGH               SPI_CR1_CPOL

#define SPI_PHASE_1EDGE                 ((uint32_t)0x00000000U)
#define SPI_PHASE_2EDGE                 SPI_CR1_CPHA

#define SPI_NSS_SOFT                    SPI_CR1_SSM
#define SPI_NSS_HARD_INPUT              ((uint32_t)0x00000000U)
#define SPI_NSS_HARD_OUTPUT             ((uint32_t)0x00040000U)

#define SPI_BAUDRATEPRESCALER_2         ((uint32_t)0x00000000U)
#define SPI_BAUDRATEPRESCALER_4         ((uint32_t)0x00000008U)
#define SPI_BAUDRATEPRESCALER_8         ((uint32_t)0x00000010U)
#define SPI_BAUDRATEPRESCALER_16        ((uint32_t)0x00000018U)
#define SPI_BAUDRATEPRESCALER_32        ((uint32_t)0x00000020U)
#define SPI_BAUDRATEPRESCALER_64        ((uint32_t)0x00000028U)
#define SPI_BAUDRATEPRESCALER_128       ((uint32_t)0x00000030U)
#define SPI_BAUDRATEPRESCALER_256       ((uint32_t)0x00000038U)

#define SPI_FIRSTBIT_MSB                ((uint32_t)0x00000000U)
#define SPI_FIRSTBIT_LSB                SPI_CR1_LSBFIRST

#define SPI_TIMODE_DISABLE             ((uint32_t)0x00000000U)
#define SPI_TIMODE_ENABLE              SPI_CR2_FRF

#define SPI_CRCCALCULATION_DISABLE     ((uint32_t)0x00000000U)
#define SPI_CRCCALCULATION_ENABLE      SPI_CR1_CRCEN

#define SPI_IT_TXE                      SPI_CR2_TXEIE
#define SPI_IT_RXNE                     SPI_CR2_RXNEIE
#define SPI_IT_ERR                      SPI_CR2_ERRIE

#define SPI_CheckFlag_BSY(tout) \
    WAIT_TIMEOUT(((m_spi->SR & SPI_FLAG_BSY) != SPI_FLAG_BSY), tout)

#ifdef STM32_USE_SPI1
STM32_SPI spi1;
#endif
#ifdef STM32_USE_SPI2
STM32_SPI spi2;
#endif
#ifdef STM32_USE_SPI3
STM32_SPI spi3;
#endif
#ifdef STM32_USE_SPI4
STM32_SPI spi4;
#endif
#ifdef STM32_USE_SPI5
STM32_SPI spi5;
#endif
#ifdef STM32_USE_SPI6
STM32_SPI spi6;
#endif

void STM32_SPI::init_all()
{
    #ifdef STM32_USE_SPI1
    spi1.m_spi = SPI1;
    #endif
    #ifdef STM32_USE_SPI2
    spi2.m_spi = SPI2;
    #endif
    #ifdef STM32_USE_SPI3
    spi3.m_spi = SPI3;
    #endif
    #ifdef STM32_USE_SPI4
    spi4.m_spi = SPI4;
    #endif
    #ifdef STM32_USE_SPI5
    spi5.m_spi = SPI5;
    #endif
    #ifdef STM32_USE_SPI6
    spi6.m_spi = SPI6;
    #endif
}

void STM32_SPI::init()
{
    switch ((uint32_t)m_spi)
    {
    #ifdef STM32_USE_SPI1
    case SPI1_BASE:
        break;
    #endif
    #ifdef STM32_USE_SPI2
    case SPI2_BASE:
        break;
    #endif
    #ifdef STM32_USE_SPI3
    case SPI3_BASE:
        break;
    #endif
    #ifdef STM32_USE_SPI4
    case SPI4_BASE:
        break;
    #endif
    #ifdef STM32_USE_SPI5
    case SPI5_BASE:
        STM32_RCC::enable_clk_SPI5();
        gpiof.set_config(GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9,
                         GPIO_MODE_AF_PP, GPIO_AF5_SPI5,
                         GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
        STM32_NVIC::enable_and_set_prior_IRQ(SPI5_IRQn, 0, 0);

        disable();

        WRITE_REG(m_spi->CR1, (STM32_SPI5_MODE | STM32_SPI5_DIRECTION | STM32_SPI5_DATA_SIZE |
                  STM32_SPI5_CLK_POL | STM32_SPI5_CLK_PH | (STM32_SPI5_NSS & SPI_CR1_SSM) |
                  STM32_SPI5_BRP | STM32_SPI5_FIRST_BIT  | STM32_SPI5_CRC));
        WRITE_REG(m_spi->CR2, (((STM32_SPI5_NSS >> 16U) & SPI_CR2_SSOE) | STM32_SPI5_TI_MODE));

        m_mode = STM32_SPI5_MODE;
        m_direction = STM32_SPI5_DIRECTION;
        m_crc_calc = STM32_SPI5_CRC;
        m_data_size = STM32_SPI5_DATA_SIZE;
        break;
    #endif
    #ifdef STM32_USE_SPI6
    case SPI6_BASE:
        STM32_RCC::enable_clk_SPI6();
        gpiog.set_config(GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14,
                         GPIO_MODE_AF_PP, GPIO_AF5_SPI6,
                         GPIO_SPEED_FREQ_VERY_HIGH, GPIO_PULLUP);
        STM32_NVIC::enable_and_set_prior_IRQ(SPI6_IRQn, 0, 0);

        disable();

        WRITE_REG(m_spi->CR1, (STM32_SPI6_MODE | STM32_SPI6_DIRECTION | STM32_SPI6_DATA_SIZE |
                  STM32_SPI6_CLK_POL | STM32_SPI6_CLK_PH | (STM32_SPI6_NSS & SPI_CR1_SSM) |
                  STM32_SPI6_BRP | STM32_SPI6_FIRST_BIT  | STM32_SPI6_CRC));
        WRITE_REG(m_spi->CR2, (((STM32_SPI5_NSS >> 16U) & SPI_CR2_SSOE) | STM32_SPI6_TI_MODE));

        m_mode = STM32_SPI6_MODE;
        m_direction = STM32_SPI6_DIRECTION;
        m_crc_calc = STM32_SPI6_CRC;
        m_data_size = STM32_SPI6_DATA_SIZE;
        break;
    #endif
    }

    #if defined(SPI_I2SCFGR_I2SMOD)
    /* Activate the SPI mode (Make sure that I2SMOD bit in I2SCFGR register is reset) */
    CLEAR_BIT(m_spi->I2SCFGR, SPI_I2SCFGR_I2SMOD);
    #endif /* SPI_I2SCFGR_I2SMOD */

    m_busy = false;
}

void STM32_SPI::irq()
{
    uint32_t itsource = m_spi->CR2;
    uint32_t itflag = m_spi->SR;

    /* SPI in mode Receiver ----------------------------------------------------*/
    if (((itflag & SPI_FLAG_OVR) == RESET) &&
        ((itflag & SPI_FLAG_RXNE) != RESET) && ((itsource & SPI_IT_RXNE) != RESET))
    {
        (this->*m_rx_call)();
        return;
    }

    /* SPI in mode Transmitter -------------------------------------------------*/
    if (((itflag & SPI_FLAG_TXE) != RESET) &&
        ((itsource & SPI_IT_TXE) != RESET))
    {
        (this->*m_tx_call)();
        return;
    }
}

uint32_t STM32_SPI::wait_busy(uint32_t timeout)
{
    WAIT_TIMEOUT(m_busy, timeout);
    return STM32_RESULT_OK;
}

uint32_t STM32_SPI::transmit(uint8_t* data, uint32_t size, TXRX_MODE mode, uint32_t timeout)
{
    switch (mode)
    {
    case TXRX_MODE::DIRECT:
        return transmit_blocked(data, size, timeout);
        break;
    case TXRX_MODE::INTERRUPT:
        return transmit_IT(data, size);
        break;
    case TXRX_MODE::DMA:
        //return transmit_blocked(data, size, timeout);
        break;
    }
    return STM32_RESULT_FAIL;
}

uint32_t STM32_SPI::recieve(uint8_t* data, uint32_t size, TXRX_MODE mode, uint32_t timeout)
{
    switch (mode)
    {
    case TXRX_MODE::DIRECT:
        return recieve_blocked(data, size, timeout);
        break;
    case TXRX_MODE::INTERRUPT:
        return recieve_IT(data, size);
        break;
    case TXRX_MODE::DMA:
        //return recieve_blocked(data, size, timeout);
        break;
    }
    return STM32_RESULT_FAIL;
}

uint32_t STM32_SPI::transmit_recieve(uint8_t* tx_buf, uint8_t* rx_buf, uint32_t size, TXRX_MODE mode, uint32_t timeout)
{
    (void)(mode);
    uint32_t tout = timeout + STM32_SYSTICK::get_tick();

    m_rx_buf = rx_buf;
    m_rx_count = size;
    m_rx_size = size;

    m_tx_buf = tx_buf;
    m_tx_count = size;
    m_tx_size = size;

    m_tx_call = nullptr;
    m_rx_call = nullptr;

    if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
        crc_reset();

    if (!is_enabled())
        enable();

    uint32_t txallowed = 1;

    if (m_data_size == SPI_DATASIZE_16BIT)
    {
        /* Transmit and Receive data in 16 Bit mode */
        if ((m_mode == SPI_MODE_SLAVE) || (m_tx_count == 0x01))
        {
            m_spi->DR = *((uint16_t *)m_tx_buf);
            m_tx_buf += sizeof(uint16_t);
            --m_tx_count;
        }

        while ((m_tx_count) || (m_rx_count))
        {
            /* Check TXE flag */
            if ((txallowed) && (m_tx_count >0) && (get_flag(SPI_FLAG_TXE)))
            {
                m_spi->DR = *((uint16_t *)m_tx_buf);
                m_tx_buf += sizeof(uint16_t);
                --m_tx_count;
                /* Next Data is a reception (Rx). Tx not allowed */
                txallowed = 0;
            }

            if ((m_tx_count == 0) && (m_crc_calc == SPI_CRCCALCULATION_ENABLE))
                enable_CRC();

            /* Check RXNE flag */
            if ((m_rx_count > 0) && (get_flag(SPI_FLAG_RXNE)))
            {
                *((uint16_t*)m_rx_buf) = m_spi->DR;
                m_rx_buf += sizeof(uint16_t);
                --m_rx_count;
                /* Next Data is a Transmission (Tx). Tx is allowed */
                txallowed = 1U;
            }

            if (STM32_SYSTICK::get_tick() >= tout)
                return STM32_RESULT_TIMEOUT;
        }
    }
    else
    {
        /* Transmit and Receive data in 8 Bit mode */
        if ((m_mode == SPI_MODE_SLAVE) || (m_tx_count == 0x01))
        {
            *((__IO uint8_t*)&m_spi->DR) = *((uint8_t *)m_tx_buf);
            m_tx_buf += sizeof(uint8_t);
            --m_tx_count;
        }

        while ((m_tx_count) || (m_rx_count))
        {
            /* Check TXE flag */
            if ((txallowed) && (m_tx_count >0) && (get_flag(SPI_FLAG_TXE)))
            {
                *((__IO uint8_t*)&m_spi->DR) = *((uint16_t *)m_tx_buf);
                m_tx_buf += sizeof(uint8_t);
                --m_tx_count;
                /* Next Data is a reception (Rx). Tx not allowed */
                txallowed = 0;
            }

            if ((m_tx_count == 0) && (m_crc_calc == SPI_CRCCALCULATION_ENABLE))
                enable_CRC();

            /* Check RXNE flag */
            if ((m_rx_count > 0) && (get_flag(SPI_FLAG_RXNE)))
            {
                *((uint8_t*)m_rx_buf) = *((__IO uint8_t*)&m_spi->DR);
                m_rx_buf += sizeof(uint8_t);
                --m_rx_count;
                /* Next Data is a Transmission (Tx). Tx is allowed */
                txallowed = 1U;
            }

            if (STM32_SYSTICK::get_tick() >= tout)
                return STM32_RESULT_TIMEOUT;
        }
    }

    /* Read CRC from DR to close CRC calculation process */
    if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
    {
        /* Read the latest data */
        WAIT_TIMEOUT(((m_spi->SR & SPI_FLAG_RXNE) == SPI_FLAG_RXNE), timeout);

        /* Read CRC to Flush DR and RXNE flag */
        uint32_t tmp = m_spi->DR;
        (void)(tmp);
    }

    /* Check if CRC error occurred */
    if (get_flag(SPI_FLAG_CRCERR))
        clear_CRCerror();

    /* Wait until TXE flag */
    WAIT_TIMEOUT(((m_spi->SR & SPI_FLAG_TXE) == SPI_FLAG_TXE), timeout);

    SPI_CheckFlag_BSY(timeout);

    if (m_mode == SPI_DIRECTION_2LINES)
        clear_ovrflag();
}

uint32_t STM32_SPI::transmit_blocked(uint8_t* data, uint32_t size, uint32_t timeout)
{
    if ((data == nullptr) || (size == 0))
        return STM32_RESULT_FAIL;

    uint32_t tout = timeout + STM32_SYSTICK::get_tick();

    m_tx_count = size;
    m_tx_size = size;
    m_tx_buf = data;

    m_rx_count = 0;
    m_rx_size = 0;
    m_rx_buf = nullptr;

    m_tx_call = nullptr;
    m_rx_call = nullptr;

    if (m_direction == SPI_DIRECTION_1LINE)
        tx_1line();

    if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
        crc_reset();

    if (!is_enabled())
        enable();

    if (m_data_size == SPI_DATASIZE_16BIT)
    {
        /* Transmit data in 16 Bit mode */
        if ((m_mode == SPI_MODE_SLAVE) || (m_tx_count == 0x01))
        {
            m_spi->DR = *data;
            data += sizeof(uint16_t);
            --m_tx_count;
        }

        while (m_tx_count)
        {
            if (get_flag(SPI_FLAG_TXE))
            {
                m_spi->DR = *data;
                data += sizeof(uint16_t);
                --m_tx_count;
            }
            else
            {
                if (STM32_SYSTICK::get_tick() >= tout)
                    return STM32_RESULT_TIMEOUT;
            }
        }
    }
    else
    {
        /* Transmit data in 8 Bit mode */
        if ((m_mode == SPI_MODE_SLAVE) || (m_tx_count == 0x01))
        {
           m_spi->DR = *data;
            data += sizeof(uint8_t);
            --m_tx_count;
        }

        while (m_tx_count)
        {
            if (get_flag(SPI_FLAG_TXE))
            {
                m_spi->DR = *data;
                data += sizeof(uint8_t);
                --m_tx_count;
            }
            else
            {
                if (STM32_SYSTICK::get_tick() >= tout)
                    return STM32_RESULT_TIMEOUT;
            }
        }
    }

    /* Wait until TXE flag */
    WAIT_TIMEOUT(((m_spi->SR & SPI_FLAG_TXE) == SPI_FLAG_TXE), timeout);

    SPI_CheckFlag_BSY(timeout);

    if (m_mode == SPI_DIRECTION_2LINES)
        clear_ovrflag();

    if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
    {
        m_spi->CR1 |= SPI_CR1_CRCNEXT;
    }

    return STM32_RESULT_OK;
}

uint32_t STM32_SPI::transmit_IT(uint8_t* data, uint32_t size)
{
    if (m_busy)
        return STM32_RESULT_BUSY;

    m_tx_count = size;
    m_tx_size = size;

    m_tx_count = size;
    m_tx_size = size;
    m_tx_buf = data;

    m_rx_count = 0;
    m_rx_size = 0;
    m_rx_buf = nullptr;
    m_rx_call = nullptr;

    if (m_data_size == SPI_DATASIZE_8BIT)
        m_tx_call = &STM32_SPI::callback_tx_8bit;
    else
        m_tx_call = &STM32_SPI::callback_tx_16bit;

    if (m_direction == SPI_DIRECTION_1LINE)
        tx_1line();

    if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
        crc_reset();

    if (m_direction == SPI_DIRECTION_2LINES)
        enable_IT(SPI_IT_TXE);
    else
        enable_IT(SPI_IT_TXE | SPI_IT_ERR);

    if (!is_enabled())
        enable();

    return STM32_RESULT_OK;
}

uint32_t STM32_SPI::recieve_blocked(uint8_t* data, uint32_t size, uint32_t timeout)
{
    if ((data == nullptr) || (size == 0))
        return STM32_RESULT_FAIL;

    uint32_t tout = timeout + STM32_SYSTICK::get_tick();

    m_tx_count = 0;
    m_tx_size = 0;
    m_tx_buf = nullptr;

    m_rx_count = size;
    m_rx_size = size;
    m_rx_buf = data;

    if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
        crc_reset();

    if (m_direction == SPI_DIRECTION_1LINE)
        rx_1line();

    if (!is_enabled())
        enable();

    if (m_data_size == SPI_DATASIZE_16BIT)
    {
        /* Transmit data in 16 Bit mode */
        while (m_rx_count)
        {
            if (get_flag(SPI_FLAG_RXNE))
            {
                *data = m_spi->DR;
                data += sizeof(uint16_t);
                --m_rx_count;
            }
            else
            {
                if (STM32_SYSTICK::get_tick() >= tout)
                    return STM32_RESULT_TIMEOUT;
            }
        }
    }
    else
    {
        /* Transmit data in 8 Bit mode */
        while (m_rx_count)
        {
            if (get_flag(SPI_FLAG_TXE))
            {
                *data = m_spi->DR;
                data += sizeof(uint8_t);
                --m_rx_count;
            }
            else
            {
                if (STM32_SYSTICK::get_tick() >= tout)
                    return STM32_RESULT_TIMEOUT;
            }
        }
    }

    if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
    {
        /* freeze the CRC before the latest data */
        enable_CRC();

        /* Read the latest data */
        WAIT_TIMEOUT(((m_spi->SR & SPI_FLAG_RXNE) == SPI_FLAG_RXNE), timeout);

        if (m_data_size == SPI_DATASIZE_16BIT)
            /* Receive last data in 16 Bit mode */
            *((uint16_t*)data) = m_spi->DR;
        else
            /* Receive last data in 8 Bit mode */
            *data = m_spi->DR;

        /* Wait the CRC data */
        WAIT_TIMEOUT(((m_spi->SR & SPI_FLAG_RXNE) == SPI_FLAG_RXNE), timeout);

        /* Read CRC to Flush DR and RXNE flag */
        uint32_t tmp = m_spi->DR;
        (void)(tmp);
    }

    if ((m_mode == SPI_MODE_MASTER) &&
        ((m_direction == SPI_DIRECTION_1LINE) || (m_direction == SPI_DIRECTION_2LINES_RXONLY)))
        disable();

    if (get_flag(SPI_FLAG_CRCERR))
        clear_CRCerror();

    return STM32_RESULT_OK;
}

uint32_t STM32_SPI::recieve_IT(uint8_t* data, uint32_t size)
{
    //
}

void STM32_SPI::callback_tx_8bit()
{
    m_spi->DR = (*m_tx_buf++);
    if ((--m_tx_count) == 0)
    {
        if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
            enable_CRC();
        close_tx_ISR();
    }
}

void STM32_SPI::callback_tx_16bit()
{
    m_spi->DR = *((uint16_t *)m_tx_buf);
    m_tx_buf += sizeof(uint16_t);
    if ((--m_tx_count) == 0)
    {
        if (m_crc_calc == SPI_CRCCALCULATION_ENABLE)
            enable_CRC();
        close_tx_ISR();
    }
}

uint32_t STM32_SPI::close_tx_ISR()
{
    //uint32_t count = SPI_DEFAULT_TIMEOUT * (STM32_RCC::get_HCLK_freq() / 24 / 1000);
    //uint32_t tickstart = STM32_SYSTICK::get_tick();

    do
    {
    } while (get_TXE() == RESET);

    disable_IT(SPI_IT_TXE | SPI_IT_ERR);

    SPI_CheckFlag_BSY(SPI_DEFAULT_TIMEOUT);

    if (m_direction == SPI_DIRECTION_2LINES)
        clear_ovrflag();

    ///TODO HAL_SPI_TxCpltCallback
    return STM32_RESULT_OK;
}

#ifdef STM32_USE_SPI1
void ISR::SPI1_IRQ()
{
    spi1.irq();
}
#endif

#ifdef STM32_USE_SPI2
void ISR::SPI2_IRQ()
{
    spi2.irq();
}
#endif

#ifdef STM32_USE_SPI3
void ISR::SPI3_IRQ()
{
    spi3.irq();
}
#endif

#ifdef STM32_USE_SPI4
void ISR::SPI4_IRQ()
{
    spi4.irq();
}
#endif

#ifdef STM32_USE_SPI5
void ISR::SPI5_IRQ()
{
    spi5.irq();
}
#endif

#ifdef STM32_USE_SPI6
void ISR::SPI6_IRQ()
{
    spi6.irq();
}
#endif
