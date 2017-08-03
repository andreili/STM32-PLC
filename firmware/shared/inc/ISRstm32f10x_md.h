
/*********************************************************
 * ISRstm32f10x_md.h
 * Automatic generated header file for MCU stm32f10x_md
 *
 *
 **/
#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include <stdint.h>
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
            void (*TAMPER_IRQ)          (void);
            void (*RTC_IRQ)             (void);
            void (*FLASH_IRQ)           (void);
            void (*RCC_IRQ)             (void);
            void (*EXTI0_IRQ)           (void);
            void (*EXTI1_IRQ)           (void);
            void (*EXTI2_IRQ)           (void);
            void (*EXTI3_IRQ)           (void);
            void (*EXTI4_IRQ)           (void);
            void (*DMA1_Channel1_IRQ)   (void);
            void (*DMA1_Channel2_IRQ)   (void);
            void (*DMA1_Channel3_IRQ)   (void);
            void (*DMA1_Channel4_IRQ)   (void);
            void (*DMA1_Channel5_IRQ)   (void);
            void (*DMA1_Channel6_IRQ)   (void);
            void (*DMA1_Channel7_IRQ)   (void);
            void (*ADC1_2_IRQ)          (void);
            void (*USB_HP_CAN1_TX_IRQ)  (void);
            void (*USB_LP_CAN1_RX0_IRQ) (void);
            void (*CAN1_RX1_IRQ)        (void);
            void (*CAN1_SCE_IRQ)        (void);
            void (*EXTI9_5_IRQ)         (void);
            void (*TIM1_BRK_IRQ)        (void);
            void (*TIM1_UP_IRQ)         (void);
            void (*TIM1_TRG_COM_IRQ)    (void);
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
            void (*RTCAlarm_IRQ)        (void);
            void (*USBWakeUp_IRQ)       (void);
            void (*__unused2[7])        (void);
            void (*BootRAM)             (void);

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
        void TAMPER_IRQ           (void);
        void RTC_IRQ              (void);
        void FLASH_IRQ            (void);
        void RCC_IRQ              (void);
        void EXTI0_IRQ            (void);
        void EXTI1_IRQ            (void);
        void EXTI2_IRQ            (void);
        void EXTI3_IRQ            (void);
        void EXTI4_IRQ            (void);
        void DMA1_Channel1_IRQ    (void);
        void DMA1_Channel2_IRQ    (void);
        void DMA1_Channel3_IRQ    (void);
        void DMA1_Channel4_IRQ    (void);
        void DMA1_Channel5_IRQ    (void);
        void DMA1_Channel6_IRQ    (void);
        void DMA1_Channel7_IRQ    (void);
        void ADC1_2_IRQ           (void);
        void USB_HP_CAN1_TX_IRQ   (void);
        void USB_LP_CAN1_RX0_IRQ  (void);
        void CAN1_RX1_IRQ         (void);
        void CAN1_SCE_IRQ         (void);
        void EXTI9_5_IRQ          (void);
        void TIM1_BRK_IRQ         (void);
        void TIM1_UP_IRQ          (void);
        void TIM1_TRG_COM_IRQ     (void);
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
        void RTCAlarm_IRQ         (void);
        void USBWakeUp_IRQ        (void);
        void BootRAM              (void);

    }

}


#endif // INTERRUPTS_H
