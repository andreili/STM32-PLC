#ifndef __STM32_RCC__
#define __STM32_RCC__

#include <stm32_conf.h>

#define RCC_APB2LPENR_ADCLPEN RCC_APB2LPENR_ADC1LPEN
#define RCC_FLAG_MASK  ((uint8_t)0x1FU)

/* Flags in the CR register */
#define RCC_FLAG_HSIRDY                  ((uint8_t)0x21U)
#define RCC_FLAG_HSERDY                  ((uint8_t)0x31U)
#define RCC_FLAG_PLLRDY                  ((uint8_t)0x39U)
#define RCC_FLAG_PLLI2SRDY               ((uint8_t)0x3BU)

/* Flags in the BDCR register */
#define RCC_FLAG_LSERDY                  ((uint8_t)0x41U)

/* Flags in the CSR register */
#define RCC_FLAG_LSIRDY                  ((uint8_t)0x61U)
#define RCC_FLAG_BORRST                  ((uint8_t)0x79U)
#define RCC_FLAG_PINRST                  ((uint8_t)0x7AU)
#define RCC_FLAG_PORRST                  ((uint8_t)0x7BU)
#define RCC_FLAG_SFTRST                  ((uint8_t)0x7CU)
#define RCC_FLAG_IWDGRST                 ((uint8_t)0x7DU)
#define RCC_FLAG_WWDGRST                 ((uint8_t)0x7EU)
#define RCC_FLAG_LPWRRST                 ((uint8_t)0x7FU)

#define RCC_IT_LSIRDY                    ((uint8_t)0x01U)
#define RCC_IT_LSERDY                    ((uint8_t)0x02U)
#define RCC_IT_HSIRDY                    ((uint8_t)0x04U)
#define RCC_IT_HSERDY                    ((uint8_t)0x08U)
#define RCC_IT_PLLRDY                    ((uint8_t)0x10U)
#define RCC_IT_PLLI2SRDY                 ((uint8_t)0x20U)
#define RCC_IT_CSS                       ((uint8_t)0x80U)

#define RCC_CIR_OFFSET                   0x0c

#define RCC_MCO1                         ((uint32_t)0x00000000U)
#define RCC_MCO2                         ((uint32_t)0x00000001U)

#define RCC_PERIPHCLK_I2S             ((uint32_t)0x00000001U)
#define RCC_PERIPHCLK_SAI_PLLI2S      ((uint32_t)0x00000002U)
#define RCC_PERIPHCLK_SAI_PLLSAI      ((uint32_t)0x00000004U)
#define RCC_PERIPHCLK_LTDC            ((uint32_t)0x00000008U)
#define RCC_PERIPHCLK_TIM             ((uint32_t)0x00000010U)
#define RCC_PERIPHCLK_RTC             ((uint32_t)0x00000020U)
#define RCC_PERIPHCLK_PLLI2S          ((uint32_t)0x00000040U)

#define RCC_I2SCLKSOURCE_PLLI2S         ((uint32_t)0x00000000U)
#define RCC_I2SCLKSOURCE_EXT            ((uint32_t)0x00000001U)
#define RCC_PLLSAIDIVR_2                ((uint32_t)0x00000000U)
#define RCC_PLLSAIDIVR_4                ((uint32_t)0x00010000U)
#define RCC_PLLSAIDIVR_8                ((uint32_t)0x00020000U)
#define RCC_PLLSAIDIVR_16               ((uint32_t)0x00030000U)
#define RCC_SAIACLKSOURCE_PLLSAI        ((uint32_t)0x00000000U)
#define RCC_SAIACLKSOURCE_PLLI2S        ((uint32_t)0x00100000U)
#define RCC_SAIACLKSOURCE_EXT           ((uint32_t)0x00200000U)
#define RCC_SAIBCLKSOURCE_PLLSAI        ((uint32_t)0x00000000U)
#define RCC_SAIBCLKSOURCE_PLLI2S        ((uint32_t)0x00400000U)
#define RCC_SAIBCLKSOURCE_EXT           ((uint32_t)0x00800000U)
#define RCC_MCO2SOURCE_SYSCLK           ((uint32_t)0x00000000U)
#define RCC_MCO2SOURCE_PLLI2SCLK        RCC_CFGR_MCO2_0
#define RCC_MCO2SOURCE_HSE              RCC_CFGR_MCO2_1
#define RCC_MCO2SOURCE_PLLCLK           RCC_CFGR_MCO2

