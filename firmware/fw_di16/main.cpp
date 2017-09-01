#include "stm32_gpio.h"

int main()
{
    gpioa.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                     GPIO_PIN_4 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                     GPIO_PIN_11, GPIO_MODE_INPUT, 0, GPIO_SPEED_FREQ_MEDIUM,
                     GPIO_PULLUP);
    gpiob.set_config(GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
                     GPIO_MODE_INPUT, 0, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);
    gpioc.set_config(GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
                     GPIO_MODE_INPUT, 0, GPIO_SPEED_FREQ_MEDIUM, GPIO_PULLUP);

    //UART::send_str("Hello world!~\n\r");
	while (1)
	{}
}

void Error_Handler()
{
    while (1) {}
}
