#include "stm32_rcc.h"
#include "stm32_pwr.h"
#include "stm32_systick.h"
#include "stm32_flash.h"
#include "stm32_gpio.h"

uint32_t STM32_RCC::m_system_core_clock;

#define __MCO1_CLK_ENABLE()   enable_clk_GPIOA()
#define MCO1_GPIO_PORT        gpioa
#define MCO1_PIN              GPIO_PIN_8

#define __MCO2_CLK_ENABLE()   enable_clk_GPIOC()
#define MCO2_GPIO_PORT        gpioc
#define MCO2_PIN              GPIO_PIN_9

#define HSI_VALUE ((uint32_t)16000000UL)
#define HSE_VALUE ((uint32_t) 8000000UL)
#define F_CPU 		168000000UL

#define HSE_TIMEOUT_VALUE           100
#define HSI_TIMEOUT_VALUE           ((uint32_t)2U)  /* 2 ms */
#define LSI_TIMEOUT_VALUE           ((uint32_t)2U)  /* 2 ms */
#define RCC_DBP_TIMEOUT_VALUE       ((uint32_t)2U)
#define PLL_TIMEOUT_VALUE           ((uint32_t)2U)
#define LSE_TIMEOUT_VALUE           5000
#define CLOCKSWITCH_TIMEOUT_VALUE   ((uint32_t)5000U) /* 5 s    */

#define RCC_HSE_BYPASS (RCC_CR_HSEBYP | RCC_CR_HSEON)
#define RCC_LSE_BYPASS (RCC_BDCR_LSEBYP | RCC_BDCR_LSEON)

#define RCC_PLL_NONE                     ((uint8_t)0x00U)
#define RCC_PLL_OFF                      ((uint8_t)0x01U)
#define RCC_PLL_ON                       ((uint8_t)0x02U)

#define RCC_PLLP_DIV2                    ((uint32_t)0x00000002U)
#define RCC_PLLP_DIV4                    ((uint32_t)0x00000004U)
#define RCC_PLLP_DIV6                    ((uint32_t)0x00000006U)
#define RCC_PLLP_DIV8                    ((uint32_t)0x00000008U)

#define RCC_PLLSOURCE_HSI                RCC_PLLCFGR_PLLSRC_HSI
#define RCC_PLLSOURCE_HSE                RCC_PLLCFGR_PLLSRC_HSE

#define RCC_CLOCKTYPE_SYSCLK             ((uint32_t)0x00000001U)
#define RCC_CLOCKTYPE_HCLK               ((uint32_t)0x00000002U)
#define RCC_CLOCKTYPE_PCLK1              ((uint32_t)0x00000004U)
#define RCC_CLOCKTYPE_PCLK2              ((uint32_t)0x00000008U)

#define RCC_SYSCLKSOURCE_HSI             RCC_CFGR_SW_HSI
#define RCC_SYSCLKSOURCE_HSE             RCC_CFGR_SW_HSE
#define RCC_SYSCLKSOURCE_PLLCLK          RCC_CFGR_SW_PLL
#define RCC_SYSCLKSOURCE_PLLRCLK         ((uint32_t)(RCC_CFGR_SW_0 | RCC_CFGR_SW_1))

#define RCC_SYSCLKSOURCE_STATUS_HSI     RCC_CFGR_SWS_HSI   /*!< HSI used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_HSE     RCC_CFGR_SWS_HSE   /*!< HSE used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_PLLCLK  RCC_CFGR_SWS_PLL   /*!< PLL used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_PLLRCLK ((uint32_t)(RCC_CFGR_SWS_0 | RCC_CFGR_SWS_1))   /*!< PLLR used as system clock */

const uint8_t APBAHBPrescTable[16] = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

void STM32_RCC::init()
{
    enable_clk_PWR();
    STM32_PWR::set_voltage_scaling_config(PWR_REGULATOR_VOLTAGE_SCALE1);
    if (config_osc() != STM32_RESULT_OK)
        Error_Handler();
    if (config_clock(FLASH_ACR_LATENCY_5WS) != STM32_RESULT_OK)
        Error_Handler();

    STM32_SYSTICK::update_freq();
}

