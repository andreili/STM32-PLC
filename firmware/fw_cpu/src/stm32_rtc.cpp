#include "stm32_rtc.h"

#define RTC_TIMEOUT_VALUE       1000

#define RTC_EXTI_LINE_ALARM_EVENT      ((uint32_t)EXTI_IMR_MR17)

#define RTC_HOURFORMAT_24              ((uint32_t)0x00000000U)
#define RTC_HOURFORMAT_12              ((uint32_t)0x00000040U)

#define RTC_OUTPUT_DISABLE             ((uint32_t)0x00000000U)
#define RTC_OUTPUT_ALARMA              ((uint32_t)0x00200000U)
#define RTC_OUTPUT_ALARMB              ((uint32_t)0x00400000U)
#define RTC_OUTPUT_WAKEUP              ((uint32_t)0x00600000U)

#define RTC_OUTPUT_POLARITY_HIGH       ((uint32_t)0x00000000U)
#define RTC_OUTPUT_POLARITY_LOW        ((uint32_t)0x00100000U)

#define RTC_OUTPUT_TYPE_OPENDRAIN      ((uint32_t)0x00000000U)
#define RTC_OUTPUT_TYPE_PUSHPULL       ((uint32_t)0x00040000U)

#define RTC_HOURFORMAT12_AM            ((uint8_t)0x00U)
#define RTC_HOURFORMAT12_PM            ((uint8_t)0x40U)

#define RTC_DAYLIGHTSAVING_SUB1H       ((uint32_t)0x00020000U)
#define RTC_DAYLIGHTSAVING_ADD1H       ((uint32_t)0x00010000U)
#define RTC_DAYLIGHTSAVING_NONE        ((uint32_t)0x00000000U)

#define RTC_STOREOPERATION_RESET        ((uint32_t)0x00000000U)
#define RTC_STOREOPERATION_SET          ((uint32_t)0x00040000U)

#define RTC_ALARMDATEWEEKDAYSEL_DATE      ((uint32_t)0x00000000U)
#define RTC_ALARMDATEWEEKDAYSEL_WEEKDAY   ((uint32_t)0x40000000U)

#define RTC_ALARMMASK_NONE                ((uint32_t)0x00000000U)
#define RTC_ALARMMASK_DATEWEEKDAY         RTC_ALRMAR_MSK4
#define RTC_ALARMMASK_HOURS               RTC_ALRMAR_MSK3
#define RTC_ALARMMASK_MINUTES             RTC_ALRMAR_MSK2
#define RTC_ALARMMASK_SECONDS             RTC_ALRMAR_MSK1
#define RTC_ALARMMASK_ALL                 ((uint32_t)0x80808080U)

#define RTC_ALARM_A                       RTC_CR_ALRAE
#define RTC_ALARM_B                       RTC_CR_ALRBE

#define RTC_ALARMSUBSECONDMASK_ALL         ((uint32_t)0x00000000U)  /*!< All Alarm SS fields are masked.
                                                                        There is no comparison on sub seconds
                                                                        for Alarm */
#define RTC_ALARMSUBSECONDMASK_SS14_1      ((uint32_t)0x01000000U)  /*!< SS[14:1] are don't care in Alarm
                                                                        comparison. Only SS[0] is compared.    */
