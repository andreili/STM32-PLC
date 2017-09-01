#include "stm32_inc.h"

#define GPIO_MODE             ((uint32_t)0x00000003U)
#define EXTI_MODE             ((uint32_t)0x10000000U)
#define GPIO_MODE_IT          ((uint32_t)0x00010000U)
#define GPIO_MODE_EVT         ((uint32_t)0x00020000U)
#define RISING_EDGE           ((uint32_t)0x00100000U)
#define FALLING_EDGE          ((uint32_t)0x00200000U)
#define GPIO_OUTPUT_TYPE      ((uint32_t)0x00000010U)

#define GPIO_NUMBER           ((uint32_t)16U)

#ifdef STM32F429xx
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
#endif
#if defined(STM32F101x6) || defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6)
#define GPIO_GET_INDEX(__GPIOx__) (((__GPIOx__) == (GPIOA))? 0U :\
                                   ((__GPIOx__) == (GPIOB))? 1U :\
                                   ((__GPIOx__) == (GPIOC))? 2U :3U)
#elif defined(STM32F100xB) || defined(STM32F101xB) || defined(STM32F103xB) || defined(STM32F105xC) || defined(STM32F107xC)
#define GPIO_GET_INDEX(__GPIOx__) (((__GPIOx__) == (GPIOA))? 0U :\
                                   ((__GPIOx__) == (GPIOB))? 1U :\
                                   ((__GPIOx__) == (GPIOC))? 2U :\
                                   ((__GPIOx__) == (GPIOD))? 3U :4U)
#elif defined(STM32F100xE) || defined(STM32F101xE) || defined(STM32F101xG) || defined(STM32F103xE) || defined(STM32F103xG)
#define GPIO_GET_INDEX(__GPIOx__) (((__GPIOx__) == (GPIOA))? 0U :\
                                   ((__GPIOx__) == (GPIOB))? 1U :\
                                   ((__GPIOx__) == (GPIOC))? 2U :\
                                   ((__GPIOx__) == (GPIOD))? 3U :\
                                   ((__GPIOx__) == (GPIOE))? 4U :\
                                   ((__GPIOx__) == (GPIOF))? 5U :6U)
#endif

#define  GPIO_CR_MODE_INPUT         ((uint32_t)0x00000000) /*!< 00: Input mode (reset state)  */
#define  GPIO_CR_CNF_ANALOG         ((uint32_t)0x00000000) /*!< 00: Analog mode  */
#define  GPIO_CR_CNF_INPUT_FLOATING ((uint32_t)0x00000004) /*!< 01: Floating input (reset state)  */
#define  GPIO_CR_CNF_INPUT_PU_PD    ((uint32_t)0x00000008) /*!< 10: Input with pull-up / pull-down  */
#define  GPIO_CR_CNF_GP_OUTPUT_PP   ((uint32_t)0x00000000) /*!< 00: General purpose output push-pull  */
#define  GPIO_CR_CNF_GP_OUTPUT_OD   ((uint32_t)0x00000004) /*!< 01: General purpose output Open-drain  */
#define  GPIO_CR_CNF_AF_OUTPUT_PP   ((uint32_t)0x00000008) /*!< 10: Alternate function output Push-pull  */
#define  GPIO_CR_CNF_AF_OUTPUT_OD   ((uint32_t)0x0000000C) /*!< 11: Alternate function output Open-drain  */

void STM32_GPIO::init_all()
{
	gpioa.init(GPIOA);
	gpiob.init(GPIOB);
	gpioc.init(GPIOC);
	gpiod.init(GPIOD);
	gpioe.init(GPIOE);
    #ifdef STM32F429xx
    gpiof.init(GPIOF);
    gpiog.init(GPIOG);
	gpioh.init(GPIOH);
    #endif
}

