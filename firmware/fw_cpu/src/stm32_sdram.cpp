#include "stm32_sdram.h"
#include "stm32_rcc.h"
#include "stm32_gpio.h"
#include "stm32_systick.h"
#include "my_func.h"

STM32_SDRAM sdram;

#define MEMTEST_PRINT
#ifdef MEMTEST_PRINT
#include "plc_control.h"
//#define MEMTEST_PRINT_VERBOSE
#endif

#define MEMTEST_BUF_SIZE (1024 * 4)

uint32_t STM32_SDRAM::init()
{
    init_gpio();

    STM32_RCC::enable_clk_FMC();

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

uint32_t STM32_SDRAM::run_tests(uint32_t start_addr, uint32_t size, bool print_debug)
{
    #ifdef MEMTEST_PRINT
    uint32_t start_ticks = STM32_SYSTICK::get_tick();
    if (print_debug)
        PLC_CONTROL::print_message("+-----------------------------+\n"
                                   "Start memory test (block size = %U bytes, bank 0x%X)\n",
                                   MEMTEST_BUF_SIZE, start_addr);
    #endif

    int cycles_count = size / MEMTEST_BUF_SIZE;
    for (int i=0 ; i<cycles_count ; ++i)
    {
        uint32_t* mem = (uint32_t*)(start_addr + (i * MEMTEST_BUF_SIZE));

        #ifdef MEMTEST_PRINT_VERBOSE
        if (print_debug)
        {
            uint32_t cur_start = start_addr + (i * MEMTEST_BUF_SIZE);
            //plc_state.data.mem_test.cur_start = start_addr + (i * MEMTEST_BUF_SIZE);
            PLC_CONTROL::print_message("\r\t\033[0KBlock #%05U/%05U (%08X-%08X): ", i+1,
                                        cycles_count,cur_start, start_addr + ((i + 1) * MEMTEST_BUF_SIZE));
        }
        #endif

        memset((uint8_t*)mem, 0, MEMTEST_BUF_SIZE);
        for (uint32_t uwIndex = 0; uwIndex < (MEMTEST_BUF_SIZE / sizeof(uint32_t)); uwIndex++)
            mem[uwIndex] = get_val(uwIndex);
        for (uint32_t uwIndex = 0; uwIndex < (MEMTEST_BUF_SIZE / sizeof(uint32_t)); uwIndex++)
            if (mem[uwIndex] != get_val(uwIndex))
            {
                #ifdef MEMTEST_PRINT
                PLC_CONTROL::print_message("ERROR at %08X (got %08X, exp %08X)\n",
                    start_addr + (i * MEMTEST_BUF_SIZE) + uwIndex * sizeof(uint32_t),
                    mem[uwIndex], get_val(uwIndex));
                #endif
                return STM32_RESULT_FAIL;
            }
        #ifdef MEMTEST_PRINT_VERBOSE
        if (print_debug)
            PLC_CONTROL::print_message("OK\n");
        #endif
        //HAL_IWDG_Refresh(&hiwdg);
    }

    #ifdef MEMTEST_PRINT
    if (print_debug)
    {
        int mem_size = (size) / 1024 / 1024;
        PLC_CONTROL::print_message("\r\033[0K\tTest Finished. Available memory: %UMb\n\tTest time: %Ums\n",
            mem_size, STM32_SYSTICK::get_tick() - start_ticks);
    }
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

    STM32_RCC::enable_clk_GPIOF();
    STM32_RCC::enable_clk_GPIOD();
    STM32_RCC::enable_clk_GPIOE();
    STM32_RCC::enable_clk_GPIOG();
    STM32_RCC::enable_clk_GPIOC();

    gpiof.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                     GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 |
                     GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_MODE_AF_PP,
                     GPIO_AF12_FMC, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL);

    gpiod.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                     GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 |
                     GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_AF12_FMC,
                     GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);

    gpioe.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 |
                     GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
                     GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_MODE_AF_PP,
                     GPIO_AF12_FMC, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);

    gpiog.set_config(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |
                     GPIO_PIN_8 | GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_AF12_FMC,
                     GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL);

    gpioc.set_config(GPIO_PIN_0 |
                 #ifdef STM32_SDRAM_BANK1
                     GPIO_PIN_2 |
                 #endif
                     GPIO_PIN_3, GPIO_MODE_AF_PP,
                     GPIO_AF12_FMC, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL);
}

