#ifndef GPIO_H
#define GPIO_H

extern "C" {
#include "gpio_lib.h"
}

#define GPIO_COUNT 1

#define PIN_CS_MODULES   SUNXI_GPE(01)
#define PIN_RE_MODULES   SUNXI_GPE(02)
#define PIN_CS_COMMS     SUNXI_GPE(03)
#define PIN_RE_COMMS     SUNXI_GPE(04)

class GPIO
{
public:
    static void init();

    static void inline CS_modules_set() { sunxi_gpio_output(PIN_CS_MODULES, 1); }
    static void inline CS_modules_unset() { sunxi_gpio_output(PIN_CS_MODULES, 0); }

    static void inline RE_modules_set() { sunxi_gpio_output(PIN_RE_MODULES, 1); }
    static void inline RE_modules_unset() { sunxi_gpio_output(PIN_RE_MODULES, 0); }

    static void inline CS_comms_set() { sunxi_gpio_output(PIN_CS_COMMS, 1); }
    static void inline CS_comms_unset() { sunxi_gpio_output(PIN_CS_COMMS, 0); }

    static void inline RE_comms_set() { sunxi_gpio_output(PIN_RE_COMMS, 1); }
    static void inline RE_comms_unset() { sunxi_gpio_output(PIN_RE_COMMS, 0); }
};

#endif // GPIO_H
