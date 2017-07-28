#include "sddriver.h"

SDDriver sd_driver;

#define CHECK_CARD_DETECT() \
    (STM32_FATFS_CARD_DETECT_PORT.pin_read(STM32_FATFS_CARD_DETECT_PIN) == RESET)

void SDDriver::init_gpio()
{
    STM32_FATFS_CARD_DETECT_PORT.set_config(STM32_FATFS_CARD_DETECT_PIN,
                                            GPIO_MODE_INPUT, 0,
                                            GPIO_SPEED_FREQ_LOW, GPIO_NOPULL);
}

uint8_t SDDriver::init(uint8_t)
{
    uint8_t sd_state = SD_OK;
    m_state = STA_NOINIT;
    if (!CHECK_CARD_DETECT())
        return SD_ERROR;
    sd_state = STM32_SD::init();
    #ifdef STM32_FATFS_BUS_4BITS
    if (sd_state == SD_OK)
    {
        //if (HAL_SD_WideBusOperation_Config(&hsd, SDIO_BUS_WIDE_4B) != SD_OK)
            //sd_state = SD_ERROR;
    }
    #endif
    return sd_state;
}

uint8_t SDDriver::status(uint8_t)
{
    return 0;
}

uint8_t SDDriver::read(uint8_t, const uint8_t*, uint32_t, uint32_t)
{
    return 0;
}

#if _USE_WRITE == 1
uint8_t SDDriver::write(uint8_t, const uint8_t*, uint32_t, uint32_t)
{
    return 0;
}

#endif
#if _USE_IOCTL == 1
uint8_t SDDriver::ioctl(uint8_t, uint8_t, void*)
{
    return 0;
}

#endif
