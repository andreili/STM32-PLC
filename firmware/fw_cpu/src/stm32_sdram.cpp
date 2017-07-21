#include "stm32_sdram.h"
#include "stm32_rcc.h"
#include "stm32_gpio.h"
#include "stm32_systick.h"
#include "my_func.h"

STM32_SDRAM sdram;

#define MEMTEST_PRINT
#ifdef MEMTEST_PRINT
#include "xprintf.h"
#define MEMTEST_PRINT_VERBOSE
#endif

#define MEMTEST_BUF_SIZE 1024

uint32_t STM32_SDRAM::init()
{
    //Initialize FMC
    STM32_RCC::enable_clk_FMC();

    init_gpio();

    #ifdef STM32_SDRAM_BANK1
    init_bank1();
    set_timing_bank1();
    #endif
    #ifdef STM32_SDRAM_BANK2
    init_bank2();
    set_timing_bank2();
    #endif

    if (run_init_sequence() != STM32_RESULT_OK)
        return STM32_RESULT_FAIL;

    return STM32_RESULT_OK;
}

#define MEM_TEST_BODY(data_type) \
    { \
    }

uint32_t inline get_val(uint32_t offset)
{
    return 0x12345678 + (offset << 8);
}

uint32_t STM32_SDRAM::run_tests(uint32_t start_addr, uint32_t size)
{
    uint32_t start_ticks = STM32_SYSTICK::get_tick();
    #ifdef MEMTEST_PRINT
    xprintf("Start memory test (block size = %U bytes, bank 0x%X)\n", MEMTEST_BUF_SIZE, start_addr);
    #endif

    int cycles_count = size / MEMTEST_BUF_SIZE;
    for (int i=0 ; i<cycles_count ; ++i)
    {
        uint32_t* mem = (uint32_t*)(start_addr + (i * MEMTEST_BUF_SIZE));

        uint32_t cur_start = start_addr + (i * MEMTEST_BUF_SIZE);
        //plc_state.data.mem_test.cur_start = start_addr + (i * MEMTEST_BUF_SIZE);
        #ifdef MEMTEST_PRINT_VERBOSE
        xprintf("\r\t\033[0KBlock #%05U/%05U (%08X-%08X): ", i+1, cycles_count,cur_start, start_addr + ((i + 1) * MEMTEST_BUF_SIZE));
        #endif

        memset((uint8_t*)mem, 0, MEMTEST_BUF_SIZE);
        for (uint32_t uwIndex = 0; uwIndex < (MEMTEST_BUF_SIZE / sizeof(uint32_t)); uwIndex++)
            mem[uwIndex] = get_val(uwIndex);
        for (uint32_t uwIndex = 0; uwIndex < (MEMTEST_BUF_SIZE / sizeof(uint32_t)); uwIndex++)
            if (mem[uwIndex] != get_val(uwIndex))
            {
                xprintf("ERROR at %08X (got %08X, exp %08X)\n",
                    start_addr + (i * MEMTEST_BUF_SIZE) + uwIndex * sizeof(uint32_t),
                    mem[uwIndex], get_val(uwIndex));
                return STM32_RESULT_FAIL;
            }
        #ifdef MEMTEST_PRINT_VERBOSE
        xprintf("OK");
        #endif
        //HAL_IWDG_Refresh(&hiwdg);
    }

    #ifdef MEMTEST_PRINT
    xprintf("\r\033[0KTest Finished. Available memory: %.2fMb\nTest time (HAL ticks): %i\n",
        (size * 1.f) / 1024.f / 1024.f, STM32_SYSTICK::get_tick() - start_ticks);
    #endif

    return STM32_RESULT_OK;
}

