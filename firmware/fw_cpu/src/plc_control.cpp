#include "plc_control.h"
#include "stm32_inc.h"
#include "plc_mod.h"
#include "plc_io.h"
#include "xprintf.h"
#include <stdarg.h>
#include "memmanager.h"
#ifdef STM32_FATFS_USE
#include "fatfs.h"
#include "ff.h"
#include "sddriver.h"
#endif

plc_state_t PLC_CONTROL::m_state;
char PLC_CONTROL::m_text_buf[PLC_TEXT_BUF_SIZE];
STM32_RTC_Date PLC_CONTROL::m_start_date;
STM32_RTC_Time PLC_CONTROL::m_start_time;

#ifdef STM32_FATFS_USE
FATFS SDFatFs;
extern char SD_path[4];
#endif

void xfunc_out(unsigned char ch)
{
    uart3.send_char(ch);
}

void PLC_CONTROL::init()
{
    m_state.run = 0;
    m_state.stop = 1;
    m_state.rs_blink = 0;
    m_state.fault = 0;
    m_state.initialized = 0;
}

void PLC_CONTROL::init_hw()
{
    plc_mod_com.init(&spi6, true);
    plc_mod_ext.init(&spi5, false);

    plc_mod_com.find_modules();
    plc_mod_ext.find_modules();
}

void xvprintf(const char* fmt, va_list arp);
extern char* outptr;

void PLC_CONTROL::print_message(const char* fmt, ...)
{
    va_list arp;

    outptr = m_text_buf;

    va_start(arp, fmt);
    xvprintf(fmt, arp);
    va_end(arp);

    *outptr = 0;
    outptr = 0;
    uart3.send_str(m_text_buf, TXRX_MODE::INTERRUPT);
}

void PLC_CONTROL::init_seq()
{
    set_rs_blink(1);

    scheck_RTC();
    STM32_RTC::get_date(&m_start_date, ERTCFormat::BIN);
    STM32_RTC::get_time(&m_start_time, ERTCFormat::BIN);

    print_message("\033[2J\n+-----------------------------+\n"
                  "|          STM32 PLC          |\n"
                  "|         System info         |\n"
                  "| CPU Speed: %03UMHz           |\n"
                  "| RAM size: 192kb + %03UMb     |\n"
                  "| Cur. time %02U.%02U.%02U %02U:%02U:%02U |\n"
                  "+-----------------------------+\n",
                  STM32_PLLN, STM32_SDRAM_SIZE_MB,
                  m_start_date.Date, m_start_date.Month, m_start_date.Year,
                  m_start_time.Hours, m_start_time.Minutes, m_start_time.Seconds);

    #if defined (DATA_IN_ExtSDRAM)
    test_RAM(true);
    test_RAM_speed();
    #endif

    MemManager::init();

    #ifdef STM32_FATFS_USE
    init_fs();
    #endif

    set_initialized(1);
    print_message("+-----------------------------+\n"
                   "Initializing PLC hardware\n");
    init_hw();
    print_message("+-----------------------------+\n");

    set_stop(0);
    set_run(1);
    set_rs_blink(1);
}

void PLC_CONTROL::main()
{
    print_message("Start main cycle\n");
    int iteration = 0;
    while (1)
    {
        STM32_RTC_Time time;
        STM32_RTC::get_time(&time, ERTCFormat::BIN);
        int millisecs = (1000 * (STM32_RTC_SYNC_PREDIV - time.SubSeconds)) / (STM32_RTC_SYNC_PREDIV + 1);
        print_message("\r\t(%02U:%02U:%02U:%03U) Test iteration: %U",
                      time.Hours, time.Minutes, time.Seconds, millisecs,
                      ++iteration);

        test_RAM(false);
    }
}

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
        time.Hours = 20;
        time.Minutes = 01;
        time.Seconds = 30;
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
            STM32_SYSTICK::delay(300);
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

void Error_Handler()
{
    PLC_IO::pin_on_FAULT();
    PLC_IO::pin_on_RS_BLINK();
    PLC_IO::pin_on_STOP();
    PLC_CONTROL::set_run(0);
    PLC_CONTROL::set_fault(1);
    PLC_CONTROL::set_stop(1);
    PLC_CONTROL::print_message("+-----------------------------+\n"
                               "+!!!!!! Error Handler() !!!!!!+\n"
                               "+!!!!!! Software reset  !!!!!!+\n"
                               "+-----------------------------+\n");

    NVIC_SystemReset();
}