#define RTC_ALARMSUBSECONDMASK_SS14_2      ((uint32_t)0x02000000U)  /*!< SS[14:2] are don't care in Alarm
                                                                        comparison. Only SS[1:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_3      ((uint32_t)0x03000000U)  /*!< SS[14:3] are don't care in Alarm
                                                                        comparison. Only SS[2:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_4      ((uint32_t)0x04000000U)  /*!< SS[14:4] are don't care in Alarm
                                                                        comparison. Only SS[3:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_5      ((uint32_t)0x05000000U)  /*!< SS[14:5] are don't care in Alarm
                                                                        comparison. Only SS[4:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_6      ((uint32_t)0x06000000U)  /*!< SS[14:6] are don't care in Alarm
                                                                        comparison. Only SS[5:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_7      ((uint32_t)0x07000000U)  /*!< SS[14:7] are don't care in Alarm
                                                                        comparison. Only SS[6:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_8      ((uint32_t)0x08000000U)  /*!< SS[14:8] are don't care in Alarm
                                                                        comparison. Only SS[7:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_9      ((uint32_t)0x09000000U)  /*!< SS[14:9] are don't care in Alarm
                                                                        comparison. Only SS[8:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_10     ((uint32_t)0x0A000000U)  /*!< SS[14:10] are don't care in Alarm
                                                                        comparison. Only SS[9:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_11     ((uint32_t)0x0B000000U)  /*!< SS[14:11] are don't care in Alarm
                                                                        comparison. Only SS[10:0] are compared */
#define RTC_ALARMSUBSECONDMASK_SS14_12     ((uint32_t)0x0C000000U)  /*!< SS[14:12] are don't care in Alarm
                                                                        comparison.Only SS[11:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_SS14_13     ((uint32_t)0x0D000000U)  /*!< SS[14:13] are don't care in Alarm
                                                                        comparison. Only SS[12:0] are compared */
#define RTC_ALARMSUBSECONDMASK_SS14        ((uint32_t)0x0E000000U)  /*!< SS[14] is don't care in Alarm
                                                                        comparison.Only SS[13:0] are compared  */
#define RTC_ALARMSUBSECONDMASK_NONE        ((uint32_t)0x0F000000U)  /*!< SS[14:0] are compared and must match
                                                                        to activate alarm. */
#define RTC_IT_TS                         ((uint32_t)0x00008000U)
#define RTC_IT_WUT                        ((uint32_t)0x00004000U)
#define RTC_IT_ALRB                       ((uint32_t)0x00002000U)
#define RTC_IT_ALRA                       ((uint32_t)0x00001000U)
#define RTC_IT_TAMP                       ((uint32_t)0x00000004U) /* Used only to Enable the Tamper Interrupt */
#define RTC_IT_TAMP1                      ((uint32_t)0x00020000U)
#define RTC_IT_TAMP2                      ((uint32_t)0x00040000U)

#define RTC_FLAG_RECALPF                  ((uint32_t)0x00010000U)
#define RTC_FLAG_TAMP2F                   ((uint32_t)0x00004000U)
#define RTC_FLAG_TAMP1F                   ((uint32_t)0x00002000U)
#define RTC_FLAG_TSOVF                    ((uint32_t)0x00001000U)
#define RTC_FLAG_TSF                      ((uint32_t)0x00000800U)
#define RTC_FLAG_WUTF                     ((uint32_t)0x00000400U)
#define RTC_FLAG_ALRBF                    ((uint32_t)0x00000200U)
#define RTC_FLAG_ALRAF                    ((uint32_t)0x00000100U)
#define RTC_FLAG_INITF                    ((uint32_t)0x00000040U)
#define RTC_FLAG_RSF                      ((uint32_t)0x00000020U)
#define RTC_FLAG_INITS                    ((uint32_t)0x00000010U)
#define RTC_FLAG_SHPF                     ((uint32_t)0x00000008U)
#define RTC_FLAG_WUTWF                    ((uint32_t)0x00000004U)
#define RTC_FLAG_ALRBWF                   ((uint32_t)0x00000002U)
#define RTC_FLAG_ALRAWF                   ((uint32_t)0x00000001U)

#define RTC_TR_RESERVED_MASK    ((uint32_t)0x007F7F7FU)
#define RTC_DR_RESERVED_MASK    ((uint32_t)0x00FFFF3FU)
#define RTC_INIT_MASK           ((uint32_t)0xFFFFFFFFU)
#define RTC_RSF_MASK            ((uint32_t)0xFFFFFF5FU)
#define RTC_FLAGS_MASK          ((uint32_t)(RTC_FLAG_TSOVF | RTC_FLAG_TSF | RTC_FLAG_WUTF | \
                                            RTC_FLAG_ALRBF | RTC_FLAG_ALRAF | RTC_FLAG_INITF | \
                                            RTC_FLAG_RSF | RTC_FLAG_INITS | RTC_FLAG_WUTWF | \
                                            RTC_FLAG_ALRBWF | RTC_FLAG_ALRAWF | RTC_FLAG_TAMP1F | \
                                            RTC_FLAG_RECALPF | RTC_FLAG_SHPF))

