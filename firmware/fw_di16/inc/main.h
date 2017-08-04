/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define DI0_Pin GPIO_PIN_13
#define DI0_GPIO_Port GPIOC
#define DI1_Pin GPIO_PIN_14
#define DI1_GPIO_Port GPIOC
#define DI2_Pin GPIO_PIN_15
#define DI2_GPIO_Port GPIOC
#define SEL_IN_Pin GPIO_PIN_0
#define SEL_IN_GPIO_Port GPIOD
#define SEL_OUT_Pin GPIO_PIN_1
#define SEL_OUT_GPIO_Port GPIOD
#define DI3_Pin GPIO_PIN_0
#define DI3_GPIO_Port GPIOA
#define DI4_Pin GPIO_PIN_1
#define DI4_GPIO_Port GPIOA
#define DI5_Pin GPIO_PIN_2
#define DI5_GPIO_Port GPIOA
#define DI6_Pin GPIO_PIN_3
#define DI6_GPIO_Port GPIOA
#define DI7_Pin GPIO_PIN_4
#define DI7_GPIO_Port GPIOA
#define DI8_Pin GPIO_PIN_12
#define DI8_GPIO_Port GPIOB
#define DI9_Pin GPIO_PIN_13
#define DI9_GPIO_Port GPIOB
#define DI10_Pin GPIO_PIN_14
#define DI10_GPIO_Port GPIOB
#define DI11_Pin GPIO_PIN_15
#define DI11_GPIO_Port GPIOB
#define DI12_Pin GPIO_PIN_8
#define DI12_GPIO_Port GPIOA
#define DI13_Pin GPIO_PIN_9
#define DI13_GPIO_Port GPIOA
#define DI14_Pin GPIO_PIN_10
#define DI14_GPIO_Port GPIOA
#define DI15_Pin GPIO_PIN_11
#define DI15_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void Error_Handler();

//#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