void STM32_RCC::deinit()
{
    m_system_core_clock = HSI_VALUE;
    RCC->CR |= RCC_CR_HSION | RCC_CR_HSITRIM_4;
    RCC->CFGR = 0;
    RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_PLLI2SON);
    
    RCC->PLLCFGR = RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLQ_2;
    
    RCC->PLLI2SCFGR = RCC_PLLI2SCFGR_PLLI2SN_6 | RCC_PLLI2SCFGR_PLLI2SN_7 | RCC_PLLI2SCFGR_PLLI2SR_1;

    RCC->CR &= ~RCC_CR_HSEBYP;
    RCC->CIR = 0;
    
    m_system_core_clock = update_system_core_clock();
}

void STM32_RCC::config_HSE(uint32_t state)
{
    switch (state)
    {
    case RCC_CR_HSEON:
        BIT_BAND_PER(RCC->CR, RCC_CR_HSEON) = ENABLE;
        break;
    case RCC_HSE_BYPASS:
        RCC->CR |= RCC_CR_HSEBYP | RCC_CR_HSEON;
        break;
    default:
        RCC->CR &= ~(RCC_CR_HSEBYP | RCC_CR_HSEON);
        break;
    }
}

void STM32_RCC::config_LSE(uint32_t state)
{
    switch (state)
    {
    case RCC_BDCR_LSEON:
        BIT_BAND_PER(RCC->BDCR, RCC_BDCR_LSEON) = ENABLE;
        break;
    case RCC_LSE_BYPASS:
        RCC->BDCR |= RCC_BDCR_LSEBYP | RCC_BDCR_LSEON;
        break;
    default:
        RCC->BDCR &= ~(RCC_BDCR_LSEBYP | RCC_BDCR_LSEON);
        break;
    }
}

void STM32_RCC::set_prescaler_RTC(uint32_t value)
{
    if ((value & RCC_BDCR_RTCSEL) == RCC_BDCR_RTCSEL)
        MODIFY_REG(RCC->CFGR, RCC_CFGR_RTCPRE, (value & 0xFFFFCFFU));
    else
        BIT_BAND_PER(RCC->CFGR, RCC_CFGR_RTCPRE) = DISABLE;
}

void STM32_RCC::set_config_RTC(uint32_t value)
{
    set_prescaler_RTC(value);
    RCC->BDCR |= value & 0x00000FFFU;
}

uint32_t STM32_RCC::get_PCLK1_freq()
{
    return (get_HCLK_freq() >> APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> POSITION_VAL(RCC_CFGR_PPRE1)]);
}

