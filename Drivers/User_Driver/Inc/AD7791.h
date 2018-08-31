#ifndef __AD7791_H
#define __AD7791_H	 

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "delay.h"
#include "common_type.h"
#include "mxconstants.h"
	
typedef struct 
{
	General_GPIOTypeDef PinCS;
	General_GPIOTypeDef PinSclk;
	General_GPIOTypeDef PinDin;
	General_GPIOTypeDef PinDout;

}AD7791_HandleTypeDef;

//exported variables
extern AD7791_HandleTypeDef hAD7791_Gate;					//handle of output voltage monitors ADC 
extern AD7791_HandleTypeDef hAD7791_Drain;				//handle of output voltage monitors ADC 
extern AD7791_HandleTypeDef hAD7791_Source;				//handle of output voltage monitors ADC 
extern AD7791_HandleTypeDef hAD7791_Sub;					//handle of output voltage monitors ADC 

//private functions
static General_GPIOTypeDef AD7791_Pin_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void M_AD7791_Reset(AD7791_HandleTypeDef *hAD7791x);
uint8_t M_AD7791_WriteReg(AD7791_HandleTypeDef *hAD7791x,uint8_t WR_DATA);
uint8_t M_AD7791_ReadStatusReg(AD7791_HandleTypeDef *hAD7791x);
static void M_AD7791_WriteModeReg(AD7791_HandleTypeDef *hAD7791x,uint8_t ModeReg);
uint32_t M_Get_AD_Data(AD7791_HandleTypeDef *hAD7791x);
//exported functions
void M_AD7791_Init(AD7791_HandleTypeDef *hAD7791x, uint8_t index);						//AD7791初始化
uint32_t M_AD7791_ReadDataReg(AD7791_HandleTypeDef *hAD7791x, int8_t index); //读取AD7791
float M_AD7791_ReadData(AD7791_HandleTypeDef *hAD7791x, int8_t index);				//计算AD7791电压值单位mV    


#endif

