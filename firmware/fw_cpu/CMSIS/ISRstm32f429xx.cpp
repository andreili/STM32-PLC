
/*********************************************************
 * ISRstm32f429xx.cpp
 * Automatic generated source file for MCU stm32f429xx
 *
 *
 **/
#include "ISRstm32f429xx.h"
__attribute__((naked, noreturn)) void ISR::DefaultHandler()
{
   for(;;)
	   *((uint32_t*)0x02000000) = 01;
}


#ifdef USE_MEMORY_ISR
__attribute__((section(".isr_vector"))) const volatile ISR::ShortVectors interruptsVectorTable =
#else
__attribute__((section(".isr_vector"))) const volatile ISR::Vectors interruptsVectorTable =
#endif
{
#ifdef USE_MEMORY_ISR
   _estack,
   ISR::Reset
};

__attribute__((section(".isr_vector2"))) ISR::Vectors MeminterruptsVectorTable =
{
#endif
   (uint32_t)&_estack,
   ISR::Reset,
   ISR::NMI,
   ISR::HardFault,
   ISR::MemManage,
   ISR::BusFault,
   ISR::UsageFault,
   {0, 0, 0, 0},
   ISR::SVC,
   ISR::DebugMon,
   {0},
   ISR::PendSV,
   ISR::SysTickTimer,
   ISR::WWDG_IRQ,
   ISR::PVD_IRQ,
   ISR::TAMP_STAMP_IRQ,
   ISR::RTC_WKUP_IRQ,
   ISR::FLASH_IRQ,
   ISR::RCC_IRQ,
   ISR::EXTI0_IRQ,
   ISR::EXTI1_IRQ,
   ISR::EXTI2_IRQ,
   ISR::EXTI3_IRQ,
   ISR::EXTI4_IRQ,
   ISR::DMA1_Stream0_IRQ,
   ISR::DMA1_Stream1_IRQ,
   ISR::DMA1_Stream2_IRQ,
   ISR::DMA1_Stream3_IRQ,
   ISR::DMA1_Stream4_IRQ,
   ISR::DMA1_Stream5_IRQ,
   ISR::DMA1_Stream6_IRQ,
   ISR::ADC_IRQ,
   ISR::CAN1_TX_IRQ,
   ISR::CAN1_RX0_IRQ,
   ISR::CAN1_RX1_IRQ,
   ISR::CAN1_SCE_IRQ,
   ISR::EXTI9_5_IRQ,
   ISR::TIM1_BRK_TIM9_IRQ,
   ISR::TIM1_UP_TIM10_IRQ,
   ISR::TIM1_TRG_COM_TIM11_IRQ,
   ISR::TIM1_CC_IRQ,
   ISR::TIM2_IRQ,
   ISR::TIM3_IRQ,
   ISR::TIM4_IRQ,
   ISR::I2C1_EV_IRQ,
   ISR::I2C1_ER_IRQ,
   ISR::I2C2_EV_IRQ,
   ISR::I2C2_ER_IRQ,
   ISR::SPI1_IRQ,
   ISR::SPI2_IRQ,
   ISR::USART1_IRQ,
   ISR::USART2_IRQ,
   ISR::USART3_IRQ,
   ISR::EXTI15_10_IRQ,
   ISR::RTC_Alarm_IRQ,
   ISR::OTG_FS_WKUP_IRQ,
   ISR::TIM8_BRK_TIM12_IRQ,
   ISR::TIM8_UP_TIM13_IRQ,
   ISR::TIM8_TRG_COM_TIM14_IRQ,
   ISR::TIM8_CC_IRQ,
   ISR::DMA1_Stream7_IRQ,
   ISR::FMC_IRQ,
   ISR::SDIO_IRQ,
   ISR::TIM5_IRQ,
   ISR::SPI3_IRQ,
   ISR::UART4_IRQ,
   ISR::UART5_IRQ,
   ISR::TIM6_DAC_IRQ,
   ISR::TIM7_IRQ,
   ISR::DMA2_Stream0_IRQ,
   ISR::DMA2_Stream1_IRQ,
   ISR::DMA2_Stream2_IRQ,
   ISR::DMA2_Stream3_IRQ,
   ISR::DMA2_Stream4_IRQ,
   ISR::ETH_IRQ,
   ISR::ETH_WKUP_IRQ,
   ISR::CAN2_TX_IRQ,
   ISR::CAN2_RX0_IRQ,
   ISR::CAN2_RX1_IRQ,
   ISR::CAN2_SCE_IRQ,
   ISR::OTG_FS_IRQ,
   ISR::DMA2_Stream5_IRQ,
   ISR::DMA2_Stream6_IRQ,
   ISR::DMA2_Stream7_IRQ,
   ISR::USART6_IRQ,
   ISR::I2C3_EV_IRQ,
   ISR::I2C3_ER_IRQ,
   ISR::OTG_HS_EP1_OUT_IRQ,
   ISR::OTG_HS_EP1_IN_IRQ,
   ISR::OTG_HS_WKUP_IRQ,
   ISR::OTG_HS_IRQ,
   ISR::DCMI_IRQ,
   {0},
   ISR::HASH_RNG_IRQ,
   ISR::FPU_IRQ,
   ISR::UART7_IRQ,
   ISR::UART8_IRQ,
   ISR::SPI4_IRQ,
   ISR::SPI5_IRQ,
   ISR::SPI6_IRQ,
   ISR::SAI1_IRQ,
   ISR::LTDC_IRQ,
   ISR::LTDC_ER_IRQ,
   ISR::DMA2D_IRQ,

};
#pragma weak Reset                = DefaultHandler 
#pragma weak NMI                  = DefaultHandler 
#pragma weak HardFault            = DefaultHandler 
#pragma weak MemManage            = DefaultHandler 
#pragma weak BusFault             = DefaultHandler 
#pragma weak UsageFault           = DefaultHandler 
#pragma weak SVC                  = DefaultHandler 
#pragma weak DebugMon             = DefaultHandler 
#pragma weak PendSV               = DefaultHandler 
#pragma weak SysTickTimer         = DefaultHandler 
#pragma weak WWDG_IRQ             = DefaultHandler 
#pragma weak PVD_IRQ              = DefaultHandler 
#pragma weak TAMP_STAMP_IRQ       = DefaultHandler 
#pragma weak RTC_WKUP_IRQ         = DefaultHandler 
#pragma weak FLASH_IRQ            = DefaultHandler 
#pragma weak RCC_IRQ              = DefaultHandler 
#pragma weak EXTI0_IRQ            = DefaultHandler 
#pragma weak EXTI1_IRQ            = DefaultHandler 
#pragma weak EXTI2_IRQ            = DefaultHandler 
#pragma weak EXTI3_IRQ            = DefaultHandler 
#pragma weak EXTI4_IRQ            = DefaultHandler 
#pragma weak DMA1_Stream0_IRQ     = DefaultHandler 
#pragma weak DMA1_Stream1_IRQ     = DefaultHandler 
#pragma weak DMA1_Stream2_IRQ     = DefaultHandler 
#pragma weak DMA1_Stream3_IRQ     = DefaultHandler 
#pragma weak DMA1_Stream4_IRQ     = DefaultHandler 
#pragma weak DMA1_Stream5_IRQ     = DefaultHandler 
#pragma weak DMA1_Stream6_IRQ     = DefaultHandler 
#pragma weak ADC_IRQ              = DefaultHandler 
#pragma weak CAN1_TX_IRQ          = DefaultHandler 
#pragma weak CAN1_RX0_IRQ         = DefaultHandler 
#pragma weak CAN1_RX1_IRQ         = DefaultHandler 
#pragma weak CAN1_SCE_IRQ         = DefaultHandler 
#pragma weak EXTI9_5_IRQ          = DefaultHandler 
#pragma weak TIM1_BRK_TIM9_IRQ    = DefaultHandler 
#pragma weak TIM1_UP_TIM10_IRQ    = DefaultHandler 
#pragma weak TIM1_TRG_COM_TIM11_IRQ = DefaultHandler 
#pragma weak TIM1_CC_IRQ          = DefaultHandler 
#pragma weak TIM2_IRQ             = DefaultHandler 
#pragma weak TIM3_IRQ             = DefaultHandler 
#pragma weak TIM4_IRQ             = DefaultHandler 
#pragma weak I2C1_EV_IRQ          = DefaultHandler 
#pragma weak I2C1_ER_IRQ          = DefaultHandler 
#pragma weak I2C2_EV_IRQ          = DefaultHandler 
#pragma weak I2C2_ER_IRQ          = DefaultHandler 
#pragma weak SPI1_IRQ             = DefaultHandler 
#pragma weak SPI2_IRQ             = DefaultHandler 
#pragma weak USART1_IRQ           = DefaultHandler 
#pragma weak USART2_IRQ           = DefaultHandler 
#pragma weak USART3_IRQ           = DefaultHandler 
#pragma weak EXTI15_10_IRQ        = DefaultHandler 
#pragma weak RTC_Alarm_IRQ        = DefaultHandler 
#pragma weak OTG_FS_WKUP_IRQ      = DefaultHandler 
#pragma weak TIM8_BRK_TIM12_IRQ   = DefaultHandler 
#pragma weak TIM8_UP_TIM13_IRQ    = DefaultHandler 
#pragma weak TIM8_TRG_COM_TIM14_IRQ = DefaultHandler 
#pragma weak TIM8_CC_IRQ          = DefaultHandler 
#pragma weak DMA1_Stream7_IRQ     = DefaultHandler 
#pragma weak FMC_IRQ              = DefaultHandler 
#pragma weak SDIO_IRQ             = DefaultHandler 
#pragma weak TIM5_IRQ             = DefaultHandler 
#pragma weak SPI3_IRQ             = DefaultHandler 
#pragma weak UART4_IRQ            = DefaultHandler 
#pragma weak UART5_IRQ            = DefaultHandler 
#pragma weak TIM6_DAC_IRQ         = DefaultHandler 
#pragma weak TIM7_IRQ             = DefaultHandler 
#pragma weak DMA2_Stream0_IRQ     = DefaultHandler 
#pragma weak DMA2_Stream1_IRQ     = DefaultHandler 
#pragma weak DMA2_Stream2_IRQ     = DefaultHandler 
#pragma weak DMA2_Stream3_IRQ     = DefaultHandler 
#pragma weak DMA2_Stream4_IRQ     = DefaultHandler 
#pragma weak ETH_IRQ              = DefaultHandler 
#pragma weak ETH_WKUP_IRQ         = DefaultHandler 
#pragma weak CAN2_TX_IRQ          = DefaultHandler 
#pragma weak CAN2_RX0_IRQ         = DefaultHandler 
#pragma weak CAN2_RX1_IRQ         = DefaultHandler 
#pragma weak CAN2_SCE_IRQ         = DefaultHandler 
#pragma weak OTG_FS_IRQ           = DefaultHandler 
#pragma weak DMA2_Stream5_IRQ     = DefaultHandler 
#pragma weak DMA2_Stream6_IRQ     = DefaultHandler 
#pragma weak DMA2_Stream7_IRQ     = DefaultHandler 
#pragma weak USART6_IRQ           = DefaultHandler 
#pragma weak I2C3_EV_IRQ          = DefaultHandler 
#pragma weak I2C3_ER_IRQ          = DefaultHandler 
#pragma weak OTG_HS_EP1_OUT_IRQ   = DefaultHandler 
#pragma weak OTG_HS_EP1_IN_IRQ    = DefaultHandler 
#pragma weak OTG_HS_WKUP_IRQ      = DefaultHandler 
#pragma weak OTG_HS_IRQ           = DefaultHandler 
#pragma weak DCMI_IRQ             = DefaultHandler 
#pragma weak HASH_RNG_IRQ         = DefaultHandler 
#pragma weak FPU_IRQ              = DefaultHandler 
#pragma weak UART7_IRQ            = DefaultHandler 
#pragma weak UART8_IRQ            = DefaultHandler 
#pragma weak SPI4_IRQ             = DefaultHandler 
#pragma weak SPI5_IRQ             = DefaultHandler 
#pragma weak SPI6_IRQ             = DefaultHandler 
#pragma weak SAI1_IRQ             = DefaultHandler 
#pragma weak LTDC_IRQ             = DefaultHandler 
#pragma weak LTDC_ER_IRQ          = DefaultHandler 
#pragma weak DMA2D_IRQ            = DefaultHandler 