uint32_t STM32_RCC::config_osc()
{
    #ifdef STM32_USE_HSE
    config_HSE(STM32_HSE_STATE);
    if (STM32_HSE_STATE != 0)
    {
        WAIT_TIMEOUT(get_flag(RCC_FLAG_HSERDY) == RESET, HSE_TIMEOUT_VALUE);
    }
    else
    {
        WAIT_TIMEOUT(get_flag(RCC_FLAG_HSERDY) != RESET, HSE_TIMEOUT_VALUE);
    }
    #elif defined(STM32_USE_HSI)
    if (STM32_HSI_STATE != 0)
    {
        enable_HSI();
        WAIT_TIMEOUT(get_flag(RCC_FLAG_HSIRDY) == RESET, HSI_TIMEOUT_VALUE);
        set_calibration_value_HSI(STM32_HSI_CALIBRATION);
    }
    else
    {
        disable_HSI();
        WAIT_TIMEOUT(get_flag(RCC_FLAG_HSIRDY) != RESET, HSI_TIMEOUT_VALUE);
    }
    #endif

    #ifdef STM32_USE_LSI
    if (STM32_LSI_STATE != 0)
    {
        enable_LSI();
        WAIT_TIMEOUT(get_flag(RCC_FLAG_LSIRDY) == RESET, LSI_TIMEOUT_VALUE);
    }
    else
    {
        disable_LSI();
        WAIT_TIMEOUT(get_flag(RCC_FLAG_LSIRDY) != RESET, LSI_TIMEOUT_VALUE);
    }
    #elif defined(STM32_USE_LSE)
    enable_clk_PWR();
    STM32_PWR::enable_backup_access();
    WAIT_TIMEOUT(STM32_PWR::check_backup_access() == RESET, RCC_DBP_TIMEOUT_VALUE);
    config_LSE(STM32_LSE_STATE);

    if (STM32_LSE_STATE != 0)
    {
        WAIT_TIMEOUT(get_flag(RCC_FLAG_LSERDY) == RESET, LSE_TIMEOUT_VALUE);
    }
    else
    {
        WAIT_TIMEOUT(get_flag(RCC_FLAG_LSERDY) != RESET, LSE_TIMEOUT_VALUE);
    }
    #endif

    #ifdef STM32_USE_PLL
    if (get_source_SYSCLK() != RCC_CFGR_SWS_PLL)
    {
        disable_PLL();
        WAIT_TIMEOUT(get_flag(RCC_FLAG_PLLRDY) != RESET, PLL_TIMEOUT_VALUE);
        if (STM32_PLL_STATE == RCC_PLL_ON)
        {
            WRITE_REG(RCC->PLLCFGR, (STM32_PLL_SOURCE | STM32_PLLM | (STM32_PLLN << POSITION_VAL(RCC_PLLCFGR_PLLN)) |
                      (((STM32_PLLP >> 1U) - 1U) << POSITION_VAL(RCC_PLLCFGR_PLLP)) |
                      (STM32_PLLQ << POSITION_VAL(RCC_PLLCFGR_PLLQ))));
            enable_PLL();
            WAIT_TIMEOUT(get_flag(RCC_FLAG_PLLRDY) == RESET, PLL_TIMEOUT_VALUE);
        }
    }
    else
        return STM32_RESULT_FAIL;
    #endif

    return STM32_RESULT_OK;
}