uint8_t RTC_ByteToBcd2(uint8_t Value);
uint8_t RTC_Bcd2ToByte(uint8_t Value);

uint32_t STM32_RTC::init()
{
    //STM32_RCC::enable_RTC();
    RCC->BDCR |= RCC_BDCR_RTCEN;

    /* Disable the write protection for RTC registers */
    write_protect_disable();

    if (enter_init_mode() != STM32_RESULT_OK)
    {
        /* Enable the write protection for RTC registers */
        write_protect_enable();
        return STM32_RESULT_FAIL;
    }

    /* Clear RTC_CR FMT, OSEL and POL Bits */
    RTC->CR &= ~(RTC_CR_FMT | RTC_CR_OSEL | RTC_CR_POL);
    /* Set RTC_CR register */
    RTC->CR |= (STM32_RTC_FORMAT |
                STM32_RTC_OUTPUT |
                STM32_RTC_OUTPUT_POL |
                RTC_CR_BYPSHAD);

    /* Configure the RTC PRER */
    RTC->PRER = STM32_RTC_SYNC_PREDIV | (STM32_RTC_ASYNC_PREDIV << 16U);

    /* Exit Initialization mode */
    RTC->ISR &= ~RTC_ISR_INIT;

    RTC->TAFCR &= ~RTC_TAFCR_ALARMOUTTYPE;
    RTC->TAFCR |= STM32_RTC_OUTPUT_TYPE;

    /* Enable the write protection for RTC registers */
    write_protect_enable();

    return STM32_RESULT_OK;
}

uint32_t STM32_RTC::deinit()
{
    /* Disable the write protection for RTC registers */
    write_protect_disable();

    /* Set Initialization mode */
    if (enter_init_mode() != STM32_RESULT_OK)
    {
        /* Enable the write protection for RTC registers */
        write_protect_enable();
        return STM32_RESULT_FAIL;
    }
    else
    {
        /* Reset TR, DR and CR registers */
        RTC->TR = (uint32_t)0x00000000U;
        RTC->DR = (uint32_t)0x00002101U;
        /* Reset All CR bits except CR[2:0] */
        RTC->CR &= (uint32_t)0x00000007U;

        /* Wait till WUTWF flag is set and if Time out is reached exit */
        uint32_t tout_ex = STM32_SYSTICK::get_tick() + RTC_TIMEOUT_VALUE;
        while ((RTC->ISR & RTC_ISR_WUTWF) == RESET)
        {
            if (STM32_SYSTICK::get_tick() > tout_ex)
            {
                /* Enable the write protection for RTC registers */
                write_protect_enable();
                return STM32_RESULT_TIMEOUT;
            }
        }

        /* Reset all RTC CR register bits */
        RTC->CR &= (uint32_t)0x00000000U;
        RTC->WUTR = (uint32_t)0x0000FFFFU;
        RTC->PRER = (uint32_t)0x007F00FFU;
        RTC->CALIBR = (uint32_t)0x00000000U;
        RTC->ALRMAR = (uint32_t)0x00000000U;
        RTC->ALRMBR = (uint32_t)0x00000000U;
        RTC->SHIFTR = (uint32_t)0x00000000U;
        RTC->CALR = (uint32_t)0x00000000U;
        RTC->ALRMASSR = (uint32_t)0x00000000U;
        RTC->ALRMBSSR = (uint32_t)0x00000000U;

        /* Reset ISR register and exit initialization mode */
        RTC->ISR = (uint32_t)0x00000000U;

        /* Reset Tamper and alternate functions configuration register */
        RTC->TAFCR = 0x00000000U;

        /* If  RTC_CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
        if ((RTC->CR & RTC_CR_BYPSHAD) == RESET)
        {
            if (wait_for_synchro() != STM32_RESULT_OK)
            {
                /* Enable the write protection for RTC registers */
                write_protect_enable();
                return STM32_RESULT_FAIL;
            }
        }
    }

    /* Enable the write protection for RTC registers */
    write_protect_enable();

    /* De-Initialize RTC MSP */
    STM32_RCC::disable_RTC();

    return STM32_RESULT_OK;
}