void STM32_SDRAM::init_bank1()
{
    uint32_t tmpreg = FMC_Bank5_6->SDCR[0];
    tmpreg &= ~(FMC_SDCR1_NC  | FMC_SDCR1_NR | FMC_SDCR1_MWID |
                FMC_SDCR1_NB  | FMC_SDCR1_CAS | FMC_SDCR1_WP |
                FMC_SDCR1_SDCLK | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE);
   tmpreg |= (STM32_SDRAM_COLUMNS | STM32_SDRAM_ROWS | STM32_SDRAM_BUS_WIDTH |
              STM32_SDRAM_BANKS | STM32_SDRAM_CAS_LAT | STM32_SDRAM_WP |
              STM32_SDRAM_CLOCK | STM32_SDRAM_BURST | STM32_SDRAM_PIPE_DELAY);
   FMC_Bank5_6->SDCR[0] = tmpreg;
}

void STM32_SDRAM::init_bank2()
{

    uint32_t tmpreg1 = FMC_Bank5_6->SDCR[0];
    tmpreg1 &= ~(FMC_SDCR1_SDCLK | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE);
    tmpreg1 |= (STM32_SDRAM_CLOCK | STM32_SDRAM_BURST | STM32_SDRAM_PIPE_DELAY);
    uint32_t tmpreg2 = FMC_Bank5_6->SDCR[1];
    tmpreg2 &= ~(FMC_SDCR1_NC  | FMC_SDCR1_NR | FMC_SDCR1_MWID |
                 FMC_SDCR1_NB  | FMC_SDCR1_CAS | FMC_SDCR1_WP);
    tmpreg2 |= (STM32_SDRAM_COLUMNS | STM32_SDRAM_ROWS | STM32_SDRAM_BUS_WIDTH |
                STM32_SDRAM_BANKS | STM32_SDRAM_CAS_LAT | STM32_SDRAM_WP);
    FMC_Bank5_6->SDCR[0] = tmpreg1;
    FMC_Bank5_6->SDCR[1] = tmpreg2;
}

void STM32_SDRAM::set_timing_bank1()
{
    uint32_t tmpreg = FMC_Bank5_6->SDTR[0];
    tmpreg &= ~(FMC_SDTR1_TMRD | FMC_SDTR1_TXSR | FMC_SDTR1_TRAS |
                FMC_SDTR1_TRC  | FMC_SDTR1_TWR  | FMC_SDTR1_TRP |
                FMC_SDTR1_TRCD);
    tmpreg |= ((STM32_SDRAM_LOAD_TO_ACTIVE_DELAY - 1) |
               ((STM32_SDRAM_EXIT_SELF_REFRESH_DELAY - 1) << 4) |
               ((STM32_SDRAM_SELF_REFRESH_TIME - 1) << 8) |
               ((STM32_SDRAM_ROW_CYCLE_DELAY - 1) << 12) |
               ((STM32_SDRAM_WRITE_RECOVERY_TIME -1) << 16) |
               ((STM32_SDRAM_RP_DELAY - 1) << 20) |
               ((STM32_SDRAM_RCD_DELAY - 1) << 24));
    FMC_Bank5_6->SDTR[0] = tmpreg;
}