#define RCC_TIMPRES_DESACTIVATED        ((uint8_t)0x00U)
#define RCC_TIMPRES_ACTIVATED           ((uint8_t)0x01U)

typedef struct
{
    uint32_t    selector;
    uint32_t    PLLI2S;
    uint32_t    PLLSAI;
    uint32_t    PLLI2SDivQ;
    uint32_t    PLLSAIDivQ;
    uint32_t    PLLSAIDivR;
    uint32_t    RTCClockSelection;
    uint32_t    TIMPresSelection;

    uint32_t    PLLSAIN;
    uint32_t    PLLSAIQ;
    uint32_t    PLLSAIR;

    uint32_t    PLLI2SN;
    uint32_t    PLLI2SR;
    uint32_t    PLLI2SQ;
} RCC_Periph_Clock_Source;

class STM32_RCC
{
public:
    static void init();
    static void deinit();

    ENDIS_REG_FLAG(HSI, RCC->CR, RCC_CR_HSION)
    static inline void set_calibration_value_HSI(uint32_t value) { MODIFY_REG(RCC->CR, RCC_CR_HSITRIM,
                                                                              value << POSITION_VAL(RCC_CR_HSITRIM)); }
    ENDIS_REG_FLAG(LSI, RCC->CSR, RCC_CSR_LSION)

    static inline void on_HSE() { BIT_BAND_PER(RCC->CR, RCC_CR_HSEON) = ENABLE; }
    static inline void on_HSE_BYPASS() { RCC->CR |= (RCC_CR_HSEBYP | RCC_CR_HSEON); }
    static inline void off_HSE() { RCC->CR &= ~(RCC_CR_HSEBYP | RCC_CR_HSEON); }
    static void config_HSE(uint32_t state);

    static void config_LSE(uint32_t state);

    ENDIS_REG_FLAG(RTC, RCC->BDCR, RCC_BDCR_RTCEN)
    static inline bool get_enabled_RTC() { return ((RCC->BDCR & RCC_BDCR_RTCEN) == RCC_BDCR_RTCEN); }
    static void set_prescaler_RTC(uint32_t value);
    static void set_config_RTC(uint32_t value);