void STM32_GPIO::set_config(uint32_t pin_mask, uint32_t pin_mode, uint8_t pin_alt, uint32_t pin_speed, uint32_t pin_pull)
{
    #ifdef STM32F10X_MD
    (void)(pin_alt);

    uint32_t position;
    uint32_t ioposition = 0x00U;
    uint32_t iocurrent = 0x00U;
    uint32_t temp = 0x00U;
    uint32_t config = 0x00;

    __IO uint32_t *configregister;
    uint32_t registeroffset = 0;

    /* Configure the port pins */
    for (position = 0; position < GPIO_NUMBER; position++)
    {
        /* Get the IO position */
        ioposition = ((uint32_t)0x01) << position;

        /* Get the current IO position */
        iocurrent = (uint32_t)(pin_mask) & ioposition;

        if (iocurrent == ioposition)
        {
            /* Based on the required mode, filling config variable with MODEy[1:0] and CNFy[3:2] corresponding bits */
            switch (pin_mode)
            {
            /* If we are configuring the pin in OUTPUT push-pull mode */
            case GPIO_MODE_OUTPUT_PP:
                config = pin_speed + GPIO_CR_CNF_GP_OUTPUT_PP;
                break;

            /* If we are configuring the pin in OUTPUT open-drain mode */
            case GPIO_MODE_OUTPUT_OD:
                config = pin_speed + GPIO_CR_CNF_GP_OUTPUT_OD;
                break;

            /* If we are configuring the pin in ALTERNATE FUNCTION push-pull mode */
            case GPIO_MODE_AF_PP:
                config = pin_speed + GPIO_CR_CNF_AF_OUTPUT_PP;
                break;

            /* If we are configuring the pin in ALTERNATE FUNCTION open-drain mode */
            case GPIO_MODE_AF_OD:
                config = pin_speed + GPIO_CR_CNF_AF_OUTPUT_OD;
                break;

            /* If we are configuring the pin in INPUT (also applicable to EVENT and IT mode) */
            case GPIO_MODE_INPUT:
            case GPIO_MODE_IT_RISING:
            case GPIO_MODE_IT_FALLING:
            case GPIO_MODE_IT_RISING_FALLING:
            case GPIO_MODE_EVT_RISING:
            case GPIO_MODE_EVT_FALLING:
            case GPIO_MODE_EVT_RISING_FALLING:
                if(pin_pull == GPIO_NOPULL)
                {
                    config = GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_FLOATING;
                }
                else if(pin_pull == GPIO_PULLUP)
                {
                    config = GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_PU_PD;

                    /* Set the corresponding ODR bit */
                    m_gpio->BSRR = ioposition;
                }
                else /* GPIO_PULLDOWN */
                {
                    config = GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_PU_PD;

                    /* Reset the corresponding ODR bit */
                    m_gpio->BRR = ioposition;
                }
                break;

            /* If we are configuring the pin in INPUT analog mode */
            case GPIO_MODE_ANALOG:
                config = GPIO_CR_MODE_INPUT + GPIO_CR_CNF_ANALOG;
                break;

            /* Parameters are checked with assert_param */
            default:
                break;
            }

            /* Check if the current bit belongs to first half or last half of the pin count number
            in order to address CRH or CRL register*/
            configregister = (iocurrent < GPIO_PIN_8) ? &m_gpio->CRL     : &m_gpio->CRH;
            registeroffset = (iocurrent < GPIO_PIN_8) ? (position << 2) : ((position - 8) << 2);

            /* Apply the new configuration of the pin to the register */
            MODIFY_REG((*configregister), ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset ), (config << registeroffset));

            /*--------------------- EXTI Mode Configuration ------------------------*/
            /* Configure the External Interrupt or event for the current IO */
            if((pin_mode & EXTI_MODE) == EXTI_MODE)
            {
                /* Enable AFIO Clock */
                SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
                temp = AFIO->EXTICR[position >> 2];
                CLEAR_BIT(temp, ((uint32_t)0x0F) << (4 * (position & 0x03)));
                SET_BIT(temp, (GPIO_GET_INDEX(m_gpio)) << (4 * (position & 0x03)));
                AFIO->EXTICR[position >> 2] = temp;


                /* Configure the interrupt mask */
                if((pin_mode & GPIO_MODE_IT) == GPIO_MODE_IT)
                {
                    SET_BIT(EXTI->IMR, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->IMR, iocurrent);
                }

                /* Configure the event mask */
                if((pin_mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)
                {
                    SET_BIT(EXTI->EMR, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->EMR, iocurrent);
                }

                /* Enable or disable the rising trigger */
                if((pin_mode & RISING_EDGE) == RISING_EDGE)
                {
                    SET_BIT(EXTI->RTSR, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->RTSR, iocurrent);
                }

                /* Enable or disable the falling trigger */
                if((pin_mode & FALLING_EDGE) == FALLING_EDGE)
                {
                    SET_BIT(EXTI->FTSR, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->FTSR, iocurrent);
                }
            }
        }
    }
    #endif // STM32F10X_MD

    #ifdef STM32F429xx
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
    #endif // STM32F429xx
}

STM32_GPIO gpioa;
STM32_GPIO gpiob;
STM32_GPIO gpioc;
STM32_GPIO gpiod;
STM32_GPIO gpioe;
STM32_GPIO gpiof;
STM32_GPIO gpiog;
STM32_GPIO gpioh;
