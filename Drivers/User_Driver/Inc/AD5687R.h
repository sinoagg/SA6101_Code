#ifndef __AD5687R_H
#define __AD5687R_H	 

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "delay.h"
#include "common_type.h"

#define CH_A 1
#define CH_B 0
//#define GATE CH_A
//#define SOURCE CH_B
//#define DRAIN CH_A
//#define SUB CH_B	 
	
	
typedef struct 
{
	General_GPIOTypeDef PinReset;
	General_GPIOTypeDef PinSclk;
	General_GPIOTypeDef PinSdata;
	General_GPIOTypeDef PinFsync;

}AD5687R_HandleTypeDef;
	 

	
	 
//private functions
static General_GPIOTypeDef AD5687R_Pin_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
static void AD5687R_WriteCmdCode(AD5687R_HandleTypeDef *hAD5687Rx, uint8_t Cmd);
static void AD5687R_ChangeMode(AD5687R_HandleTypeDef *hAD5687Rx);
static void AD5687R_WriteChCode(AD5687R_HandleTypeDef *hAD5687Rx, uint8_t ch);
//exported functions
void AD5687R_Init(AD5687R_HandleTypeDef *hAD5687Rx, int8_t DAC_num);
void AD5687R_WriteIR(AD5687R_HandleTypeDef *hAD5687Rx, uint8_t channel, uint16_t val);
		    
#endif