void STM32_SDRAM::init_gpio()
{
    #ifdef STM32_SDRAM_BANK2
    STM32_RCC::enable_clk_GPIOB();
    gpiob.set_config(GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_3, GPIO_MODE_AF_PP,
                     GPIO_AF12_FMC, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
    #endif
    STM32_RCC::enable_clk_GPIOC();
    gpioc.set_config(GPIO_PIN_0 |
                 #ifdef STM32_SDRAM_BANK1
                     GPIO_PIN_2 |
                 #endif
                     GPIO_PIN_3, GPIO_MODE_AF_PP,
                     GPIO_AF12_FMC, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
    STM32_RCC::enable_clk_GPIOD();
    gpiod.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                     GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 |
                     GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_AF12_FMC,
                     GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
    STM32_RCC::enable_clk_GPIOE();
    gpioe.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 |
                     GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
                     GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_MODE_AF_PP,
                     GPIO_AF12_FMC, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
    STM32_RCC::enable_clk_GPIOF();
    gpiof.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                     GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 |
                     GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_MODE_AF_PP,
                     GPIO_AF12_FMC, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
    STM32_RCC::enable_clk_GPIOG();
    gpiog.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |
                     GPIO_PIN_8 | GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_AF12_FMC,
                     GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
}

void STM32_SDRAM::init_bank1()
{
    FMC_Bank5_6->SDCR[0] &= ~(FMC_SDCR1_NC  | FMC_SDCR1_NR | FMC_SDCR1_MWID |
                              FMC_SDCR1_NB  | FMC_SDCR1_CAS | FMC_SDCR1_WP |
                              FMC_SDCR1_SDCLK | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE);
    FMC_Bank5_6->SDCR[0] |= (STM32_SDRAM_COLUMNS | STM32_SDRAM_ROWS | STM32_SDRAM_BUS_WIDTH |
                             STM32_SDRAM_BANKS | STM32_SDRAM_CAS_LAT | STM32_SDRAM_WP |
                             STM32_SDRAM_CLOCK | STM32_SDRAM_BURST | STM32_SDRAM_PIPE_DELAY);
}

void STM32_SDRAM::init_bank2()
{

    FMC_Bank5_6->SDCR[0] &= ~(FMC_SDCR1_SDCLK | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE);
    FMC_Bank5_6->SDCR[0] |= (STM32_SDRAM_CLOCK | STM32_SDRAM_BURST | STM32_SDRAM_PIPE_DELAY);
    FMC_Bank5_6->SDCR[1] &= ~(FMC_SDCR1_NC  | FMC_SDCR1_NR | FMC_SDCR1_MWID |
                              FMC_SDCR1_NB  | FMC_SDCR1_CAS | FMC_SDCR1_WP);
    FMC_Bank5_6->SDCR[1] |= (STM32_SDRAM_COLUMNS | STM32_SDRAM_ROWS | STM32_SDRAM_BUS_WIDTH |
                             STM32_SDRAM_BANKS | STM32_SDRAM_CAS_LAT | STM32_SDRAM_WP);
}

void STM32_SDRAM::set_timing_bank1()
{
    FMC_Bank5_6->SDTR[0] &= ~(FMC_SDTR1_TMRD | FMC_SDTR1_TXSR | FMC_SDTR1_TRAS |
                              FMC_SDTR1_TRC  | FMC_SDTR1_TWR  | FMC_SDTR1_TRP |
                              FMC_SDTR1_TRCD);
    FMC_Bank5_6->SDTR[0] |= ((STM32_SDRAM_LOAD_TO_ACTIVE_DELAY - 1) |
                             ((STM32_SDRAM_EXIT_SELF_REFRESH_DELAY - 1) << 4) |
                             ((STM32_SDRAM_SELF_REFRESH_TIME - 1) << 8) |
                             ((STM32_SDRAM_ROW_CYCLE_DELAY - 1) << 12) |
                             ((STM32_SDRAM_WRITE_RECOVERY_TIME -1) << 16) |
                             ((STM32_SDRAM_RP_DELAY - 1) << 20) |
                             ((STM32_SDRAM_RCD_DELAY - 1) << 24));
}

void STM32_SDRAM::set_timing_bank2()
{
    FMC_Bank5_6->SDTR[0] &= ~(FMC_SDTR1_TRC | FMC_SDTR1_TRP);
    FMC_Bank5_6->SDTR[0] |= (((STM32_SDRAM_ROW_CYCLE_DELAY - 1) << 12) |
                             ((STM32_SDRAM_RP_DELAY - 1) << 20));
    FMC_Bank5_6->SDTR[1] &= ~(FMC_SDTR1_TMRD | FMC_SDTR1_TXSR | FMC_SDTR1_TRAS |
                              FMC_SDTR1_TRC  | FMC_SDTR1_TWR  | FMC_SDTR1_TRP |
                              FMC_SDTR1_TRCD);
    FMC_Bank5_6->SDTR[1] |= ((STM32_SDRAM_LOAD_TO_ACTIVE_DELAY - 1) |
                             ((STM32_SDRAM_EXIT_SELF_REFRESH_DELAY - 1) << 4) |
                             ((STM32_SDRAM_SELF_REFRESH_TIME - 1) << 8) |
                             ((STM32_SDRAM_WRITE_RECOVERY_TIME -1) << 16) |
                             ((STM32_SDRAM_RCD_DELAY - 1) << 24));
}

#define SEND_COMAND(m, t, r, mrd, to) \
    { \
        if (send_comand(m, t, r, mrd, to) != STM32_RESULT_OK) \
            return STM32_RESULT_FAIL; \
    }

#define SDRAM_CMD_CLK_ENABLE		(FMC_SDCMR_MODE_0)
#define SDRAM_CMD_PALL				(FMC_SDCMR_MODE_1)
#define SDRAM_CMD_AUTOREFRESH_MODE	(FMC_SDCMR_MODE_0 | FMC_SDCMR_MODE_1)
#define SDRAM_CMD_LOAD_MODE			(FMC_SDCMR_MODE_2)

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

uint32_t STM32_SDRAM::run_init_sequence()
{
    #if defined(STM32_SDRAM_BANK1) && defined(STM32_SDRAM_BANK2)
    SEND_COMAND(SDRAM_CMD_CLK_ENABLE, FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2, 1, 0, 0xffff);
    STM32_SYSTICK::delay(100);
    SEND_COMAND(SDRAM_CMD_PALL, FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2, 1, 0, 0xffff);
    SEND_COMAND(SDRAM_CMD_AUTOREFRESH_MODE, FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2, 4, 0, 0xffff);
    SEND_COMAND(SDRAM_CMD_LOAD_MODE, FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2, 1,
                STM32_SDRAM_BURST_LENGTH |
                SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
                STM32_SDRAM_CAS_LAT_MR |
                SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                SDRAM_MODEREG_WRITEBURST_MODE_SINGLE, 0xffff);
    program_refresh_rate(STM32_SDRAM_REFRESH_RATE);
    disable_write_protection(0);
    disable_write_protection(1);
    #elif defined(STM32_SDRAM_BANK1)
    SEND_COMAND(SDRAM_CMD_CLK_ENABLE, FMC_SDCMR_CTB1, 1, 0, 0xffff);
    STM32_SYSTICK::delay(100);
    SEND_COMAND(SDRAM_CMD_PALL, FMC_SDCMR_CTB1, 1, 0, 0xffff);
    SEND_COMAND(SDRAM_CMD_AUTOREFRESH_MODE, FMC_SDCMR_CTB1, 4, 0, 0xffff);
    SEND_COMAND(SDRAM_CMD_LOAD_MODE, FMC_SDCMR_CTB1, 1,
                STM32_SDRAM_BURST_LENGTH |
                SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
                STM32_SDRAM_CAS_LAT_MR |
                SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                SDRAM_MODEREG_WRITEBURST_MODE_SINGLE, 0xffff);
    program_refresh_rate(STM32_SDRAM_REFRESH_RATE);
    disable_write_protection(0);
    #elif defined(STM32_SDRAM_BANK2)
    SEND_COMAND(SDRAM_CMD_CLK_ENABLE, FMC_SDCMR_CTB2, 1, 0, 0xffff);
    STM32_SYSTICK::delay(100);
    SEND_COMAND(SDRAM_CMD_PALL, FMC_SDCMR_CTB1, 1, 0, 0xffff);
    SEND_COMAND(SDRAM_CMD_AUTOREFRESH_MODE, FMC_SDCMR_CTB2, 4, 0, 0xffff);
    SEND_COMAND(SDRAM_CMD_LOAD_MODE, FMC_SDCMR_CTB2, 1,
                STM32_SDRAM_BURST_LENGTH |
                SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
                STM32_SDRAM_CAS_LAT_MR |
                SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                SDRAM_MODEREG_WRITEBURST_MODE_SINGLE, 0xffff);
    program_refresh_rate(STM32_SDRAM_REFRESH_RATE);
    disable_write_protection(1);
    #endif

    return STM32_RESULT_OK;
}

uint32_t STM32_SDRAM::send_comand(uint32_t mode, uint32_t target, uint32_t refresh, uint32_t mode_reg_def, uint32_t timeout)
{
    FMC_Bank5_6->SDCMR = mode | target | ((refresh -1) << 5) | (mode_reg_def << 9);
    WAIT_TIMEOUT((FMC_Bank5_6->SDSR & FMC_SDSR_BUSY), timeout);
    return STM32_RESULT_OK;
}
