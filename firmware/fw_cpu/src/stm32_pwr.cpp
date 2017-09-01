#include "stm32_inc.h"

#define PWR_OVERDRIVE_TIMEOUT_VALUE  1000U
#define PWR_UDERDRIVE_TIMEOUT_VALUE  1000U
#define PWR_BKPREG_TIMEOUT_VALUE     1000U
#define PWR_VOSRDY_TIMEOUT_VALUE     1000U

#define PWR_OVERDRIVE_ENABLE() BIT_BAND_PER(PWR->CR, PWR_CR_ODEN) = ENABLE
#define PWR_OVERDRIVE_DISABLE() BIT_BAND_PER(PWR->CR, PWR_CR_ODEN) = DISABLE

void STM32_PWR::deinit()
{
    STM32_RCC::force_reset_PWR();
    STM32_RCC::release_reset_PWR();
}

void STM32_PWR::config_PVD()
{
    disable_EXTI_event();
    disable_EXTI_IT();
    disable_EXTI_rising_edge();
    disable_EXTI_falling_edge();

    if ((STM32_PVD_MODE & PVD_MODE_IT) == PVD_MODE_IT)
        enable_EXTI_IT();
    if ((STM32_PVD_MODE & PVD_MODE_EVT) == PVD_MODE_EVT)
        enable_EXTI_event();
    if ((STM32_PVD_MODE & PVD_RISING_EDGE) == PVD_RISING_EDGE)
        enable_EXTI_rising_edge();
    if ((STM32_PVD_MODE & PVD_FALLING_EDGE) == PVD_FALLING_EDGE)
        enable_EXTI_falling_edge();
}

void STM32_PWR::enter_sleep_mode(uint8_t SLEEPEntry)
{
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
    if (SLEEPEntry == PWR_SLEEPENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
}

void STM32_PWR::enter_stop_mode(uint32_t Regulator, uint8_t STOPEntry)
{
    MODIFY_REG(PWR->CR, (PWR_CR_PDDS | PWR_CR_LPDS), Regulator);
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
    if(STOPEntry == PWR_STOPENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

void STM32_PWR::enter_standby_mode()
{
    SET_BIT(PWR->CR, PWR_CR_PDDS);
    SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
    __WFI();
}

#ifdef STM32F429xx
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
#endif

void ISR::PVD_IRQ()
{
    if (STM32_PWR::get_EXTI_flag() != RESET)
    {
        ///TODO: PVDCallback()
        STM32_PWR::clear_EXTI_flag();
    }
}
