#include "plc_bus.h" 

void PLCBus::init(bool is_com)
{
    m_bus_com = is_com;
    unselect();
    init_CS();
    select();
}

void PLCBus::init_CS()
{
#ifdef PLATFORM_STM32
    if (m_bus_com)
    {
        m_spi = &spi6;
        PLC_MOD_COM_SEL_GPIO.set_config(PLC_MOD_COM_SEL_PIN, GPIO_MODE_OUTPUT_PP, 0,
                                        GPIO_SPEED_FREQ_LOW, GPIO_PULLUP);
    }
    else
    {
        m_spi = &spi5;
        PLC_MOD_EXT_SEL_GPIO.set_config(PLC_MOD_EXT_SEL_PIN, GPIO_MODE_OUTPUT_PP, 0,
                                        GPIO_SPEED_FREQ_LOW, GPIO_PULLUP);
    }
#else
#warning Init CS GPIO
#endif
}

void PLCBus::unselect()
{
#ifdef PLATFORM_STM32
    if (m_bus_com)
        PLC_MOD_COM_SEL_GPIO.pin_ON(PLC_MOD_COM_SEL_PIN);
    else
        PLC_MOD_EXT_SEL_GPIO.pin_ON(PLC_MOD_EXT_SEL_PIN);
#else
#warning SPI CS
#endif
}

void PLCBus::select()
{
#ifdef PLATFORM_STM32
    if (m_bus_com)
        PLC_MOD_COM_SEL_GPIO.pin_OFF(PLC_MOD_COM_SEL_PIN);
    else
        PLC_MOD_EXT_SEL_GPIO.pin_OFF(PLC_MOD_EXT_SEL_PIN);
#else
#warning SPI CS
#endif
}

uint32_t PLCBus::transmit(uint8_t* tx_data, uint32_t tx_size, uint32_t timeout)
{
#ifdef PLATFORM_STM32
    return m_spi->transmit(tx_data, tx_size, TXRX_MODE::DIRECT, timeout);
#else
#warning SPI transmit
    return 0;
#endif
}

uint32_t PLCBus::transmit_recieve(uint8_t* tx_data, uint8_t* rx_data, uint32_t tx_size, uint32_t timeout)
{
#ifdef PLATFORM_STM32
    return m_spi->transmit_recieve(tx_data, rx_data, tx_size, TXRX_MODE::DIRECT, timeout);
#else
#warning SPI transmit+recieve
    return 0;
#endif
}
