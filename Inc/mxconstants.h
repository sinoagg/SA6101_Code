/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
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
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define DAC2_RESET_Pin GPIO_PIN_0
#define DAC2_RESET_GPIO_Port GPIOC
#define DAC2_SDATA_Pin GPIO_PIN_1
#define DAC2_SDATA_GPIO_Port GPIOC
#define DAC2_FSYNC_Pin GPIO_PIN_2
#define DAC2_FSYNC_GPIO_Port GPIOC
#define DAC2_SCLK_Pin GPIO_PIN_3
#define DAC2_SCLK_GPIO_Port GPIOC
#define DAC1_RESET_Pin GPIO_PIN_0
#define DAC1_RESET_GPIO_Port GPIOA
#define DAC1_SDATA_Pin GPIO_PIN_1
#define DAC1_SDATA_GPIO_Port GPIOA
#define DAC1_FSYNC_Pin GPIO_PIN_8
#define DAC1_FSYNC_GPIO_Port GPIOA
#define DAC1_SCLK_Pin GPIO_PIN_9
#define DAC1_SCLK_GPIO_Port GPIOC

#define CTRL1_Pin GPIO_PIN_4
#define CTRL1_GPIO_Port GPIOA
#define CTRL2_Pin GPIO_PIN_5
#define CTRL2_GPIO_Port GPIOA
#define CTRL3_Pin GPIO_PIN_6
#define CTRL3_GPIO_Port GPIOA
#define CTRL4_Pin GPIO_PIN_7
#define CTRL4_GPIO_Port GPIOA
#define ADC_SDI_Pin GPIO_PIN_14
#define ADC_SDI_GPIO_Port GPIOB
#define ADC_SCLK_Pin GPIO_PIN_15
#define ADC_SCLK_GPIO_Port GPIOB
#define ADC_SDO_Pin GPIO_PIN_6
#define ADC_SDO_GPIO_Port GPIOC
#define ADC_CNV_Pin GPIO_PIN_7
#define ADC_CNV_GPIO_Port GPIOC
//#define PGA_WR_Pin GPIO_PIN_8
//#define PGA_WR_GPIO_Port GPIOC
//#define PGA_A0_Pin GPIO_PIN_9
//#define PGA_A0_GPIO_Port GPIOC
//#define PGA_A1_Pin GPIO_PIN_8
//#define PGA_A1_GPIO_Port GPIOA

#define RS485_RE_Pin GPIO_PIN_6
#define RS485_RE_GPIO_Port GPIOB
#define RS485_RE2_Pin GPIO_PIN_8
#define RS485_RE2_GPIO_Port GPIOC

#define CTRL9_Pin GPIO_PIN_2
#define CTRL9_GPIO_Port GPIOB
#define CTRL10_Pin GPIO_PIN_3
#define CTRL10_GPIO_Port GPIOB
#define CTRL11_Pin GPIO_PIN_4
#define CTRL11_GPIO_Port GPIOB
#define CTRL12_Pin GPIO_PIN_12
#define CTRL12_GPIO_Port GPIOB
#define CTRL13_Pin GPIO_PIN_13
#define CTRL13_GPIO_Port GPIOB
#define CTRL14_Pin GPIO_PIN_7
#define CTRL14_GPIO_Port GPIOB
#define CTRL15_Pin GPIO_PIN_15
#define CTRL15_GPIO_Port GPIOC
#define CTRL16_Pin GPIO_PIN_13
#define CTRL16_GPIO_Port GPIOC
#define CTRL17_Pin GPIO_PIN_14
#define CTRL17_GPIO_Port GPIOC
#define CTRL18_Pin GPIO_PIN_5
#define CTRL18_GPIO_Port GPIOB
#define CTRL19_Pin GPIO_PIN_2
#define CTRL19_GPIO_Port GPIOD
#define CTRL20_Pin GPIO_PIN_1
#define CTRL20_GPIO_Port GPIOB

#define M_ADC_CS1_Pin GPIO_PIN_12
#define M_ADC_CS1_GPIO_Port GPIOC
#define M_ADC_CS2_Pin GPIO_PIN_15
#define M_ADC_CS2_GPIO_Port GPIOA
#define M_ADC_CS3_Pin GPIO_PIN_11
#define M_ADC_CS3_GPIO_Port GPIOC
#define M_ADC_CS4_Pin GPIO_PIN_10
#define M_ADC_CS4_GPIO_Port GPIOC
#define M_ADC_SCLK_Pin GPIO_PIN_4
#define M_ADC_SCLK_GPIO_Port GPIOC
#define M_ADC_DIN_Pin GPIO_PIN_0
#define M_ADC_DIN_GPIO_Port GPIOB
#define M_ADC_DOUT_Pin GPIO_PIN_5
#define M_ADC_DOUT_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_8
#define LED0_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_9
#define LED1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
