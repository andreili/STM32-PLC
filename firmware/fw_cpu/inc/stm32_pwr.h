#ifndef STM32_PWR_H
#define STM32_PWR_H

#include "stm32f4xx.h"
#include "bitbanding.h"

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

/* ------------- PWR registers bit address in the alias region ---------------*/
/* --- CR Register ---*/
/* Alias word address of FPDS bit */
#define FPDS_BIT_NUMBER          POSITION_VAL(PWR_CR_FPDS)
#define CR_FPDS_BB               (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (FPDS_BIT_NUMBER * 4U))

/* Alias word address of ODEN bit   */
#define ODEN_BIT_NUMBER          POSITION_VAL(PWR_CR_ODEN)
#define CR_ODEN_BB               (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (ODEN_BIT_NUMBER * 4U))

/* Alias word address of MRLVDS bit */
#define MRLVDS_BIT_NUMBER        POSITION_VAL(PWR_CR_MRLVDS)
#define CR_MRLVDS_BB             (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (MRLVDS_BIT_NUMBER * 4U))

/* Alias word address of LPLVDS bit */
#define LPLVDS_BIT_NUMBER        POSITION_VAL(PWR_CR_LPLVDS)
#define CR_LPLVDS_BB             (uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (LPLVDS_BIT_NUMBER * 4U))

class STM32_PWR
{
public:
    static inline void set_voltage_scaling_config(uint32_t config) { MODIFY_REG(PWR->CR, PWR_CR_VOS, config);  }
    static inline void enable_overdrive() { BIT_BAND_PER(PWR->CR, PWR_CR_ODSWEN) = 1; }
    static inline void disable_overdrive() { BIT_BAND_PER(PWR->CR, PWR_CR_ODSWEN) = 0; }
};

#endif // STM32_PWR_H
