#ifndef STM32_PWR_H
#define STM32_PWR_H

#include "stm32_conf.h"

#define PWR_WAKEUP_PIN1                 ((uint32_t)0x00000100U)

#define PWR_PVDLEVEL_0                  PWR_CR_PLS_LEV0
#define PWR_PVDLEVEL_1                  PWR_CR_PLS_LEV1
#define PWR_PVDLEVEL_2                  PWR_CR_PLS_LEV2
#define PWR_PVDLEVEL_3                  PWR_CR_PLS_LEV3
#define PWR_PVDLEVEL_4                  PWR_CR_PLS_LEV4
#define PWR_PVDLEVEL_5                  PWR_CR_PLS_LEV5
#define PWR_PVDLEVEL_6                  PWR_CR_PLS_LEV6
#define PWR_PVDLEVEL_7                  PWR_CR_PLS_LEV7/* External input analog voltage
                                                          (Compare internally to VREFINT) */

#define PWR_PVD_MODE_NORMAL                 ((uint32_t)0x00000000U)   /*!< basic mode is used */
#define PWR_PVD_MODE_IT_RISING              ((uint32_t)0x00010001U)   /*!< External Interrupt Mode with Rising edge trigger detection */
#define PWR_PVD_MODE_IT_FALLING             ((uint32_t)0x00010002U)   /*!< External Interrupt Mode with Falling edge trigger detection */
#define PWR_PVD_MODE_IT_RISING_FALLING      ((uint32_t)0x00010003U)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection */
#define PWR_PVD_MODE_EVENT_RISING           ((uint32_t)0x00020001U)   /*!< Event Mode with Rising edge trigger detection */
#define PWR_PVD_MODE_EVENT_FALLING          ((uint32_t)0x00020002U)   /*!< Event Mode with Falling edge trigger detection */
#define PWR_PVD_MODE_EVENT_RISING_FALLING   ((uint32_t)0x00020003U)   /*!< Event Mode with Rising/Falling edge trigger detection */

#define PWR_MAINREGULATOR_ON                        ((uint32_t)0x00000000U)
#define PWR_LOWPOWERREGULATOR_ON                    PWR_CR_LPDS

#define PWR_SLEEPENTRY_WFI              ((uint8_t)0x01U)
#define PWR_SLEEPENTRY_WFE              ((uint8_t)0x02U)

#define PWR_STOPENTRY_WFI               ((uint8_t)0x01U)
#define PWR_STOPENTRY_WFE               ((uint8_t)0x02U)

#define PWR_FLAG_WU                     PWR_CSR_WUF
#define PWR_FLAG_SB                     PWR_CSR_SBF
#define PWR_FLAG_PVDO                   PWR_CSR_PVDO
#define PWR_FLAG_BRR                    PWR_CSR_BRR
#define PWR_FLAG_VOSRDY                 PWR_CSR_VOSRDY

#define PWR_EXTI_LINE_PVD  ((uint32_t)EXTI_IMR_MR16)  /*!< External interrupt line 16 Connected to the PVD EXTI Line */

#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)
#define PWR_CR_OFFSET            0x00U
#define PWR_CSR_OFFSET           0x04U
#define PWR_CR_OFFSET_BB         (PWR_OFFSET + PWR_CR_OFFSET)
#define PWR_CSR_OFFSET_BB        (PWR_OFFSET + PWR_CSR_OFFSET)

#define PWR_MAINREGULATOR_UNDERDRIVE_ON                       PWR_CR_MRUDS
#define PWR_LOWPOWERREGULATOR_UNDERDRIVE_ON                   ((uint32_t)(PWR_CR_LPDS | PWR_CR_LPUDS))

#define PWR_FLAG_ODRDY                  PWR_CSR_ODRDY
#define PWR_FLAG_ODSWRDY                PWR_CSR_ODSWRDY
#define PWR_FLAG_UDRDY                  PWR_CSR_UDSWRDY

#define PWR_REGULATOR_VOLTAGE_SCALE1         PWR_CR_VOS             /* Scale 1 mode(default value at reset): the maximum value of fHCLK is 168 MHz. It can be extended to
                                                                       180 MHz by activating the over-drive mode. */