    static inline void set_TIM_prescaler(uint32_t val) { MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_TIMPRE_Msk, val << RCC_DCKCFGR_TIMPRE_Pos); }

    static inline void force_reset_backup() { BIT_BAND_PER(RCC->BDCR, RCC_BDCR_BDRST) = ENABLE; }
    static inline void release_reset_backup() { BIT_BAND_PER(RCC->BDCR, RCC_BDCR_BDRST) = DISABLE; }

    ENDIS_REG_FLAG(PLL, RCC->CR, RCC_CR_PLLON)
    static inline void set_config_PLL_source(uint32_t value) { MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, value); }
    static inline uint32_t get_source_PLL_OSC() { return RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC; }
    static inline void set_config_PLL_PLLM(uint32_t value) { MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, value); }

    static inline void set_config_SYSCLK(uint32_t value) { MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, value); }
    static inline uint32_t get_source_SYSCLK() { return (RCC->CFGR & RCC_CFGR_SWS); }

    static inline void set_config_MCO1(uint32_t source, uint32_t div)
        { MODIFY_REG(RCC->CFGR, (RCC_CFGR_MCO1 | RCC_CFGR_MCO1PRE), (source | div)); }

    static inline void set_config_MCO2(uint32_t source, uint32_t div)
        { MODIFY_REG(RCC->CFGR, (RCC_CFGR_MCO2 | RCC_CFGR_MCO2PRE), (source | (div << 3))); }

    static inline void enable_IT(uint32_t value) { *((__IO uint8_t*)(RCC_BASE + RCC_CIR_OFFSET + 1)) |= value; }
    static inline void disable_IT(uint32_t value) { *((__IO uint8_t*)(RCC_BASE + RCC_CIR_OFFSET + 1)) &= ~value; }
    static inline void clear_IT(uint32_t value) { *((__IO uint8_t*)(RCC_BASE + RCC_CIR_OFFSET + 2)) = value; }
    static inline bool get_IT(uint32_t value) { return (RCC->CIR & value) == value; }

    static inline void clear_reset_flags() { RCC->CSR |= RCC_CSR_RMVF; }

    static bool get_flag(uint32_t value);

    static uint32_t config_osc();
    static uint32_t config_clock(uint32_t flash_latency);
    
    static inline uint32_t get_HCLK_freq() { return m_system_core_clock; }
    static uint32_t get_PCLK1_freq();
    static uint32_t get_PCLK2_freq();

    static void config_MCO(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv);

    ENDIS_REG_FLAG(CSS, RCC->CR, RCC_CR_CSSON)

    CLK_ENDIS(AHB1, GPIOA)
    CLK_ENDIS(AHB1, GPIOB)
    CLK_ENDIS(AHB1, GPIOC)
    CLK_ENDIS(AHB1, GPIOD)
    CLK_ENDIS(AHB1, GPIOE)
    CLK_ENDIS(AHB1, GPIOF)
    CLK_ENDIS(AHB1, GPIOG)
    CLK_ENDIS(AHB1, GPIOH)
    CLK_ENDIS(AHB1, GPIOI)
    CLK_ENDIS(AHB1, GPIOJ)
    CLK_ENDIS(AHB1, GPIOK)
    CLK_ENDIS(AHB1, CRC)
    CLK_ENDIS(AHB1, BKPSRAM)
    CLK_ENDIS(AHB1, CCMDATARAM)
    CLK_ENDIS(AHB1, DMA1)
    CLK_ENDIS(AHB1, DMA2)
    CLK_ENDIS(AHB1, DMA2D)
    CLK_ENDIS(AHB1, ETHMAC)
    CLK_ENDIS(AHB1, ETHMACTX)
    CLK_ENDIS(AHB1, ETHMACRX)
    CLK_ENDIS(AHB1, ETHMACPTP)
    CLK_ENDIS(AHB1, OTGHS)
    CLK_ENDIS(AHB1, OTGHSULPI)
    CLK_ENDIS(AHB2, DCMI)
    CLK_ENDIS(AHB2, RNG)
    CLK_ENDIS(AHB2, OTGFS)

    CLK_ENDIS(AHB3, FMC)
    CLK_ENDIS(APB1, TIM2)
    CLK_ENDIS(APB1, TIM3)
    CLK_ENDIS(APB1, TIM4)
    CLK_ENDIS(APB1, TIM5)
    CLK_ENDIS(APB1, TIM6)
    CLK_ENDIS(APB1, TIM7)
    CLK_ENDIS(APB1, TIM12)
    CLK_ENDIS(APB1, TIM13)
    CLK_ENDIS(APB1, TIM14)
    CLK_ENDIS(APB1, WWDG)
    CLK_ENDIS(APB1, SPI2)
    CLK_ENDIS(APB1, SPI3)
    CLK_ENDIS(APB1, USART2)
    CLK_ENDIS(APB1, USART3)
    CLK_ENDIS(APB1, UART4)
    CLK_ENDIS(APB1, UART5)
    CLK_ENDIS(APB1, I2C1)
    CLK_ENDIS(APB1, I2C2)
    CLK_ENDIS(APB1, I2C3)
    CLK_ENDIS(APB1, CAN1)
    CLK_ENDIS(APB1, CAN2)
    CLK_ENDIS(APB1, PWR)
    CLK_ENDIS(APB1, DAC)
    CLK_ENDIS(APB1, UART7)
    CLK_ENDIS(APB1, UART8)

    CLK_ENDIS(APB2, TIM1)
    CLK_ENDIS(APB2, TIM8)
    CLK_ENDIS(APB2, USART1)
    CLK_ENDIS(APB2, USART6)
    CLK_ENDIS(APB2, ADC1)
    CLK_ENDIS(APB2, ADC2)
    CLK_ENDIS(APB2, ADC3)
    CLK_ENDIS(APB2, SDIO)
    CLK_ENDIS(APB2, SPI1)
    CLK_ENDIS(APB2, SPI4)
    CLK_ENDIS(APB2, SYSCFG)
    CLK_ENDIS(APB2, TIM9)
    CLK_ENDIS(APB2, TIM10)
    CLK_ENDIS(APB2, TIM11)
    CLK_ENDIS(APB2, SPI5)
    CLK_ENDIS(APB2, SPI6)
    CLK_ENDIS(APB2, SAI1)
    CLK_ENDIS(APB2, LTDC)

    static inline void force_reset_AHB1() { RCC->AHB1RSTR = 0xFFFFFFFFU; }
    static inline void release_reset_AHB1() { RCC->AHB1RSTR = 0x00U; }
    PER_RESET_SLEEP(AHB1, GPIOA)
    PER_RESET_SLEEP(AHB1, GPIOB)
    PER_RESET_SLEEP(AHB1, GPIOC)
    PER_RESET_SLEEP(AHB1, GPIOH)
    PER_RESET_SLEEP(AHB1, DMA1)
    PER_RESET_SLEEP(AHB1, DMA2)

    static inline void force_reset_APB1() { RCC->APB1RSTR = 0xFFFFFFFFU; }
    static inline void release_reset_APB1() { RCC->APB1RSTR = 0x00U; }
    PER_RESET_SLEEP(APB1, TIM5)
    PER_RESET_SLEEP(APB1, WWDG)
    PER_RESET_SLEEP(APB1, SPI2)
    PER_RESET_SLEEP(APB1, USART2)
    PER_RESET_SLEEP(APB1, I2C1)
    PER_RESET_SLEEP(APB1, I2C2)
    PER_RESET_SLEEP(APB1, PWR)

    static inline void force_reset_APB2() { RCC->APB2RSTR = 0xFFFFFFFFU; }
    static inline void release_reset_APB2() { RCC->APB2RSTR = 0x00U; }
    PER_RESET_SLEEP(APB2, TIM1)
    PER_RESET_SLEEP(APB2, USART1)
    PER_RESET_SLEEP(APB2, USART6)
    PER_RESET_SLEEP(APB2, ADC)
    PER_RESET_SLEEP(APB2, SPI1)
    PER_RESET_SLEEP(APB2, SYSCFG)
    PER_RESET_SLEEP(APB2, TIM9)
    PER_RESET_SLEEP(APB2, TIM11)

    static void NMI_IRQ_Handler();

    /* RCCex */
    static uint32_t periph_CLK_config(RCC_Periph_Clock_Source *sources);

    ENDIS_REG_FLAG_NAME(PLLSAI_IT, RCC->CIR, RCC_CIR_PLLSAIRDYIE)
    static inline void clear_PLLSAI_IT() { clear_IT(RCC_CIR_PLLSAIRDYF); }
    static bool get_PLLSAI_IT() { return get_IT(RCC_CIR_PLLSAIRDYIE); }
    static bool get_PLLSAI_flag() { return ((RCC->CR & RCC_CR_PLLSAIRDY) == RCC_CR_PLLSAIRDY); }

    static inline void PLLI2S_SAI_config(uint32_t sn, uint32_t sq, uint32_t sr)
        { RCC->PLLI2SCFGR = (sn << RCC_PLLI2SCFGR_PLLI2SN_Pos) |
                (sq << RCC_PLLI2SCFGR_PLLI2SQ_Pos) |
                (sr << RCC_PLLI2SCFGR_PLLI2SR_Pos); }
    static inline void PLLI2S_SAI_configQ(uint32_t sq)
        { MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_PLLI2SDIVQ, sq); }

    static inline void PLLSAI_config(uint32_t sn, uint32_t sq, uint32_t sr)
        { RCC->PLLSAICFGR = (sn << RCC_PLLSAICFGR_PLLSAIN_Pos) |
                (sq << RCC_PLLSAICFGR_PLLSAIQ_Pos) |
                (sr << RCC_PLLSAICFGR_PLLSAIR_Pos); }
    static inline void PLLSAI_configQ(uint32_t sq)
        { MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVQ, ((sq - 1) << 8)); }
    static inline void PLLSAI_configR(uint32_t sr)
        { MODIFY_REG(RCC->DCKCFGR, RCC_DCKCFGR_PLLSAIDIVR, sr); }

    ENDIS_REG_FLAG(PLLI2S, RCC->CR, RCC_CR_PLLI2SON)
    ENDIS_REG_FLAG(PLLSAI, RCC->CR, RCC_CR_PLLSAION)
private:
    static uint32_t m_system_core_clock;

    static uint32_t update_system_core_clock();
};

#endif
