#ifndef PLC_BUS_H
#define PLC_BUS_H

#include <inttypes.h>

#ifdef PLATFORM_STM32
class STM32_SPI;
#endif

class PLCBus
{
public:
    void init(bool is_com);
    
    void unselect();
    void select();
    
    uint32_t transmit(uint8_t* tx_data, uint32_t tx_size, uint32_t timeout);
    uint32_t transmit_recieve(uint8_t* tx_data, uint8_t* rx_data, uint32_t tx_size, uint32_t timeout);
private:
    bool        m_bus_com;
#ifdef PLATFORM_STM32
    STM32_SPI   *m_spi;
#endif
    void init_CS();
};

#endif