#define PWR_REGULATOR_VOLTAGE_SCALE2         PWR_CR_VOS_1           /* Scale 2 mode: the maximum value of fHCLK is 144 MHz. It can be extended to
                                                                       168 MHz by activating the over-drive mode. */
#define PWR_REGULATOR_VOLTAGE_SCALE3         PWR_CR_VOS_0           /* Scale 3 mode: the maximum value of fHCLK is 120 MHz. */

#define PWR_WAKEUP_PIN3                 ((uint32_t)0x00000040U)



class STM32_PWR
{
public:
    /* PWR PVD EXTI */
    static inline void enable_EXTI_IT() { BIT_BAND_PER(EXTI->IMR, PWR_EXTI_LINE_PVD) = ENABLE; }
    static inline void disable_EXTI_IT() { BIT_BAND_PER(EXTI->IMR, PWR_EXTI_LINE_PVD) = DISABLE; }

    static inline void enable_EXTI_event() { BIT_BAND_PER(EXTI->EMR, PWR_EXTI_LINE_PVD) = ENABLE; }
    static inline void disable_EXTI_event() { BIT_BAND_PER(EXTI->EMR, PWR_EXTI_LINE_PVD) = DISABLE; }

    static inline void enable_EXTI_rising_edge() { BIT_BAND_PER(EXTI->RTSR, PWR_EXTI_LINE_PVD) = ENABLE; }
    static inline void disable_EXTI_rising_edge() { BIT_BAND_PER(EXTI->RTSR, PWR_EXTI_LINE_PVD) = DISABLE; }

    static inline void enable_EXTI_falling_edge() { BIT_BAND_PER(EXTI->FTSR, PWR_EXTI_LINE_PVD) = ENABLE; }
    static inline void disable_EXTI_falling_edge() { BIT_BAND_PER(EXTI->FTSR, PWR_EXTI_LINE_PVD) = DISABLE; }

    static inline uint32_t get_EXTI_flag() { return EXTI->PR & (PWR_EXTI_LINE_PVD); }
    static inline void clear_EXTI_flag() { BIT_BAND_PER(EXTI->PR, PWR_EXTI_LINE_PVD) = DISABLE; }

    static inline void set_EXTI_generate_swit() { BIT_BAND_PER(EXTI->SWIER, PWR_EXTI_LINE_PVD) = ENABLE; }

    /* Voltage control */
    static inline void set_voltage_scaling_config(uint32_t config) { MODIFY_REG(PWR->CR, PWR_CR_VOS, config);  }

    static uint32_t enable_overdrive();
    static uint32_t disable_overdrive();

    static inline void enable_overdrive_switching() { BIT_BAND_PER(PWR->CR, PWR_CR_ODSWEN) = ENABLE; }
    static inline void disable_overdrive_switching() { BIT_BAND_PER(PWR->CR, PWR_CR_ODSWEN) = DISABLE; }

    static inline void enable_underdrive() { BIT_BAND_PER(PWR->CR, PWR_CR_UDEN) = ENABLE; }
    static inline void disable_underdrive() { BIT_BAND_PER(PWR->CR, PWR_CR_UDEN) = DISABLE; }

    static uint32_t enter_underdrive_stop_mode(uint32_t Regulator, uint8_t STOPEntry);

    static inline bool get_odrudr_flag(uint32_t flag_msk) { return ((PWR->CSR & flag_msk) == flag_msk); }
    static inline void clear_odrudr_flag() { BIT_BAND_PER(PWR->CSR, PWR_FLAG_UDRDY) = ENABLE; }

    static inline uint32_t get_voltage_range() { return PWR->CR & PWR_CR_VOS; }

    /* Backup regulator */
    static uint32_t enable_backup_regulator();
    static uint32_t disable_backup_regulator();

    /* flash control */
    static inline void enable_flash_power_down() { BIT_BAND_PER(PWR->CR, PWR_CR_FPDS) = ENABLE; }
    static inline void disable_flash_power_down() { BIT_BAND_PER(PWR->CR, PWR_CR_FPDS) = DISABLE; }
};

#endif // STM32_PWR_H
