#include "gpio.h"
#include <cstdlib>
#include <string>

void GPIO::init()
{
    if (sunxi_gpio_init())
    {
        return;
    }

    sunxi_gpio_set_cfgpin(PIN_CS_MODULES, SUNXI_GPIO_OUTPUT);
    sunxi_gpio_output(PIN_CS_MODULES, 0);
}
