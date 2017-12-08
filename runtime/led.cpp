#include "led.h"
#include <cstdlib>
#include <string>

const char* led_names[] = {"led_error", "led_stop", "led_run"};

void LED::led_ON(ELED led)
{
    const char* name = led_names[(int)led];
    run_command((std::string("echo 255 > /sys/class/leds/") + name + "/brightness").c_str());
}

void LED::led_blink(ELED led)
{
    const char* name = led_names[(int)led];
    run_command((std::string("echo 500 > /sys/class/leds/") + name + "/delay_on").c_str());
    run_command((std::string("echo 500 > /sys/class/leds/") + name + "/delay_off").c_str());
    run_command((std::string("echo timer > /sys/class/leds/") + name + "/trigger").c_str());
    run_command((std::string("echo 255 > /sys/class/leds/") + name + "/brightness").c_str());
}

void LED::led_OFF(ELED led)
{
    const char* name = led_names[(int)led];
    run_command((std::string("echo 0 > /sys/class/leds/") + name + "/brightness").c_str());
}

void LED::run_command(const char* cmd)
{
    std::system(cmd);
}
