#include "plc_control.h"
#ifdef PLATFORM_STM32
    #include "stm32_inc.h"
    #ifdef STM32_FATFS_USE
    #include "fatfs.h"
    #include "ff.h"
    #include "sddriver.h"
    #endif
    #include "memmanager.h"
    #include "xprintf.h"
#else
    #include <cstdio>
    #include <limits.h>
    #include <pthread.h>
    #include <sched.h>
    #include <sys/mman.h>
    #include "plc_platform.h"
#endif
#include "plc_mod.h"
#include "plc_io.h"
#include <stdarg.h>
#include "plc_state.h"
#include "plc_fw.h"

uint8_t PLC_CONTROL::m_state_run;
uint8_t PLC_CONTROL::m_state_stop;
uint8_t PLC_CONTROL::m_state_rs_blink;
uint8_t PLC_CONTROL::m_state_error;
uint8_t PLC_CONTROL::m_state_fault;
uint8_t PLC_CONTROL::m_state_com_fault;
uint8_t PLC_CONTROL::m_state_initialized;
char PLC_CONTROL::m_text_buf[PLC_TEXT_BUF_SIZE];

#ifdef PLATFORM_STM32
#ifdef STM32_FATFS_USE
FATFS SDFatFs;
extern char SD_path[4];
#endif

void xfunc_out(unsigned char ch)
{
    uart3.send_char(ch);
}
#endif

void PLC_CONTROL::init()
{
    m_state_run = 0;
    m_state_stop = 1;
    m_state_rs_blink = 0;
    m_state_error = 0;
    m_state_fault = 0;
    m_state_com_fault = 0;
    m_state_initialized = 0;
}

void PLC_CONTROL::init_hw()
{
#ifdef PLATFORM_STM32
    plc_mod_com.init(&spi6, true);
    plc_mod_ext.init(&spi5, false);
#else
    plc_mod_com.init(true);
    plc_mod_ext.init(false);
#endif

    plc_mod_com.find_modules();
    plc_mod_ext.find_modules();
}

#ifdef PLATFORM_STM32
void xvprintf(const char* fmt, va_list arp);
extern char* outptr;
#endif

void PLC_CONTROL::print_message(const char* fmt, ...)
{
    va_list arp;

#ifdef PLATFORM_STM32
    outptr = m_text_buf;
#endif

    va_start(arp, fmt);
#ifdef PLATFORM_STM32
    xvprintf(fmt, arp);
#else
    vsnprintf(m_text_buf, PLC_TEXT_BUF_SIZE, fmt, arp);
#endif
    va_end(arp);

#ifdef PLATFORM_STM32
    *outptr = 0;
    outptr = 0;
    uart3.send_str(m_text_buf, TXRX_MODE::INTERRUPT);
#else
    printf("%s", m_text_buf);
#endif
}