uint32_t STM32_RTC::set_time(STM32_RTC_Time *time, ERTCFormat format)
{
    uint32_t tmpreg = 0U;

    if(format == ERTCFormat::BIN)
    {
        if ((RTC->CR & RTC_CR_FMT) == RESET)
            time->TimeFormat = 0x00U;

        tmpreg = (((uint32_t)RTC_ByteToBcd2(time->Hours) << 16U) |
                  ((uint32_t)RTC_ByteToBcd2(time->Minutes) << 8U) |
                  ((uint32_t)RTC_ByteToBcd2(time->Seconds)) |
                  (((uint32_t)time->TimeFormat) << 16U));
    }
    else
    {
        if ((RTC->CR & RTC_CR_FMT) != RESET)
            tmpreg = RTC_Bcd2ToByte(time->Hours);
        else
            time->TimeFormat = 0x00U;
        tmpreg = (((uint32_t)(time->Hours) << 16U) |
                  ((uint32_t)(time->Minutes) << 8U) |
                  ((uint32_t)time->Seconds) |
                  ((uint32_t)(time->TimeFormat) << 16U));
    }

    /* Disable the write protection for RTC registers */
    write_protect_disable();

    /* Set Initialization mode */
    if (enter_init_mode() != STM32_RESULT_OK)
    {
        /* Enable the write protection for RTC registers */
        write_protect_enable();
        return STM32_RESULT_FAIL;
    }
    else
    {
        /* Set the RTC_TR register */
        RTC->TR = (tmpreg & RTC_TR_RESERVED_MASK);

        /* Clear the bits to be configured */
        RTC->CR &= ~RTC_CR_BCK;

        /* Configure the RTC_CR register */
        RTC->CR |= (time->DayLightSaving | time->StoreOperation);

        /* Exit Initialization mode */
        RTC->ISR &= ~RTC_ISR_INIT;

        /* If  CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
        if ((RTC->CR & RTC_CR_BYPSHAD) == RESET)
        {
            if (wait_for_synchro() != STM32_RESULT_OK)
            {
                /* Enable the write protection for RTC registers */
                write_protect_enable();
                return STM32_RESULT_FAIL;
            }
        }

        /* Enable the write protection for RTC registers */
        write_protect_enable();

        return STM32_RESULT_OK;
    }
}

