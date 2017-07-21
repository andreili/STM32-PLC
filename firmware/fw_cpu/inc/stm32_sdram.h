#ifndef STM32_SDRAM_H
#define STM32_SDRAM_H

#include "stm32_conf.h"

#define SDRAM_BASE_BANK1 0xC0000000
#define SDRAM_BASE_BANK2 0xD0000000

#define FMC_SDRAM_COLUMN_BITS_NUM_8           ((uint32_t)0x00000000U)
#define FMC_SDRAM_COLUMN_BITS_NUM_9           ((uint32_t)0x00000001U)
#define FMC_SDRAM_COLUMN_BITS_NUM_10          ((uint32_t)0x00000002U)
#define FMC_SDRAM_COLUMN_BITS_NUM_11          ((uint32_t)0x00000003U)

#define FMC_SDRAM_ROW_BITS_NUM_11             ((uint32_t)0x00000000U)
#define FMC_SDRAM_ROW_BITS_NUM_12             ((uint32_t)0x00000004U)
#define FMC_SDRAM_ROW_BITS_NUM_13             ((uint32_t)0x00000008U)

#define FMC_SDRAM_MEM_BUS_WIDTH_8             ((uint32_t)0x00000000U)
#define FMC_SDRAM_MEM_BUS_WIDTH_16            ((uint32_t)0x00000010U)
#define FMC_SDRAM_MEM_BUS_WIDTH_32            ((uint32_t)0x00000020U)

#define FMC_SDRAM_INTERN_BANKS_NUM_2          ((uint32_t)0x00000000U)
#define FMC_SDRAM_INTERN_BANKS_NUM_4          ((uint32_t)0x00000040U)

#define FMC_SDRAM_CAS_LATENCY_1               ((uint32_t)0x00000080U)
#define FMC_SDRAM_CAS_LATENCY_2               ((uint32_t)0x00000100U)
#define FMC_SDRAM_CAS_LATENCY_3               ((uint32_t)0x00000180U)

#define FMC_SDRAM_WRITE_PROTECTION_DISABLE    ((uint32_t)0x00000000U)
#define FMC_SDRAM_WRITE_PROTECTION_ENABLE     ((uint32_t)0x00000200U)

#define FMC_SDRAM_CLOCK_DISABLE               ((uint32_t)0x00000000U)
#define FMC_SDRAM_CLOCK_PERIOD_2              ((uint32_t)0x00000800U)
#define FMC_SDRAM_CLOCK_PERIOD_3              ((uint32_t)0x00000C00U)

#define FMC_SDRAM_RBURST_DISABLE              ((uint32_t)0x00000000U)
#define FMC_SDRAM_RBURST_ENABLE               ((uint32_t)0x00001000U)

#define FMC_SDRAM_RPIPE_DELAY_0               ((uint32_t)0x00000000U)
#define FMC_SDRAM_RPIPE_DELAY_1               ((uint32_t)0x00002000U)
#define FMC_SDRAM_RPIPE_DELAY_2               ((uint32_t)0x00004000U)

#define FMC_SDRAM_CMD_NORMAL_MODE             ((uint32_t)0x00000000U)
#define FMC_SDRAM_CMD_CLK_ENABLE              ((uint32_t)0x00000001U)
#define FMC_SDRAM_CMD_PALL                    ((uint32_t)0x00000002U)
#define FMC_SDRAM_CMD_AUTOREFRESH_MODE        ((uint32_t)0x00000003U)
#define FMC_SDRAM_CMD_LOAD_MODE               ((uint32_t)0x00000004U)
#define FMC_SDRAM_CMD_SELFREFRESH_MODE        ((uint32_t)0x00000005U)
#define FMC_SDRAM_CMD_POWERDOWN_MODE          ((uint32_t)0x00000006U)

#define FMC_SDRAM_CMD_TARGET_BANK2            FMC_SDCMR_CTB2
#define FMC_SDRAM_CMD_TARGET_BANK1            FMC_SDCMR_CTB1
#define FMC_SDRAM_CMD_TARGET_BANK1_2          ((uint32_t)0x00000018U)

#define FMC_SDRAM_NORMAL_MODE                 ((uint32_t)0x00000000U)
#define FMC_SDRAM_SELF_REFRESH_MODE           FMC_SDSR_MODES1_0
#define FMC_SDRAM_POWER_DOWN_MODE             FMC_SDSR_MODES1_1

class STM32_SDRAM
{
public:
    static uint32_t init();
    static uint32_t run_tests(uint32_t start_addr, uint32_t size);
private:
    static void init_gpio();
    static void init_bank1();
    static void init_bank2();
    static void set_timing_bank1();
    static void set_timing_bank2();
    static uint32_t run_init_sequence();
    static uint32_t send_comand(uint32_t mode, uint32_t target, uint32_t refresh, uint32_t mode_reg_def, uint32_t timeout);

    static inline void program_refresh_rate(uint32_t rate) { FMC_Bank5_6->SDRTR |= (rate << 1); }
    static inline void disable_write_protection(int bank) { MODIFY_REG(FMC_Bank5_6->SDCR[bank], FMC_SDCR1_WP, 0); }
};

#endif // STM32_SDRAM_H
