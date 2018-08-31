#include "stm32f1xx_hal.h"	   
#include "AD5687R.h"
#include "mxconstants.h"
#include "delay.h"


static General_GPIOTypeDef AD5687R_Pin_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	General_GPIOTypeDef PinName; 
	PinName.GPIOx=GPIOx;
	PinName.GPIO_Pin=GPIO_Pin;
	return PinName;
}

void AD5687R_Init(AD5687R_HandleTypeDef *hAD5687Rx, int8_t DAC_num)
{

	if(DAC_num==0)
	{
		hAD5687Rx->PinReset = AD5687R_Pin_Init(DAC1_RESET_GPIO_Port, DAC1_RESET_Pin);
		hAD5687Rx->PinFsync = AD5687R_Pin_Init(DAC1_FSYNC_GPIO_Port, DAC1_FSYNC_Pin);
		hAD5687Rx->PinSclk = AD5687R_Pin_Init(DAC1_SCLK_GPIO_Port, DAC1_SCLK_Pin);
		hAD5687Rx->PinSdata = AD5687R_Pin_Init(DAC1_SDATA_GPIO_Port, DAC1_SDATA_Pin);
	}
	else
	{
		hAD5687Rx->PinReset = AD5687R_Pin_Init(DAC2_RESET_GPIO_Port, DAC2_RESET_Pin);
		hAD5687Rx->PinFsync = AD5687R_Pin_Init(DAC2_FSYNC_GPIO_Port, DAC2_FSYNC_Pin);
		hAD5687Rx->PinSclk = AD5687R_Pin_Init(DAC2_SCLK_GPIO_Port, DAC2_SCLK_Pin);
		hAD5687Rx->PinSdata = AD5687R_Pin_Init(DAC2_SDATA_GPIO_Port, DAC2_SDATA_Pin);
	}
	HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(hAD5687Rx->PinFsync.GPIOx, hAD5687Rx->PinFsync.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(hAD5687Rx->PinReset.GPIOx, hAD5687Rx->PinReset.GPIO_Pin, GPIO_PIN_SET);
	delay_us(100);
	AD5687R_ChangeMode(&(*hAD5687Rx));
}

static void AD5687R_WriteCmdCode(AD5687R_HandleTypeDef *hAD5687Rx, uint8_t Cmd)
{
	uint8_t i;
	uint8_t j=0x08;

	for(i=0;i<4;i++)
	{
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		if(j&Cmd)
		HAL_GPIO_WritePin(hAD5687Rx->PinSdata.GPIOx, hAD5687Rx->PinSdata.GPIO_Pin, GPIO_PIN_SET);
		else
		HAL_GPIO_WritePin(hAD5687Rx->PinSdata.GPIOx, hAD5687Rx->PinSdata.GPIO_Pin, GPIO_PIN_RESET);
		j>>=1;
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
	}
}


static void AD5687R_ChangeMode(AD5687R_HandleTypeDef *hAD5687Rx)					//default mode is power down
{
	uint8_t i;
	uint16_t j=0x80;

	delay_us(5);
	HAL_GPIO_WritePin(hAD5687Rx->PinFsync.GPIOx, hAD5687Rx->PinFsync.GPIO_Pin, GPIO_PIN_RESET);
	delay_us(5);
	AD5687R_WriteCmdCode(&(*hAD5687Rx), 0x04);
	for(i=0;i<16;i++)
	{
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
	}
	for(i=0;i<8;i++)
	{
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		if(j&0x3C)
		HAL_GPIO_WritePin(hAD5687Rx->PinSdata.GPIOx, hAD5687Rx->PinSdata.GPIO_Pin, GPIO_PIN_SET);
		else
		HAL_GPIO_WritePin(hAD5687Rx->PinSdata.GPIOx, hAD5687Rx->PinSdata.GPIO_Pin, GPIO_PIN_RESET);
		j>>=1;
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
	}
	delay_us(10);
	HAL_GPIO_WritePin(hAD5687Rx->PinFsync.GPIOx, hAD5687Rx->PinFsync.GPIO_Pin, GPIO_PIN_SET);
}

void AD5687R_WriteIR(AD5687R_HandleTypeDef *hAD5687Rx, uint8_t channel, uint16_t val)
{
	uint8_t i;
	uint16_t j=0x8000;

	delay_us(5);
	HAL_GPIO_WritePin(hAD5687Rx->PinFsync.GPIOx, hAD5687Rx->PinFsync.GPIO_Pin, GPIO_PIN_RESET);
	delay_us(5);
	AD5687R_WriteCmdCode(&(*hAD5687Rx), 0x01);					//write and update channel n
	AD5687R_WriteChCode(&(*hAD5687Rx), channel);										//write channel
	for(i=0;i<16;i++)
	{
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		if(j&val)
		HAL_GPIO_WritePin(hAD5687Rx->PinSdata.GPIOx, hAD5687Rx->PinSdata.GPIO_Pin, GPIO_PIN_SET);
		else
		HAL_GPIO_WritePin(hAD5687Rx->PinSdata.GPIOx, hAD5687Rx->PinSdata.GPIO_Pin, GPIO_PIN_RESET);
		j>>=1;
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
	}
//	for(i=0;i<4;i++)
//	{
//		delay_us(10);
//		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_SET);
//		delay_us(10);
//		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
//	}
	delay_us(5);
	HAL_GPIO_WritePin(hAD5687Rx->PinFsync.GPIOx, hAD5687Rx->PinFsync.GPIO_Pin, GPIO_PIN_SET);
}

void AD5687R_WriteChCode(AD5687R_HandleTypeDef *hAD5687Rx, uint8_t ch)
{
	uint8_t i;
	uint8_t j=0x08;

	if(ch==0)
		ch=1<<3;
	for(i=0;i<4;i++)
	{
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		if(j&ch)
		HAL_GPIO_WritePin(hAD5687Rx->PinSdata.GPIOx, hAD5687Rx->PinSdata.GPIO_Pin, GPIO_PIN_SET);
		else
		HAL_GPIO_WritePin(hAD5687Rx->PinSdata.GPIOx, hAD5687Rx->PinSdata.GPIO_Pin, GPIO_PIN_RESET);
		j>>=1;
		delay_us(5);
		HAL_GPIO_WritePin(hAD5687Rx->PinSclk.GPIOx, hAD5687Rx->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
	}	
}