uint32_t STM32_RCC::config_clock(uint32_t flash_latency)
{
    if (flash_latency > STM32_FLASH::get_latency())
    {
        STM32_FLASH::set_latency(flash_latency);
        if (STM32_FLASH::get_latency() != flash_latency)
            return STM32_RESULT_FAIL;
    }

    if ((STM32_CLOCK_TYPE & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
    {
        MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, STM32_CLOCK_AHB_DIV);
    }

    if ((STM32_CLOCK_TYPE & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
    {
        if (STM32_CLOCK_SYSCLK_SOURCE == RCC_CFGR_SW_HSE)
        {
            if (get_flag(RCC_FLAG_HSERDY) == RESET)
                return STM32_RESULT_FAIL;
        }
        else if ((STM32_CLOCK_SYSCLK_SOURCE == RCC_CFGR_SW_PLL) ||
                 (STM32_CLOCK_SYSCLK_SOURCE == (RCC_CFGR_SW_HSE | RCC_CFGR_SW_PLL)))
        {
            if (get_flag(RCC_FLAG_PLLRDY) == RESET)
                return STM32_RESULT_FAIL;
        }
        else if (get_flag(RCC_FLAG_HSIRDY) == RESET)
            return STM32_RESULT_FAIL;

        set_config_SYSCLK(STM32_CLOCK_SYSCLK_SOURCE);
        switch (STM32_CLOCK_SYSCLK_SOURCE)
        {
        case RCC_SYSCLKSOURCE_HSE:
            WAIT_TIMEOUT(get_source_SYSCLK() != RCC_SYSCLKSOURCE_STATUS_HSE, CLOCKSWITCH_TIMEOUT_VALUE);
            break;
        case RCC_SYSCLKSOURCE_PLLCLK:
            WAIT_TIMEOUT(get_source_SYSCLK() != RCC_SYSCLKSOURCE_STATUS_PLLCLK, CLOCKSWITCH_TIMEOUT_VALUE);
            break;
        default:
            WAIT_TIMEOUT(get_source_SYSCLK() != RCC_SYSCLKSOURCE_STATUS_HSI, CLOCKSWITCH_TIMEOUT_VALUE);
            break;
        }
    }

    if (flash_latency < STM32_FLASH::get_latency())
    {
        STM32_FLASH::set_latency(flash_latency);
        if (STM32_FLASH::get_latency() != flash_latency)
            return STM32_RESULT_FAIL;
    }

    if ((STM32_CLOCK_TYPE & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
    {
        MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, STM32_CLOCK_APB1_DIV);
    }

    if ((STM32_CLOCK_TYPE & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
    {
        MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, STM32_CLOCK_APB2_DIV);
    }

    m_system_core_clock = update_system_core_clock() >> APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE)>> RCC_CFGR_HPRE_Pos];
    STM32_SYSTICK::init();

    return STM32_RESULT_OK;
}

bool STM32_RCC::get_flag(uint32_t value)
{
    __IO uint32_t reg;
    switch (value >> 5U)
    {
    case 0x01U:
        reg = RCC->CR;
        break;
    case 0x02U:
        reg = RCC->BDCR;
        break;
    case 0x03U:
        reg = RCC->CSR;
        break;
    default:
        reg = RCC->CIR;
        break;
    }
    return (reg & (0x01U << (value & RCC_FLAG_MASK)));
}

uint32_t STM32_RCC::get_PCLK2_freq()
{
    return (get_HCLK_freq() >> APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> POSITION_VAL(RCC_CFGR_PPRE2)]);
}

void STM32_RCC::config_MCO(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv)
{
    if (RCC_MCOx == RCC_MCO1)
    {
        __MCO1_CLK_ENABLE();
        MCO1_GPIO_PORT.set_config(MCO1_PIN, GPIO_MODE_AF_PP, GPIO_AF0_MCO, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
        MODIFY_REG(RCC->CFGR, (RCC_CFGR_MCO1 | RCC_CFGR_MCO1PRE), (RCC_MCOSource | RCC_MCODiv));
    }
    #ifdef RCC_CFGR_MCO2
    else
    {
        __MCO2_CLK_ENABLE();
        MCO2_GPIO_PORT.set_config(MCO2_PIN, GPIO_MODE_AF_PP, GPIO_AF0_MCO, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_NOPULL);
        MODIFY_REG(RCC->CFGR, (RCC_CFGR_MCO2 | RCC_CFGR_MCO2PRE), (RCC_MCOSource | RCC_MCODiv));
    }
    #endif
}

void STM32_RCC::NMI_IRQ_Handler()
{
    if (get_IT(RCC_IT_CSS))
    {
        ///TODO: CSSCallback
        clear_IT(RCC_IT_CSS);
    }
}

uint32_t STM32_RCC::update_system_core_clock()
{
    uint32_t tmp = 0,
            pllvco = 0,
            pllp = 2,
            pllsource = 0,
            pllm = 2;
    uint32_t sysclockfreq = 0U;

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp)
    {
    case RCC_CFGR_SWS_HSI:  /* HSI used as system clock source */
        sysclockfreq = HSI_VALUE;
        break;
    case RCC_CFGR_SWS_HSE:  /* HSE used as system clock source */
        sysclockfreq = HSE_VALUE;
        break;
    case RCC_CFGR_SWS_PLL:  /* PLL used as system clock source */
        pllsource = get_source_PLL_OSC() >> 22;
        pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
        if (pllsource != 0)
        {
            /* HSE used as PLL clock source */
            pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
        }
        else
        {
            /* HSI used as PLL clock source */
            pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
        }
        pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
        sysclockfreq = pllvco/pllp;
        break;
    default:
        sysclockfreq = HSI_VALUE;
        break;
    }
    /* Compute HCLK frequency --------------------------------------------------*/
    /* Get HCLK prescaler */
    tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
    /* HCLK frequency */
    sysclockfreq >>= tmp;

    return sysclockfreq;
}