void PLC_CONTROL::init_seq()
{
    set_rs_blink(1);

#ifdef PLATFORM_STM32
    scheck_RTC();
#endif
    PLC_STATE::init();
    plc_datetime_t* start_dt = PLC_STATE::get_start_dt();

#ifdef PLATFORM_STM32
    print_message("\033[2J\n+-----------------------------+\n"
                  "|          STM32 PLC          |\n"
                  "|         System info         |\n"
                  "| CPU Speed: %03UMHz           |\n"
                  "| RAM size: 192kb + %03UMb     |\n"
                  "| Cur. date %02U.%02U.%02U %02U:%02U:%02U |\n"
                  "+-----------------------------+\n",
                  STM32_PLLN, STM32_SDRAM_SIZE_MB,
#else
    print_message("\033[2J\n+-----------------------------+\n"
                  "|    STM32 PLC (emulator)     |\n"
                  "| Cur. date %02i.%02i.%02i %02i:%02i:%02i |\n"
                  "+-----------------------------+\n",
#endif
                  start_dt->day, start_dt->month, start_dt->year,
                  start_dt->hour, start_dt->min, start_dt->sec);

#ifdef PLATFORM_STM32
    #if defined (DATA_IN_ExtSDRAM)
    test_RAM(true);
    test_RAM_speed();
    #endif

    MemManager::init();

    #ifdef STM32_FATFS_USE
    init_fs();
    #endif
#endif

    set_initialized(1);
    print_message("+-----------------------------+\n"
                   "Initializing PLC hardware\n");
    init_hw();
    print_message("+-----------------------------+\n");
    
    if (!plc_fw.load())
        Error_Handler();

    set_stop(0);
    set_run(1);
    set_rs_blink(1);
}

#ifndef PLATFORM_STM32
void *thread_func(void *data);
#endif

void PLC_CONTROL::main()
{
#ifndef PLATFORM_STM32
    RT_INIT_THREAD();
}
    
void *thread_func(void *data)
{
#endif
    PLC_CONTROL::print_message("Start main cycle\n");
    int iteration = 0;
    while (1)
    {
        PLC_STATE::update_ct();
        plc_datetime_t* dt = PLC_STATE::get_current_dt();
         //PLC_CONTROL::print_message("\r\t%02i:%02i", prev_sec, dt->sec);
        if (++iteration % 1000 == 0)
        {
#ifdef PLATFORM_STM32
            PLC_CONTROL::print_message("\r\t(%02U:%02U:%02U:%03U) Test iteration: %U (%04Umsec)",
#else
            PLC_CONTROL::print_message("\r\t(%02i:%02i:%02i:%03i) Test iteration: %i (%04imsec)",
#endif
                        dt->hour, dt->min, dt->sec, dt->msec,
                        iteration, PLC_STATE::get_last_cycle_time());
        }

        plc_fw.execute_OB1();
    }
}

void PLC_CONTROL::fault_state_on()
{
    PLC_CONTROL::set_run(0);
    PLC_CONTROL::set_fault(1);
    PLC_CONTROL::set_stop(1);
}

#ifdef PLATFORM_STM32
void PLC_CONTROL::scheck_RTC()
{
    STM32_RTC_Date date;
    STM32_RTC::get_date(&date, ERTCFormat::BIN);
    if (date.Year == 0)
    {
        date.Year = 17;
        date.Month = RTC_MONTH_SEPTEMBER;
        date.Date = 23;
        date.WeekDay = RTC_WEEKDAY_SATURDAY;
        STM32_RTC::set_date(&date, ERTCFormat::BIN);

        STM32_RTC_Time time;
        time.Hours = 23;
        time.Minutes = 01;
        time.Seconds = 0;
        time.TimeFormat = 0;
        STM32_RTC::set_time(&time, ERTCFormat::BIN);
    }
}

void PLC_CONTROL::init_fs()
{
    if (SDDriver::is_card_present())
    {
        FAT_FS::init();
        print_message("+-----------------------------+\n"
                      "Pass to mount SD-card\n");
        while (f_mount(&SDFatFs, (TCHAR const*)SD_path, 1) != FR_OK)
        {
            print_message("\rUnable to mount FAT partition! Check SD-card!");
            STM32_SYSTICK::delay(300);
        }
        print_message("FAT partition mounted\n");
    }
    else
        print_message("+-----------------------------+\n"
                      "SD-card was not detected, skipping the initialization of FATFs\n");
}

void PLC_CONTROL::test_RAM(bool print_debug)
{
    if (STM32_SDRAM::run_tests(SDRAM_BASE_BANK1,
                               (STM32_SDRAM_SIZE_MB * 1024 * 1024),
                               print_debug) != STM32_RESULT_OK)
        Error_Handler();
}

void PLC_CONTROL::test_RAM_speed()
{
    #ifdef MEM_SPEED_TEST
    PLC_CONTROL::print_message("+-----------------------------+\n"
                               "Test RAM speeds\n");
    uint8_t buf_int[TEST_SIZE];
    test_mem_speed(buf_int, "Internal RAM");
    test_mem_speed((uint8_t*)SDRAM_BASE_BANK1, "External RAM");
    #endif
}
#endif

void Error_Handler()
{
    PLC_IO::pin_on_FAULT();
    PLC_IO::pin_on_RS_BLINK();
    PLC_IO::pin_on_STOP();
    PLC_CONTROL::fault_state_on();
    PLC_CONTROL::print_message("+-----------------------------+\n"
                               "+!!!!!! Error Handler() !!!!!!+\n"
                               "+!!!!!! Software reset  !!!!!!+\n"
                               "+-----------------------------+\n");

#ifdef PLATFORM_STM32
    NVIC_SystemReset();
#else
    while (1);
#endif
}
