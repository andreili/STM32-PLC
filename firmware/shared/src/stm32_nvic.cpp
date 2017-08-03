#include "stm32_inc.h"
#ifdef STM32F429xx
#include "plc_io.h"
#endif

/* from core_cm4.h */

FORCE_INLINE uint32_t _NVIC_GetPriorityGrouping(void)
{
  return ((uint32_t)((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos));
}

FORCE_INLINE uint32_t _NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);   /* only values 0..7 are used          */
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(__NVIC_PRIO_BITS)) ?
              (uint32_t)(__NVIC_PRIO_BITS) :
              (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits     = ((PriorityGroupTmp + (uint32_t)(__NVIC_PRIO_BITS)) < (uint32_t)7UL) ?
              (uint32_t)0UL :
              (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(__NVIC_PRIO_BITS));

  return (
           ((PreemptPriority & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
           ((SubPriority     & (uint32_t)((1UL << (SubPriorityBits    )) - 1UL)))
         );
}

void STM32_NVIC::init()
{
    set_priority_grouping(NVIC_PRIORITYGROUP_4);
    set_priority(MemoryManagement_IRQn, 0, 0);
    set_priority(BusFault_IRQn, 0, 0);
    set_priority(UsageFault_IRQn, 0, 0);
    set_priority(SVCall_IRQn, 0, 0);
    set_priority(DebugMonitor_IRQn, 0, 0);
    set_priority(PendSV_IRQn, 0, 0);
    set_priority(SysTick_IRQn, 0, 0);
    set_priority(FLASH_IRQn, 0, 0);
    enable_IRQ(FLASH_IRQn);
}

void STM32_NVIC::init_vectors()
{
    #ifdef STM32_USE_UART1
    enable_and_set_prior_IRQ(USART1_IRQn, 0, 0);
    #endif
    #ifdef STM32_USE_UART2
    enable_and_set_prior_IRQ(USART2_IRQn, 0, 0);
    #endif
    #ifdef STM32_USE_UART3
    enable_and_set_prior_IRQ(USART3_IRQn, 0, 0);
    #endif
    #ifdef STM32_USE_UART4
    enable_and_set_prior_IRQ(USART4_IRQn, 0, 0);
    #endif
    #ifdef STM32_USE_UART5
    enable_and_set_prior_IRQ(USART5_IRQn, 0, 0);
    #endif
    #ifdef STM32_USE_UART6
    enable_and_set_prior_IRQ(USART6_IRQn, 0, 0);
    #endif
    #ifdef STM32_USE_UART7
    enable_and_set_prior_IRQ(USART7_IRQn, 0, 0);
    #endif
    #ifdef STM32_USE_UART8
    enable_and_set_prior_IRQ(USART8_IRQn, 0, 0);
    #endif
}

void ISR::NMI()
{
    //TODO
}

void ISR::MemManage()
{
    Error_Handler();
}

void ISR::BusFault()
{
    Error_Handler();
}

void ISR::UsageFault()
{
    Error_Handler();
}

void ISR::HardFault()
{
    Error_Handler();
}

void ISR::FLASH_IRQ()
{
    //PLC_IO::pin_on_FAULT();
}
