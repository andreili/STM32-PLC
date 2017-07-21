#include "ISRstm32f429xx.h"
#include "stm32f4xx.h"
#include "init.h"
#include "stm32_uart.h"
#include "stm32_rcc.h"
#include "stm32_gpio.h"
#include "stm32_flash.h"
#include "my_func.h"
#include "plc_io.h"
#include "stm32_systick.h"
#include "xprintf.h"

int main();

/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */

// base initialization
void base_init()
{
    STM32_RCC::deinit();
    memset((char*)0x20000000, 0, 0x20000);
    STM32_FLASH::enable_instruction_cache();
    STM32_FLASH::enable_data_cache();
    STM32_FLASH::enable_prefetch_buffer();
void uart3_putc(unsigned char ch)
{
    uart3.send_char(ch);
}

void SystemInit()
{
    base_init();
    // system initialization
    __enable_fault_irq();
    __enable_irq();
    STM32_RCC::init();

	#if defined (DATA_IN_ExtSDRAM)
    SystemInit_ExtMemCtl();
	#endif

	/* Configure the Vector Table location add offset address ------------------*/
	#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
	#else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
	#endif
    
    STM32_GPIO::init_all();
    PLC_IO::init();
    
    
    uart3.init(USART3);
    uart3.set_baud_rate(115200);
    xfunc_out = uart3_putc;
    xprintf("Hello world!\n\r");
}

#define SDRAM_CMD_CLK_ENABLE		(FMC_SDCMR_MODE_0 | FMC_SDCMR_CTB1)
#define SDRAM_CMD_PALL				(FMC_SDCMR_MODE_1 | FMC_SDCMR_CTB1)
#define SDRAM_CMD_AUTOREFRESH_MODE	(FMC_SDCMR_MODE_0 | FMC_SDCMR_MODE_1 | FMC_SDCMR_CTB1)
#define SDRAM_CMD_LOAD_MODE			(FMC_SDCMR_MODE_2 | FMC_SDCMR_CTB1)
#define SDRAM_WAIT_CMD() \
	timeout = 0xFFFF; \
	while((tmpreg != 0) && (timeout-- > 0)) \
	{ \
		tmpreg = FMC_Bank5_6->SDSR & FMC_SDSR_BUSY;  \
	}

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200) 
#define REFRESH_COUNT       					 ((uint32_t)0x0569)   /* SDRAM refresh counter (90MHz SD clock) */

