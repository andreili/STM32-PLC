#include "stm32_gpio.h"
#include "stm32_rcc.h"

#define GPIO_MODE             ((uint32_t)0x00000003U)
#define EXTI_MODE             ((uint32_t)0x10000000U)
#define GPIO_MODE_IT          ((uint32_t)0x00010000U)
#define GPIO_MODE_EVT         ((uint32_t)0x00020000U)
#define RISING_EDGE           ((uint32_t)0x00100000U)
#define FALLING_EDGE          ((uint32_t)0x00200000U)
#define GPIO_OUTPUT_TYPE      ((uint32_t)0x00000010U)

#define GPIO_NUMBER           ((uint32_t)16U)

#define GPIO_GET_INDEX(__GPIOx__)    (uint8_t)(((__GPIOx__) == (GPIOA))? 0U :\
                                               ((__GPIOx__) == (GPIOB))? 1U :\
                                               ((__GPIOx__) == (GPIOC))? 2U :\
                                               ((__GPIOx__) == (GPIOD))? 3U :\
                                               ((__GPIOx__) == (GPIOE))? 4U :\
                                               ((__GPIOx__) == (GPIOF))? 5U :\
                                               ((__GPIOx__) == (GPIOG))? 6U :\
                                               ((__GPIOx__) == (GPIOH))? 7U :\
                                               ((__GPIOx__) == (GPIOI))? 8U :\
                                               ((__GPIOx__) == (GPIOJ))? 9U : 10U)

void STM32_GPIO::init_all()
{
	gpioa.init(GPIOA);
	gpiob.init(GPIOB);
	gpioc.init(GPIOC);
	gpiod.init(GPIOD);
	gpioe.init(GPIOE);
	gpiof.init(GPIOF);
	gpiog.init(GPIOG);
	gpioh.init(GPIOH);
}

void STM32_GPIO::set_config(uint32_t pin_mask, uint32_t pin_mode, uint8_t pin_alt, uint32_t pin_speed, uint32_t pin_pull)
{
    uint32_t position;
    uint32_t ioposition = 0x00U;
    uint32_t iocurrent = 0x00U;
    uint32_t temp = 0x00U;
    
    /* Configure the port pins */
    for (position = 0U; position < GPIO_NUMBER; position++)
    {
        /* Get the IO position */
        ioposition = ((uint32_t)0x01U) << position;
        /* Get the current IO position */
        iocurrent = pin_mask & ioposition;

        if (iocurrent == ioposition)
        {
            /*--------------------- GPIO Mode Configuration ------------------------*/
            /* In case of Alternate function mode selection */
            if ((pin_mode == GPIO_MODE_AF_PP) || (pin_mode == GPIO_MODE_AF_OD))
            {
                /* Configure Alternate function mapped with the current IO */
                temp = m_gpio->AFR[position >> 3U];
                temp &= ~((uint32_t)0xFU << ((uint32_t)(position & (uint32_t)0x07U) * 4U)) ;
                temp |= ((uint32_t)(pin_alt) << (((uint32_t)position & (uint32_t)0x07U) * 4U));
                m_gpio->AFR[position >> 3U] = temp;
            }

            /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
            temp = m_gpio->MODER;
            temp &= ~(GPIO_MODER_MODER0 << (position * 2U));
            temp |= ((pin_mode & GPIO_MODE) << (position * 2U));
            m_gpio->MODER = temp;

            /* In case of Output or Alternate function mode selection */
            if ((pin_mode == GPIO_MODE_OUTPUT_PP) || (pin_mode == GPIO_MODE_AF_PP) ||
                (pin_mode == GPIO_MODE_OUTPUT_OD) || (pin_mode == GPIO_MODE_AF_OD))
            {
                /* Configure the IO Speed */
                temp = m_gpio->OSPEEDR; 
                temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2U));
                temp |= (pin_speed << (position * 2U));
                m_gpio->OSPEEDR = temp;

                /* Configure the IO Output Type */
                temp = m_gpio->OTYPER;
                temp &= ~(GPIO_OTYPER_OT_0 << position) ;
                temp |= (((pin_mode & GPIO_OUTPUT_TYPE) >> 4U) << position);
                m_gpio->OTYPER = temp;
            }

            /* Activate the Pull-up or Pull down resistor for the current IO */
            temp = m_gpio->PUPDR;
            temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2U));
            temp |= (pin_pull << (position * 2U));
            m_gpio->PUPDR = temp;

            /*--------------------- EXTI Mode Configuration ------------------------*/
            /* Configure the External Interrupt or event for the current IO */
            if ((pin_mode & EXTI_MODE) == EXTI_MODE)
            {
                /* Enable SYSCFG Clock */
                STM32_RCC::enable_clk_SYSCFG();

                temp = SYSCFG->EXTICR[position >> 2U];
                temp &= ~(((uint32_t)0x0FU) << (4U * (position & 0x03U)));
                temp |= ((uint32_t)(GPIO_GET_INDEX(m_gpio)) << (4U * (position & 0x03U)));
                SYSCFG->EXTICR[position >> 2U] = temp;

                /* Clear EXTI line configuration */
                temp = EXTI->IMR;
                temp &= ~((uint32_t)iocurrent);
                if ((pin_mode & GPIO_MODE_IT) == GPIO_MODE_IT)
                    temp |= iocurrent;
                EXTI->IMR = temp;

                temp = EXTI->EMR;
                temp &= ~((uint32_t)iocurrent);
                if ((pin_mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)
                    temp |= iocurrent;
                EXTI->EMR = temp;

                /* Clear Rising Falling edge configuration */
                temp = EXTI->RTSR;
                temp &= ~((uint32_t)iocurrent);
                if ((pin_mode & RISING_EDGE) == RISING_EDGE)
                    temp |= iocurrent;
                EXTI->RTSR = temp;

                temp = EXTI->FTSR;
                temp &= ~((uint32_t)iocurrent);
                if ((pin_mode & FALLING_EDGE) == FALLING_EDGE)
                    temp |= iocurrent;
                EXTI->FTSR = temp;
            }
        }
    }
}

STM32_GPIO gpioa;
STM32_GPIO gpiob;
STM32_GPIO gpioc;
STM32_GPIO gpiod;
STM32_GPIO gpioe;
STM32_GPIO gpiof;
STM32_GPIO gpiog;
STM32_GPIO gpioh;
