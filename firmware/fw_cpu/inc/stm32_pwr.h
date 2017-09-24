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

#define PVD_MODE_IT               ((uint32_t)0x00010000U)
#define PVD_MODE_EVT              ((uint32_t)0x00020000U)
#define PVD_RISING_EDGE           ((uint32_t)0x00000001U)
#define PVD_FALLING_EDGE          ((uint32_t)0x00000002U)

class STM32_PWR
{
public:
    static void deinit();

    ENDIS_REG_FLAG(backup_access, PWR->CR, PWR_CR_DBP)
    static inline bool is_backup_acces_RO() { return ((PWR->CR & PWR_CR_DBP) == RESET); }

    static void config_PVD();
    ENDIS_REG_FLAG(PVD, PWR->CR, PWR_CR_PVDE)

    static inline void enable_wakeup_pin(uint32_t WakeUpPinx) { SET_BIT(PWR->CSR, WakeUpPinx); }
    static inline void disable_wakeup_pin(uint32_t WakeUpPinx) { CLEAR_BIT(PWR->CSR, WakeUpPinx); }

    static void enter_sleep_mode(uint8_t SLEEPEntry);
    static void enter_stop_mode(uint32_t Regulator, uint8_t STOPEntry);
    static void enter_standby_mode();

    ENDIS_REG_FLAG(sleep_on_exit, SCB->SCR, SCB_SCR_SLEEPONEXIT_Msk)
    ENDIS_REG_FLAG(SEV_on_pend, SCB->SCR, SCB_SCR_SEVONPEND_Msk)

    /* PWR PVD EXTI */
    ENDIS_REG_FLAG(EXTI_IT, EXTI->IMR, PWR_EXTI_LINE_PVD)
    ENDIS_REG_FLAG(EXTI_event, EXTI->EMR, PWR_EXTI_LINE_PVD)
    ENDIS_REG_FLAG(EXTI_rising_edge, EXTI->RTSR, PWR_EXTI_LINE_PVD)
    ENDIS_REG_FLAG(EXTI_falling_edge, EXTI->FTSR, PWR_EXTI_LINE_PVD)

    static inline uint32_t get_EXTI_flag() { return EXTI->PR & (PWR_EXTI_LINE_PVD); }
    static inline void clear_EXTI_flag() { BIT_BAND_PER(EXTI->PR, PWR_EXTI_LINE_PVD) = DISABLE; }

    static inline void set_EXTI_generate_swit() { BIT_BAND_PER(EXTI->SWIER, PWR_EXTI_LINE_PVD) = ENABLE; }

    /* Voltage control */
    static inline void set_voltage_scaling_config(uint32_t config) { MODIFY_REG(PWR->CR, PWR_CR_VOS, config);  }

    static uint32_t enable_overdrive();
    static uint32_t disable_overdrive();

    ENDIS_REG_FLAG(overdrive_switching, PWR->CR, PWR_CR_ODSWEN)
    ENDIS_REG_FLAG(underdrive, PWR->CR, PWR_CR_UDEN)

    static uint32_t enter_underdrive_stop_mode(uint32_t Regulator, uint8_t STOPEntry);

    static inline bool get_odrudr_flag(uint32_t flag_msk) { return ((PWR->CSR & flag_msk) == flag_msk); }
    static inline void clear_odrudr_flag() { BIT_BAND_PER(PWR->CSR, PWR_FLAG_UDRDY) = ENABLE; }

    static inline uint32_t get_voltage_range() { return PWR->CR & PWR_CR_VOS; }

    /* Backup regulator */
    static uint32_t enable_backup_regulator();
    static uint32_t disable_backup_regulator();

    /* flash control */
    ENDIS_REG_FLAG(flash_power_down, PWR->CR, PWR_CR_FPDS)
};

#endif // STM32_PWR_H