void STM32_SDRAM::set_timing_bank2()
{
    uint32_t tmpreg1 = FMC_Bank5_6->SDTR[0];
    tmpreg1 &= ~(FMC_SDTR1_TRC | FMC_SDTR1_TRP);
    tmpreg1 |= (((STM32_SDRAM_ROW_CYCLE_DELAY - 1) << 12) |
                ((STM32_SDRAM_RP_DELAY - 1) << 20));
    uint32_t tmpreg2 = FMC_Bank5_6->SDTR[1];
    tmpreg2 &= ~(FMC_SDTR1_TMRD | FMC_SDTR1_TXSR | FMC_SDTR1_TRAS |
                 FMC_SDTR1_TRC  | FMC_SDTR1_TWR  | FMC_SDTR1_TRP |
                 FMC_SDTR1_TRCD);
    tmpreg2 |= ((STM32_SDRAM_LOAD_TO_ACTIVE_DELAY - 1) |
                ((STM32_SDRAM_EXIT_SELF_REFRESH_DELAY - 1) << 4) |
                ((STM32_SDRAM_SELF_REFRESH_TIME - 1) << 8) |
                ((STM32_SDRAM_WRITE_RECOVERY_TIME -1) << 16) |
                ((STM32_SDRAM_RCD_DELAY - 1) << 24));
    FMC_Bank5_6->SDTR[0] = tmpreg1;
    FMC_Bank5_6->SDTR[1] = tmpreg2;
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
#define SDRAM_CMD_TIMEOUT			(0x1000)

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
    SEND_COMAND(SDRAM_CMD_CLK_ENABLE, FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2, 1, 0, SDRAM_CMD_TIMEOUT);
    STM32_SYSTICK::delay(100);
    SEND_COMAND(SDRAM_CMD_PALL, FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2, 1, 0, SDRAM_CMD_TIMEOUT);
    SEND_COMAND(SDRAM_CMD_AUTOREFRESH_MODE, FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2, 4, 0, SDRAM_CMD_TIMEOUT);
    SEND_COMAND(SDRAM_CMD_LOAD_MODE, FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2, 1,
                STM32_SDRAM_BURST_LENGTH |
                SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
                STM32_SDRAM_CAS_LAT_MR |
                SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                SDRAM_MODEREG_WRITEBURST_MODE_SINGLE, SDRAM_CMD_TIMEOUT);
    program_refresh_rate(STM32_SDRAM_REFRESH_RATE);
    disable_write_protection(0);
    disable_write_protection(1);
    #elif defined(STM32_SDRAM_BANK1)
    SEND_COMAND(SDRAM_CMD_CLK_ENABLE, FMC_SDCMR_CTB1, 1, 0, SDRAM_CMD_TIMEOUT);
    STM32_SYSTICK::delay(100);
    SEND_COMAND(SDRAM_CMD_PALL, FMC_SDCMR_CTB1, 1, 0, SDRAM_CMD_TIMEOUT);
    SEND_COMAND(SDRAM_CMD_AUTOREFRESH_MODE, FMC_SDCMR_CTB1, 4, 0, SDRAM_CMD_TIMEOUT);
    SEND_COMAND(SDRAM_CMD_LOAD_MODE, FMC_SDCMR_CTB1, 1,
                STM32_SDRAM_BURST_LENGTH |
                SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
                STM32_SDRAM_CAS_LAT_MR |
                SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                SDRAM_MODEREG_WRITEBURST_MODE_SINGLE, SDRAM_CMD_TIMEOUT);
    program_refresh_rate(STM32_SDRAM_REFRESH_RATE);
    disable_write_protection(0);
    #elif defined(STM32_SDRAM_BANK2)
    SEND_COMAND(SDRAM_CMD_CLK_ENABLE, FMC_SDCMR_CTB2, 1, 0, SDRAM_CMD_TIMEOUT);
    STM32_SYSTICK::delay(100);
    SEND_COMAND(SDRAM_CMD_PALL, FMC_SDCMR_CTB1, 1, 0, SDRAM_CMD_TIMEOUT);
    SEND_COMAND(SDRAM_CMD_AUTOREFRESH_MODE, FMC_SDCMR_CTB2, 4, 0, SDRAM_CMD_TIMEOUT);
    SEND_COMAND(SDRAM_CMD_LOAD_MODE, FMC_SDCMR_CTB2, 1,
                STM32_SDRAM_BURST_LENGTH |
                SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
                STM32_SDRAM_CAS_LAT_MR |
                SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                SDRAM_MODEREG_WRITEBURST_MODE_SINGLE, SDRAM_CMD_TIMEOUT);
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
