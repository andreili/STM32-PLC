#include "plc_platform.h"

#ifdef PLATFORM_STM32
#else

#include <ctime>
#include <fstream>

struct period_info 
{
    struct timespec next_period;
    long period_ns;
};

static void inc_period(struct period_info *pinfo) 
{
    pinfo->next_period.tv_nsec += pinfo->period_ns;

    while (pinfo->next_period.tv_nsec >= 1000000000) 
    {
            /* timespec nsec overflow */
            pinfo->next_period.tv_sec++;
            pinfo->next_period.tv_nsec -= 1000000000;
    }
}

void rt_delay_ms(uint32_t ms)
{
    struct period_info info;
    
    clock_gettime(CLOCK_MONOTONIC, &(info.next_period));
    info.period_ns = ms * 1000000;
    inc_period(&info);
    /* for simplicity, ignoring possibilities of signal wakes */
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &info.next_period, NULL);
}

int32_t load_file(char* fn, uint8_t* buf)
{
    std::ifstream ifs(fn);
    if (ifs)
    {
        ifs.seekg (0, ifs.end);
        int32_t size = ifs.tellg();
        ifs.seekg (0, ifs.beg);

        ifs.read((char*)buf, size);
        ifs.close();

        return size;
    }
    else
        return -1;
}

#endif
