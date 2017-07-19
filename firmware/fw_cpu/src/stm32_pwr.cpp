#include "stm32_pwr.h"
#include "stm32_systick.h"
#include "stm32_rcc.h"

#define PWR_OVERDRIVE_TIMEOUT_VALUE  1000U
#define PWR_UDERDRIVE_TIMEOUT_VALUE  1000U
#define PWR_BKPREG_TIMEOUT_VALUE     1000U
#define PWR_VOSRDY_TIMEOUT_VALUE     1000U

#define PWR_OVERDRIVE_ENABLE() BIT_BAND_PER(PWR->CR, PWR_CR_ODEN) = ENABLE
#define PWR_OVERDRIVE_DISABLE() BIT_BAND_PER(PWR->CR, PWR_CR_ODEN) = DISABLE

uint32_t STM32_PWR::enable_overdrive()
{
    STM32_RCC::enable_clk_PWR();

    /* Enable the Over-drive to extend the clock frequency to 180 Mhz */
    PWR_OVERDRIVE_ENABLE();

    /* Get tick */
    uint32_t tickstart = STM32_SYSTICK::get_tick();

    while ((PWR->CSR & PWR_FLAG_ODRDY) != PWR_FLAG_ODRDY)
    {
        if ((STM32_SYSTICK::get_tick() - tickstart) > PWR_OVERDRIVE_TIMEOUT_VALUE)
            return STM32_RESULT_TIMEOUT;
    }

    /* Enable the Over-drive switch */
    enable_overdrive_switching();

    /* Get tick */
    tickstart = STM32_SYSTICK::get_tick();

    while ((PWR->CSR & PWR_FLAG_ODSWRDY) != PWR_FLAG_ODSWRDY)
    {
        if ((STM32_SYSTICK::get_tick() - tickstart ) > PWR_OVERDRIVE_TIMEOUT_VALUE)
            return STM32_RESULT_TIMEOUT;
    }
    return STM32_RESULT_OK;
}

uint32_t STM32_PWR::disable_overdrive()
{
    STM32_RCC::enable_clk_PWR();

    /* Disable the Over-drive switch */
    disable_overdrive_switching();

    /* Get tick */
    uint32_t tickstart = STM32_SYSTICK::get_tick();

    while ((PWR->CSR & PWR_FLAG_ODSWRDY) == PWR_FLAG_ODSWRDY)
    {
        if ((STM32_SYSTICK::get_tick() - tickstart) > PWR_OVERDRIVE_TIMEOUT_VALUE)
            return STM32_RESULT_TIMEOUT;
    }

    /* Disable the Over-drive */
    PWR_OVERDRIVE_DISABLE();

    /* Get tick */
    tickstart = STM32_SYSTICK::get_tick();

    while ((PWR->CSR & PWR_FLAG_ODRDY) == PWR_FLAG_ODRDY)
    {
        if ((STM32_SYSTICK::get_tick() - tickstart) > PWR_OVERDRIVE_TIMEOUT_VALUE)
            return STM32_RESULT_TIMEOUT;
    }

    return STM32_RESULT_OK;
}

uint32_t STM32_PWR::enter_underdrive_stop_mode(uint32_t Regulator, uint8_t STOPEntry)
{
    uint32_t tmpreg1 = 0U;
    uint32_t tickstart = 0U;

    STM32_RCC::enable_clk_PWR();
    /* Enable the Under-drive Mode ---------------------------------------------*/
    /* Clear Under-drive flag */
    clear_odrudr_flag();

    /* Enable the Under-drive */
    enable_underdrive();

    /* Get tick */
    tickstart = STM32_SYSTICK::get_tick();

    /* Wait for UnderDrive mode is ready */
    while ((PWR->CSR & PWR_FLAG_UDRDY) == PWR_FLAG_UDRDY)
    {
        if ((STM32_SYSTICK::get_tick() - tickstart) > PWR_UDERDRIVE_TIMEOUT_VALUE)
            return STM32_RESULT_TIMEOUT;
    }

    /* Select the regulator state in STOP mode ---------------------------------*/
    tmpreg1 = PWR->CR;
    /* Clear PDDS, LPDS, MRLUDS and LPLUDS bits */
    tmpreg1 &= (uint32_t)~(PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_LPUDS | PWR_CR_MRUDS);

    /* Set LPDS, MRLUDS and LPLUDS bits according to PWR_Regulator value */
    tmpreg1 |= Regulator;

    /* Store the new value */
    PWR->CR = tmpreg1;

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Select STOP mode entry --------------------------------------------------*/
    if (STOPEntry == PWR_SLEEPENTRY_WFI)
        /* Request Wait For Interrupt */
        __WFI();
    else
        /* Request Wait For Event */
        __WFE();

    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);

    return STM32_RESULT_OK;
}

uint32_t STM32_PWR::enable_backup_regulator()
{
    BIT_BAND_PER(PWR->CSR, PWR_CSR_BRE) = SET;
    uint32_t tickstart = STM32_SYSTICK::get_tick();

    /* Wait till Backup regulator ready flag is set */
    while((RCC->CSR & PWR_FLAG_BRR) == RESET)
    {
        if((STM32_SYSTICK::get_tick() - tickstart) > PWR_BKPREG_TIMEOUT_VALUE)
            return STM32_RESULT_TIMEOUT;
    }
    return STM32_RESULT_OK;
}

uint32_t STM32_PWR::disable_backup_regulator()
{
    BIT_BAND_PER(PWR->CSR, PWR_CSR_BRE) = RESET;
    uint32_t tickstart = STM32_SYSTICK::get_tick();

    /* Wait till Backup regulator ready flag is set */
    while((RCC->CSR & PWR_FLAG_BRR) == SET)
    {
        if((STM32_SYSTICK::get_tick() - tickstart) > PWR_BKPREG_TIMEOUT_VALUE)
            return STM32_RESULT_TIMEOUT;
    }
    return STM32_RESULT_OK;
}
