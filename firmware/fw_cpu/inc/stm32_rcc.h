#ifndef __STM32_RCC__
#define __STM32_RCC__

#include <stdint.h>
#include "stm32f4xx.h"
#include "bitbanding.h"

#define CLK_ENDIS(enr, name) \
    static inline void clk_enable_ ## name() { BIT_BAND_PER(RCC->enr ## ENR, RCC_ ## enr ## ENR_ ## name ## EN) = 1; } \
    static inline void clk_disable_ ## name() { BIT_BAND_PER(RCC->enr ## ENR, RCC_ ## enr ## ENR_ ## name ## EN) = 1; }

class STM32_RCC
{
public:
    static void init();
    static void deinit();

    static inline void on_HSE() { BIT_BAND_PER(RCC->CR, RCC_CR_HSEON) = 1; }
    static inline void on_HSE_BYPASS() { RCC->CR |= (RCC_CR_HSEBYP | RCC_CR_HSEON); }
    static inline void off_HSE() { RCC->CR &= ~(RCC_CR_HSEBYP | RCC_CR_HSEON); }
    
    static inline uint32_t get_HCLK_freq() { return m_system_core_clock; }
    static uint32_t get_PCLK1_freq();
    static uint32_t get_PCLK2_freq();

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
private:
    static uint32_t m_system_core_clock;

    static void update_system_core_clock();
};

#endif
