#ifndef LED_H
#define LED_H

enum class ELED
{
    ERROR,
    STOP,
    RUN,
};

class LED
{
public:
    static void led_ON(ELED led);
    static void led_blink(ELED led);
    static void led_OFF(ELED led);
private:
    static void run_command(const char *cmd);
};

#endif // LED_H
