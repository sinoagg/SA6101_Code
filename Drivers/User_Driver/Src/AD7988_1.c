#include "stm32f1xx_hal.h"	   
#include "AD7988_1.h"
#include "mxconstants.h"
#include "delay.h"


static General_GPIOTypeDef AD7988_1_Pin_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	General_GPIOTypeDef PinName; 
	PinName.GPIOx=GPIOx;
	PinName.GPIO_Pin=GPIO_Pin;
	return PinName;
}

void AD7988_1_Init(AD7988_1_HandleTypeDef *hAD7988_1x)
{

	hAD7988_1x->PinSdi = AD7988_1_Pin_Init(ADC_SDI_GPIO_Port, ADC_SDI_Pin);
	hAD7988_1x->PinSdo = AD7988_1_Pin_Init(ADC_SDO_GPIO_Port, ADC_SDO_Pin);
	hAD7988_1x->PinSclk = AD7988_1_Pin_Init(ADC_SCLK_GPIO_Port, ADC_SCLK_Pin);
	hAD7988_1x->PinCnv = AD7988_1_Pin_Init(ADC_CNV_GPIO_Port, ADC_CNV_Pin);
	delay_us(100);
	HAL_GPIO_WritePin(ADC_CNV_GPIO_Port, ADC_CNV_Pin, GPIO_PIN_RESET);		//Í£Ö¹×ª»»
	HAL_GPIO_WritePin(ADC_SDI_GPIO_Port, ADC_SDI_Pin, GPIO_PIN_SET);			//Ð¾Æ¬Ê¹ÄÜ
}

uint16_t AD7988_1_ReadData(AD7988_1_HandleTypeDef *hAD7988_1x)
{
	uint16_t data=0;
	uint8_t i;

	HAL_GPIO_WritePin(hAD7988_1x->PinCnv.GPIOx, hAD7988_1x->PinCnv.GPIO_Pin, GPIO_PIN_SET);		//start conversion
	delay_us(2);																																							//wait for conversion complete
	HAL_GPIO_WritePin(hAD7988_1x->PinCnv.GPIOx, hAD7988_1x->PinCnv.GPIO_Pin, GPIO_PIN_RESET);		//enable output
	delay_us(1);																																							//wait for data ready
	for(i=0;i<16;i++)
	{
		HAL_GPIO_WritePin(hAD7988_1x->PinSclk.GPIOx, hAD7988_1x->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		data|=HAL_GPIO_ReadPin(hAD7988_1x->PinSdo.GPIOx, hAD7988_1x->PinSdo.GPIO_Pin);
		HAL_GPIO_WritePin(hAD7988_1x->PinSclk.GPIOx, hAD7988_1x->PinSclk.GPIO_Pin, GPIO_PIN_RESET);	//start to read data
		if(i<15) data<<=1;
	}
	return data;
}
