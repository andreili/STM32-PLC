#include "stm32_rcc.h"
#include "stm32_pwr.h"

uint32_t STM32_RCC::m_system_core_clock;

#define HSI_VALUE ((uint32_t)16000000UL)
#define HSE_VALUE ((uint32_t) 8000000UL)
#define F_CPU 		168000000UL

#define HSE_STARTUP_TIMEOUT 100


const uint8_t APBAHBPrescTable[16] = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

void STM32_RCC::init()
{
    clk_enable_PWR();
    STM32_PWR::set_voltage_scaling_config(PWR_REGULATOR_VOLTAGE_SCALE1);
    //on_HSE();

    update_system_core_clock();
}

void STM32_RCC::deinit()
{
    RCC->CR |= RCC_CR_HSION | RCC_CR_HSITRIM_4;
    RCC->CFGR = 0;
    RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON | RCC_CR_PLLI2SON);
    
    RCC->PLLCFGR = RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLQ_2;
    
    RCC->PLLI2SCFGR = RCC_PLLI2SCFGR_PLLI2SN_6 | RCC_PLLI2SCFGR_PLLI2SN_7 | RCC_PLLI2SCFGR_PLLI2SR_1;
    
    RCC->CR &= ~RCC_CR_HSEBYP;
    RCC->CIR = 0;
    
    update_system_core_clock();
}

uint32_t STM32_RCC::get_PCLK1_freq()
{
    return (get_HCLK_freq() >> APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> POSITION_VAL(RCC_CFGR_PPRE1)]);
}

uint32_t STM32_RCC::get_PCLK2_freq()
{
    return (get_HCLK_freq() >> APBAHBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> POSITION_VAL(RCC_CFGR_PPRE2)]);
}

void STM32_RCC::update_system_core_clock()
{
    uint32_t tmp = 0,
            pllvco = 0,
            pllp = 2,
            pllsource = 0,
            pllm = 2;

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp)
    {
    case 0x00:  /* HSI used as system clock source */
        m_system_core_clock = HSI_VALUE;
        break;
    case 0x04:  /* HSE used as system clock source */
        m_system_core_clock = HSE_VALUE;
        break;
    case 0x08:  /* PLL used as system clock source */
        /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
        SYSCLK = PLL_VCO / PLL_P
        */
        pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
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
        m_system_core_clock = pllvco/pllp;
        break;
    default:
        m_system_core_clock = HSI_VALUE;
        break;
    }
    /* Compute HCLK frequency --------------------------------------------------*/
    /* Get HCLK prescaler */
    tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
    /* HCLK frequency */
    m_system_core_clock >>= tmp;
}