uint32_t STM32_RTC::get_time(STM32_RTC_Time *time, ERTCFormat format)
{
    uint32_t tmpreg = 0U;

    /* Get subseconds structure field from the corresponding register */
    time->SubSeconds = (RTC->SSR);

    /* Get SecondFraction structure field from the corresponding register field*/
    time->SecondFraction = (RTC->PRER & RTC_PRER_PREDIV_S);

    /* Get the TR register */
    tmpreg = (RTC->TR & RTC_TR_RESERVED_MASK);

    /* Fill the structure fields with the read parameters */
    time->Hours = (tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> 16U;
    time->Minutes = (tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >>8U;
    time->Seconds = tmpreg & (RTC_TR_ST | RTC_TR_SU);
    time->TimeFormat = (tmpreg & (RTC_TR_PM)) >> 16U;

    /* Check the input parameters format */
    if (format == ERTCFormat::BIN)
    {
        /* Convert the time structure parameters to Binary format */
        time->Hours = RTC_Bcd2ToByte(time->Hours);
        time->Minutes = RTC_Bcd2ToByte(time->Minutes);
        time->Seconds = RTC_Bcd2ToByte(time->Seconds);
    }

    return STM32_RESULT_OK;
}

uint32_t STM32_RTC::set_date(STM32_RTC_Date *date, ERTCFormat format)
{
    uint32_t datetmpreg = 0U;

    if ((format == ERTCFormat::BIN) && ((date->Month & 0x10U) == 0x10U))
        date->Month = ((date->Month & (~0x10U)) + 0x0AU);

    if (format == ERTCFormat::BIN)
    {
        datetmpreg = ((RTC_ByteToBcd2(date->Year) << 16U) |
                      (RTC_ByteToBcd2(date->Month) << 8U) |
                      (RTC_ByteToBcd2(date->Date)) |
                      (date->WeekDay << 13U));
    }
    else
    {
        datetmpreg = RTC_Bcd2ToByte(date->Month);
        datetmpreg = RTC_Bcd2ToByte(date->Date);

        datetmpreg = (((date->Year) << 16U) |
                      ((date->Month) << 8U) |
                      (date->Date) |
                      ((date->WeekDay) << 13U));
    }

    /* Disable the write protection for RTC registers */
    write_protect_disable();

    /* Set Initialization mode */
    if(enter_init_mode() != STM32_RESULT_OK)
    {
        /* Enable the write protection for RTC registers */
        write_protect_enable();
        return STM32_RESULT_FAIL;
    }
    else
    {
        /* Set the RTC_DR register */
        RTC->DR = (datetmpreg & RTC_DR_RESERVED_MASK);

        /* Exit Initialization mode */
        RTC->ISR &= ~RTC_ISR_INIT;

        /* If  CR_BYPSHAD bit = 0, wait for synchro else this check is not needed */
        if ((RTC->CR & RTC_CR_BYPSHAD) == RESET)
        {
            if (wait_for_synchro() != STM32_RESULT_OK)
            {
                /* Enable the write protection for RTC registers */
                write_protect_enable();
                return STM32_RESULT_FAIL;
            }
        }

        /* Enable the write protection for RTC registers */
        write_protect_enable();

        return STM32_RESULT_OK;
    }
}

uint32_t STM32_RTC::get_date(STM32_RTC_Date *date, ERTCFormat format)
{
    uint32_t datetmpreg = 0U;

    /* Get the DR register */
    datetmpreg = (RTC->DR & RTC_DR_RESERVED_MASK);

    /* Fill the structure fields with the read parameters */
    date->Year = (datetmpreg & (RTC_DR_YT | RTC_DR_YU)) >> 16U;
    date->Month = (datetmpreg & (RTC_DR_MT | RTC_DR_MU)) >> 8U;
    date->Date = datetmpreg & (RTC_DR_DT | RTC_DR_DU);
    date->WeekDay = (datetmpreg & (RTC_DR_WDU)) >> 13U;

    /* Check the input parameters format */
    if (format == ERTCFormat::BIN)
    {
        /* Convert the date structure parameters to Binary format */
        date->Year = RTC_Bcd2ToByte(date->Year);
        date->Month = RTC_Bcd2ToByte(date->Month);
        date->Date = RTC_Bcd2ToByte(date->Date);
    }
    return STM32_RESULT_OK;
}

uint32_t STM32_RTC::enter_init_mode()
{
    /* Check if the Initialization mode is set */
    if ((RTC->ISR & RTC_ISR_INITF) == RESET)
    {
        /* Set the Initialization mode */
        RTC->ISR = RTC_INIT_MASK;

        /* Wait till RTC is in INIT state and if Time out is reached exit */
        WAIT_TIMEOUT((RTC->ISR & RTC_ISR_INITF) == RESET, RTC_TIMEOUT_VALUE);
    }

    return STM32_RESULT_OK;
}

uint32_t STM32_RTC::wait_for_synchro()
{
    /* Clear RSF flag */
    RTC->ISR &= RTC_RSF_MASK;

    /* Wait the registers to be synchronised */
    WAIT_TIMEOUT((RTC->ISR & RTC_ISR_RSF) == RESET, RTC_TIMEOUT_VALUE);

    return STM32_RESULT_OK;
}

uint8_t RTC_ByteToBcd2(uint8_t Value)
{
    uint32_t bcdhigh = 0U;

    while(Value >= 10U)
    {
        bcdhigh++;
        Value -= 10U;
    }

    return  ((uint8_t)(bcdhigh << 4U) | Value);
}

uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
    uint32_t tmp = 0U;
    tmp = ((uint8_t)(Value & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U;
    return (tmp + (Value & (uint8_t)0x0FU));
}