void SystemInit_ExtMemCtl(void)
{
    //#if defined (DATA_IN_ExtSDRAM)
	register uint32_t tmpreg = 0, timeout = 0xFFFF;
	register __IO uint32_t index;

	/* Enable GPIOC, GPIOD, GPIOE, GPIOF and GPIOG interface clock */
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN |
					 RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN |
					 RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN);
    /* Delay after an RCC peripheral clock enabling */
    timeout = 0x1;
    while(timeout-- > 0);

	GPIOC->AFR[0]  = (GPIO_AFRL_AFSEL0_2 | GPIO_AFRL_AFSEL2_3 |
					  GPIO_AFRL_AFSEL3_2);
	GPIOC->MODER   = (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER2_1 |
					  GPIO_MODER_MODER3_1);
	GPIOC->OSPEEDR = (GPIO_OSPEEDR_OSPEED0_1 | GPIO_OSPEEDR_OSPEED2_1 |
					  GPIO_OSPEEDR_OSPEED3_1);
	GPIOC->OTYPER  = 0x00000000;
	GPIOC->PUPDR   = 0x00000000;

	/* Connect PDx pins to FMC Alternate function */
	GPIOD->AFR[0]  = (GPIO_AFRL_AFSEL0_2  | GPIO_AFRL_AFSEL0_3 |
					  GPIO_AFRL_AFSEL1_2  | GPIO_AFRL_AFSEL1_3);
	GPIOD->AFR[1]  = (GPIO_AFRH_AFSEL8_2  | GPIO_AFRH_AFSEL8_3 |
					  GPIO_AFRH_AFSEL9_2  | GPIO_AFRH_AFSEL9_3 |
					  GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_3 |
					  GPIO_AFRH_AFSEL14_2 | GPIO_AFRH_AFSEL14_3 |
					  GPIO_AFRH_AFSEL15_2 | GPIO_AFRH_AFSEL15_3);
	/* Configure PDx pins in Alternate function mode */  
	GPIOD->MODER   = (GPIO_MODER_MODER0_1  | GPIO_MODER_MODER1_1 |
					  GPIO_MODER_MODER8_1  | GPIO_MODER_MODER9_1 |
					  GPIO_MODER_MODER10_1 | GPIO_MODER_MODER14_1 |
					  GPIO_MODER_MODER15_1);
	/* Configure PDx pins speed to 50 MHz */  
	GPIOD->OSPEEDR = (GPIO_OSPEEDR_OSPEED0_1  | GPIO_OSPEEDR_OSPEED1_1 |
					  GPIO_OSPEEDR_OSPEED8_1  | GPIO_OSPEEDR_OSPEED9_1 |
					  GPIO_OSPEEDR_OSPEED10_1 | GPIO_OSPEEDR_OSPEED14_1 |
					  GPIO_OSPEEDR_OSPEED15_1);
	/* Configure PDx pins Output type to push-pull */  
	GPIOD->OTYPER  = 0x00000000;
	/* No pull-up, pull-down for PDx pins */ 
	GPIOD->PUPDR   = 0x00000000;

	/* Connect PEx pins to FMC Alternate function */
	GPIOE->AFR[0]  = (GPIO_AFRL_AFSEL0_2  | GPIO_AFRL_AFSEL0_3 |
					  GPIO_AFRL_AFSEL1_2  | GPIO_AFRL_AFSEL1_3 |
					  GPIO_AFRL_AFSEL7_2  | GPIO_AFRL_AFSEL7_3);
	GPIOE->AFR[1]  = (GPIO_AFRH_AFSEL8_2  | GPIO_AFRH_AFSEL8_3 |
					  GPIO_AFRH_AFSEL9_2  | GPIO_AFRH_AFSEL9_3 |
					  GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_3 |
					  GPIO_AFRH_AFSEL11_2 | GPIO_AFRH_AFSEL11_3 |
					  GPIO_AFRH_AFSEL12_2 | GPIO_AFRH_AFSEL12_3 |
					  GPIO_AFRH_AFSEL13_2 | GPIO_AFRH_AFSEL13_3 |
					  GPIO_AFRH_AFSEL14_2 | GPIO_AFRH_AFSEL14_3 |
					  GPIO_AFRH_AFSEL15_2 | GPIO_AFRH_AFSEL15_3);
	/* Configure PEx pins in Alternate function mode */ 
	GPIOE->MODER   = (GPIO_MODER_MODER0_1  | GPIO_MODER_MODER1_1 |
					  GPIO_MODER_MODER7_1  | GPIO_MODER_MODER8_1 |
					  GPIO_MODER_MODER9_1  | GPIO_MODER_MODER10_1 |
					  GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1 |
					  GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 |
					  GPIO_MODER_MODER15_1);
	/* Configure PEx pins speed to 50 MHz */ 
	GPIOE->OSPEEDR = (GPIO_OSPEEDR_OSPEED0_1  | GPIO_OSPEEDR_OSPEED1_1 |
					  GPIO_OSPEEDR_OSPEED7_1  | GPIO_OSPEEDR_OSPEED8_1 |
					  GPIO_OSPEEDR_OSPEED9_1  | GPIO_OSPEEDR_OSPEED10_1 |
					  GPIO_OSPEEDR_OSPEED11_1 | GPIO_OSPEEDR_OSPEED12_1 |
					  GPIO_OSPEEDR_OSPEED13_1 | GPIO_OSPEEDR_OSPEED14_1 |
					  GPIO_OSPEEDR_OSPEED15_1);
	/* Configure PEx pins Output type to push-pull */  
	GPIOE->OTYPER  = 0x00000000;
	/* No pull-up, pull-down for PEx pins */ 
	GPIOE->PUPDR   = 0x00000000;

	/* Connect PFx pins to FMC Alternate function */
	GPIOF->AFR[0]  = (GPIO_AFRL_AFSEL0_2  | GPIO_AFRL_AFSEL0_3 |
					  GPIO_AFRL_AFSEL1_2  | GPIO_AFRL_AFSEL1_3 |
					  GPIO_AFRL_AFSEL2_2  | GPIO_AFRL_AFSEL2_3 |
					  GPIO_AFRL_AFSEL3_2  | GPIO_AFRL_AFSEL3_3 |
					  GPIO_AFRL_AFSEL4_2  | GPIO_AFRL_AFSEL4_3 |
					  GPIO_AFRL_AFSEL5_2  | GPIO_AFRL_AFSEL5_3 |
					  GPIO_AFRL_AFSEL6_2  | GPIO_AFRL_AFSEL6_3 |
					  GPIO_AFRL_AFSEL7_2  | GPIO_AFRL_AFSEL7_3);
	GPIOF->AFR[1]  = (GPIO_AFRH_AFSEL8_2  | GPIO_AFRH_AFSEL8_3 |
					  GPIO_AFRH_AFSEL9_2  | GPIO_AFRH_AFSEL9_3 |
					  GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_3 |
					  GPIO_AFRH_AFSEL11_2 | GPIO_AFRH_AFSEL11_3 |
					  GPIO_AFRH_AFSEL12_2 | GPIO_AFRH_AFSEL12_3 |
					  GPIO_AFRH_AFSEL13_2 | GPIO_AFRH_AFSEL13_3 |
					  GPIO_AFRH_AFSEL14_2 | GPIO_AFRH_AFSEL14_3 |
					  GPIO_AFRH_AFSEL15_2 | GPIO_AFRH_AFSEL15_3);
	/* Configure PFx pins in Alternate function mode */   
	GPIOF->MODER   = (GPIO_MODER_MODER0_1  | GPIO_MODER_MODER1_1 |
					  GPIO_MODER_MODER2_1  | GPIO_MODER_MODER3_1 |
					  GPIO_MODER_MODER4_1  | GPIO_MODER_MODER5_1 |
					  GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1 |
					  GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 |
					  GPIO_MODER_MODER15_1);
	//0xAA800AAA;
	/* Configure PFx pins speed to 50 MHz */ 
	GPIOF->OSPEEDR = (GPIO_OSPEEDR_OSPEED0_1  | GPIO_OSPEEDR_OSPEED1_1 |
					  GPIO_OSPEEDR_OSPEED2_1  | GPIO_OSPEEDR_OSPEED3_1 |
					  GPIO_OSPEEDR_OSPEED4_1  | GPIO_OSPEEDR_OSPEED5_1 |
					  GPIO_OSPEEDR_OSPEED11_1 | GPIO_OSPEEDR_OSPEED12_1 |
					  GPIO_OSPEEDR_OSPEED13_1 | GPIO_OSPEEDR_OSPEED14_1 |
					  GPIO_OSPEEDR_OSPEED15_1);
	/* Configure PFx pins Output type to push-pull */  
	GPIOF->OTYPER  = 0x00000000;
	/* No pull-up, pull-down for PFx pins */ 
	GPIOF->PUPDR   = 0x00000000;

	/* Connect PGx pins to FMC Alternate function */
	GPIOG->AFR[0]  = (GPIO_AFRL_AFSEL0_2  | GPIO_AFRL_AFSEL0_3 |
					  GPIO_AFRL_AFSEL1_2  | GPIO_AFRL_AFSEL1_3 |
					  GPIO_AFRL_AFSEL2_2  | GPIO_AFRL_AFSEL2_3 |
					  GPIO_AFRL_AFSEL3_2  | GPIO_AFRL_AFSEL3_3 |
					  GPIO_AFRL_AFSEL4_2  | GPIO_AFRL_AFSEL4_3 |
					  GPIO_AFRL_AFSEL5_2  | GPIO_AFRL_AFSEL5_3 |
					  GPIO_AFRL_AFSEL6_2  | GPIO_AFRL_AFSEL6_3 |
					  GPIO_AFRL_AFSEL7_2  | GPIO_AFRL_AFSEL7_3);
	GPIOG->AFR[1]  = (GPIO_AFRH_AFSEL8_2  | GPIO_AFRH_AFSEL8_3 |
					  GPIO_AFRH_AFSEL9_2  | GPIO_AFRH_AFSEL9_3 |
					  GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_3 |
					  GPIO_AFRH_AFSEL11_2 | GPIO_AFRH_AFSEL11_3 |
					  GPIO_AFRH_AFSEL12_2 | GPIO_AFRH_AFSEL12_3 |
					  GPIO_AFRH_AFSEL13_2 | GPIO_AFRH_AFSEL13_3 |
					  GPIO_AFRH_AFSEL14_2 | GPIO_AFRH_AFSEL14_3 |
					  GPIO_AFRH_AFSEL15_2 | GPIO_AFRH_AFSEL15_3);
	/* Configure PGx pins in Alternate function mode */ 
	GPIOG->MODER   = (GPIO_MODER_MODER0_1  | GPIO_MODER_MODER1_1 |
					  GPIO_MODER_MODER2_1  | GPIO_MODER_MODER3_1 |
					  GPIO_MODER_MODER4_1  | GPIO_MODER_MODER5_1 |
					  GPIO_MODER_MODER6_1  | GPIO_MODER_MODER7_1 |
					  GPIO_MODER_MODER8_1  | GPIO_MODER_MODER9_1 |
					  GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1 |
					  GPIO_MODER_MODER12_1 | GPIO_MODER_MODER13_1 |
					  GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1);
	/* Configure PGx pins speed to 50 MHz */ 
	GPIOG->OSPEEDR = (GPIO_OSPEEDR_OSPEED0_1  | GPIO_OSPEEDR_OSPEED1_1 |
					  GPIO_OSPEEDR_OSPEED2_1  | GPIO_OSPEEDR_OSPEED3_1 |
					  GPIO_OSPEEDR_OSPEED4_1  | GPIO_OSPEEDR_OSPEED5_1 |
					  GPIO_OSPEEDR_OSPEED6_1  | GPIO_OSPEEDR_OSPEED7_1 |
					  GPIO_OSPEEDR_OSPEED8_1  | GPIO_OSPEEDR_OSPEED9_1 |
					  GPIO_OSPEEDR_OSPEED10_1 | GPIO_OSPEEDR_OSPEED11_1 |
					  GPIO_OSPEEDR_OSPEED12_1 | GPIO_OSPEEDR_OSPEED13_1 |
					  GPIO_OSPEEDR_OSPEED14_1 | GPIO_OSPEEDR_OSPEED15_1);
	/* Configure PGx pins Output type to push-pull */  
	GPIOG->OTYPER  = 0x00000000;
	/* No pull-up, pull-down for PGx pins */ 
	GPIOG->PUPDR   = 0x00000000;

	/*-- FMC Configuration -------------------------------------------------------*/
	/* Enable the FMC interface clock */
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
    /* Delay after an RCC peripheral clock enabling */
    timeout = 0x1;
    while(timeout-- > 0);

	/* Configure and enable SDRAM bank1 */
	FMC_Bank5_6->SDTR[0] = (FMC_SDTR1_TMRD_0 |						// 0x1
							FMC_SDTR1_TXSR_0 | FMC_SDTR1_TXSR_2 |	// 0x50
							FMC_SDTR1_TRAS_0 | FMC_SDTR1_TRAS_1 |	// 0x300
							FMC_SDTR1_TRC_0 | FMC_SDTR1_TRC_2 |		// 0x5000
							FMC_SDTR1_TWR_0 |
							FMC_SDTR1_TRP_0 |
							FMC_SDTR1_TRCD_0);

	/* SDRAM initialization sequence */
	/* Clock enable command */
	FMC_Bank5_6->SDCMR = SDRAM_CMD_CLK_ENABLE;
	tmpreg = FMC_Bank5_6->SDSR & FMC_SDSR_BUSY; 
	SDRAM_WAIT_CMD();

	/* Delay */
	for (index = 0; index<1000; index++);

	/* PALL command */
	FMC_Bank5_6->SDCMR = SDRAM_CMD_PALL;
	SDRAM_WAIT_CMD();

	/* Auto refresh command */
	FMC_Bank5_6->SDCMR = (SDRAM_CMD_AUTOREFRESH_MODE | FMC_SDCMR_NRFS_0 | FMC_SDCMR_NRFS_1);
	SDRAM_WAIT_CMD();

	/* MRD register program */
	FMC_Bank5_6->SDCMR = (SDRAM_CMD_LOAD_MODE |
						  ((SDRAM_MODEREG_WRITEBURST_MODE_SINGLE |
						    SDRAM_MODEREG_CAS_LATENCY_3) << FMC_SDCMR_MRD_Pos));
	SDRAM_WAIT_CMD();

	/* Set refresh count */
	tmpreg = FMC_Bank5_6->SDRTR;
	FMC_Bank5_6->SDRTR = (tmpreg | (REFRESH_COUNT << 1)); //(0x0000027C<<1));

	/* Disable write protection */
	tmpreg = FMC_Bank5_6->SDCR[0]; 
	FMC_Bank5_6->SDCR[0] = (tmpreg & ~FMC_SDCR1_WP);
	//#endif /* DATA_IN_ExtSDRAM */
}

void ISR::Reset()
{
    SystemInit();
    main();
}
