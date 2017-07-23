#ifndef STM32_NVIC_H
#define STM32_NVIC_H

#include "stm32_conf.h"

#define NVIC_PRIORITYGROUP_0 ((uint32_t)0x07)/*!< 0 bits for pre-emption priority
                                                  4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1 ((uint32_t)0x06)/*!< 1 bits for pre-emption priority
                                                  3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2 ((uint32_t)0x05)/*!< 2 bits for pre-emption priority
                                                  2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3 ((uint32_t)0x04)/*!< 3 bits for pre-emption priority
                                                  1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4 ((uint32_t)0x03)/*!< 4 bits for pre-emption priority
                                                  0 bits for subpriority */

class STM32_NVIC
{
public:
    static inline void set_priority_grouping(uint32_t val) { NVIC_SetPriorityGrouping(val); }
    static inline uint32_t get_priority_grouping() { return NVIC_GetPriorityGrouping(); }

    static inline uint32_t encode_priority(uint32_t group, uint32_t pre_prior, uint32_t sub_prior)
            { return NVIC_EncodePriority(group, pre_prior, sub_prior); }

    static inline void set_priority(IRQn_Type irqn, uint32_t val) { NVIC_SetPriority(irqn, val); }
};

#endif // STM32_NVIC_H
