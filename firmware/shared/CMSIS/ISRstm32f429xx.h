
/*********************************************************
 * ISRstm32f429xx.h
 * Automatic generated header file for MCU stm32f429xx
 *
 *
 **/
#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdint.h>
extern uint32_t _estack;
//#define USE_MEMORY_ISR

extern "C"
{
    namespace ISR
    {
        void DefaultHandler();
        struct ShortVectors
        {
            uint32_t estack;
            void (*Reset )(void);
        };

        struct Vectors
        {
            uint32_t estack;
            void (*Reset)               (void);
            void (*NMI)                 (void);
            void (*HardFault)           (void);
            void (*MemManage)           (void);
            void (*BusFault)            (void);
            void (*UsageFault)          (void);
            void (*__unused0[4])        (void);
            void (*SVC)                 (void);
            void (*DebugMon)            (void);
            void (*__unused1[1])        (void);
            void (*PendSV)              (void);
            void (*SysTickTimer)        (void);
            void (*WWDG_IRQ)            (void);
            void (*PVD_IRQ)             (void);
            void (*TAMP_STAMP_IRQ)      (void);
            void (*RTC_WKUP_IRQ)        (void);
            void (*FLASH_IRQ)           (void);
            void (*RCC_IRQ)             (void);
            void (*EXTI0_IRQ)           (void);
            void (*EXTI1_IRQ)           (void);
            void (*EXTI2_IRQ)           (void);
            void (*EXTI3_IRQ)           (void);
            void (*EXTI4_IRQ)           (void);
            void (*DMA1_Stream0_IRQ)    (void);
            void (*DMA1_Stream1_IRQ)    (void);
            void (*DMA1_Stream2_IRQ)    (void);
            void (*DMA1_Stream3_IRQ)    (void);
            void (*DMA1_Stream4_IRQ)    (void);
            void (*DMA1_Stream5_IRQ)    (void);
            void (*DMA1_Stream6_IRQ)    (void);
            void (*ADC_IRQ)             (void);
            void (*CAN1_TX_IRQ)         (void);
            void (*CAN1_RX0_IRQ)        (void);
            void (*CAN1_RX1_IRQ)        (void);
            void (*CAN1_SCE_IRQ)        (void);
            void (*EXTI9_5_IRQ)         (void);
            void (*TIM1_BRK_TIM9_IRQ)   (void);
            void (*TIM1_UP_TIM10_IRQ)   (void);
            void (*TIM1_TRG_COM_TIM11_IRQ)(void);
            void (*TIM1_CC_IRQ)         (void);
            void (*TIM2_IRQ)            (void);
            void (*TIM3_IRQ)            (void);
            void (*TIM4_IRQ)            (void);
            void (*I2C1_EV_IRQ)         (void);
            void (*I2C1_ER_IRQ)         (void);
            void (*I2C2_EV_IRQ)         (void);
            void (*I2C2_ER_IRQ)         (void);
            void (*SPI1_IRQ)            (void);
            void (*SPI2_IRQ)            (void);
            void (*USART1_IRQ)          (void);
            void (*USART2_IRQ)          (void);
            void (*USART3_IRQ)          (void);
            void (*EXTI15_10_IRQ)       (void);
            void (*RTC_Alarm_IRQ)       (void);
            void (*OTG_FS_WKUP_IRQ)     (void);
            void (*TIM8_BRK_TIM12_IRQ)  (void);
            void (*TIM8_UP_TIM13_IRQ)   (void);
            void (*TIM8_TRG_COM_TIM14_IRQ)(void);
            void (*TIM8_CC_IRQ)         (void);
            void (*DMA1_Stream7_IRQ)    (void);
            void (*FMC_IRQ)             (void);
            void (*SDIO_IRQ)            (void);
            void (*TIM5_IRQ)            (void);
            void (*SPI3_IRQ)            (void);
            void (*UART4_IRQ)           (void);
            void (*UART5_IRQ)           (void);
            void (*TIM6_DAC_IRQ)        (void);
            void (*TIM7_IRQ)            (void);
            void (*DMA2_Stream0_IRQ)    (void);
            void (*DMA2_Stream1_IRQ)    (void);
            void (*DMA2_Stream2_IRQ)    (void);
            void (*DMA2_Stream3_IRQ)    (void);
            void (*DMA2_Stream4_IRQ)    (void);
            void (*ETH_IRQ)             (void);
            void (*ETH_WKUP_IRQ)        (void);
            void (*CAN2_TX_IRQ)         (void);
            void (*CAN2_RX0_IRQ)        (void);
            void (*CAN2_RX1_IRQ)        (void);
            void (*CAN2_SCE_IRQ)        (void);
            void (*OTG_FS_IRQ)          (void);
            void (*DMA2_Stream5_IRQ)    (void);
            void (*DMA2_Stream6_IRQ)    (void);
            void (*DMA2_Stream7_IRQ)    (void);
            void (*USART6_IRQ)          (void);
            void (*I2C3_EV_IRQ)         (void);
            void (*I2C3_ER_IRQ)         (void);
            void (*OTG_HS_EP1_OUT_IRQ)  (void);
            void (*OTG_HS_EP1_IN_IRQ)   (void);
            void (*OTG_HS_WKUP_IRQ)     (void);
            void (*OTG_HS_IRQ)          (void);
            void (*DCMI_IRQ)            (void);
            void (*__unused2[1])        (void);
            void (*HASH_RNG_IRQ)        (void);
            void (*FPU_IRQ)             (void);
            void (*UART7_IRQ)           (void);
            void (*UART8_IRQ)           (void);
            void (*SPI4_IRQ)            (void);
            void (*SPI5_IRQ)            (void);
            void (*SPI6_IRQ)            (void);
            void (*SAI1_IRQ)            (void);
            void (*LTDC_IRQ)            (void);
            void (*LTDC_ER_IRQ)         (void);
            void (*DMA2D_IRQ)           (void);

        };
        void Reset                (void);
        void NMI                  (void);
        void HardFault            (void);
        void MemManage            (void);
        void BusFault             (void);
        void UsageFault           (void);
        void SVC                  (void);
        void DebugMon             (void);
        void PendSV               (void);
        void SysTickTimer         (void);
        void WWDG_IRQ             (void);
        void PVD_IRQ              (void);
        void TAMP_STAMP_IRQ       (void);
        void RTC_WKUP_IRQ         (void);
        void FLASH_IRQ            (void);
        void RCC_IRQ              (void);
        void EXTI0_IRQ            (void);
        void EXTI1_IRQ            (void);
        void EXTI2_IRQ            (void);
        void EXTI3_IRQ            (void);
        void EXTI4_IRQ            (void);
        void DMA1_Stream0_IRQ     (void);
        void DMA1_Stream1_IRQ     (void);
        void DMA1_Stream2_IRQ     (void);
        void DMA1_Stream3_IRQ     (void);
        void DMA1_Stream4_IRQ     (void);
        void DMA1_Stream5_IRQ     (void);
        void DMA1_Stream6_IRQ     (void);
        void ADC_IRQ              (void);
        void CAN1_TX_IRQ          (void);
        void CAN1_RX0_IRQ         (void);
        void CAN1_RX1_IRQ         (void);
        void CAN1_SCE_IRQ         (void);
        void EXTI9_5_IRQ          (void);
        void TIM1_BRK_TIM9_IRQ    (void);
        void TIM1_UP_TIM10_IRQ    (void);
        void TIM1_TRG_COM_TIM11_IRQ (void);
        void TIM1_CC_IRQ          (void);
        void TIM2_IRQ             (void);
        void TIM3_IRQ             (void);
        void TIM4_IRQ             (void);
        void I2C1_EV_IRQ          (void);
        void I2C1_ER_IRQ          (void);
        void I2C2_EV_IRQ          (void);
        void I2C2_ER_IRQ          (void);
        void SPI1_IRQ             (void);
        void SPI2_IRQ             (void);
        void USART1_IRQ           (void);
        void USART2_IRQ           (void);
        void USART3_IRQ           (void);
        void EXTI15_10_IRQ        (void);
        void RTC_Alarm_IRQ        (void);
        void OTG_FS_WKUP_IRQ      (void);
        void TIM8_BRK_TIM12_IRQ   (void);
        void TIM8_UP_TIM13_IRQ    (void);
        void TIM8_TRG_COM_TIM14_IRQ (void);
        void TIM8_CC_IRQ          (void);
        void DMA1_Stream7_IRQ     (void);
        void FMC_IRQ              (void);
        void SDIO_IRQ             (void);
        void TIM5_IRQ             (void);
        void SPI3_IRQ             (void);
        void UART4_IRQ            (void);
        void UART5_IRQ            (void);
        void TIM6_DAC_IRQ         (void);
        void TIM7_IRQ             (void);
        void DMA2_Stream0_IRQ     (void);
        void DMA2_Stream1_IRQ     (void);
        void DMA2_Stream2_IRQ     (void);
        void DMA2_Stream3_IRQ     (void);
        void DMA2_Stream4_IRQ     (void);
        void ETH_IRQ              (void);
        void ETH_WKUP_IRQ         (void);
        void CAN2_TX_IRQ          (void);
        void CAN2_RX0_IRQ         (void);
        void CAN2_RX1_IRQ         (void);
        void CAN2_SCE_IRQ         (void);
        void OTG_FS_IRQ           (void);
        void DMA2_Stream5_IRQ     (void);
        void DMA2_Stream6_IRQ     (void);
        void DMA2_Stream7_IRQ     (void);
        void USART6_IRQ           (void);
        void I2C3_EV_IRQ          (void);
        void I2C3_ER_IRQ          (void);
        void OTG_HS_EP1_OUT_IRQ   (void);
        void OTG_HS_EP1_IN_IRQ    (void);
        void OTG_HS_WKUP_IRQ      (void);
        void OTG_HS_IRQ           (void);
        void DCMI_IRQ             (void);
        void HASH_RNG_IRQ         (void);
        void FPU_IRQ              (void);
        void UART7_IRQ            (void);
        void UART8_IRQ            (void);
        void SPI4_IRQ             (void);
        void SPI5_IRQ             (void);
        void SPI6_IRQ             (void);
        void SAI1_IRQ             (void);
        void LTDC_IRQ             (void);
        void LTDC_ER_IRQ          (void);
        void DMA2D_IRQ            (void);

    }

}


#endif // INTERRUPTS_H
