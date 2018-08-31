#include "AD7791.h"

AD7791_HandleTypeDef hAD7791_Gate;				//handle of output voltage monitors ADC 
AD7791_HandleTypeDef hAD7791_Source;				//handle of output voltage monitors ADC 
AD7791_HandleTypeDef hAD7791_Drain;				//handle of output voltage monitors ADC 
AD7791_HandleTypeDef hAD7791_Sub;				//handle of output voltage monitors ADC 

/*AD7791引脚初始化函数*/
static General_GPIOTypeDef AD7791_Pin_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	General_GPIOTypeDef PinName; 
	PinName.GPIOx=GPIOx;
	PinName.GPIO_Pin=GPIO_Pin;
	return PinName;
}

/*AD7791初始化函数*/
void M_AD7791_Init(AD7791_HandleTypeDef *hAD7791x, uint8_t index)
{
	if(index==0)
	{
			hAD7791x->PinCS = AD7791_Pin_Init(M_ADC_CS1_GPIO_Port, M_ADC_CS1_Pin);
			HAL_GPIO_WritePin(GPIOC, M_ADC_CS3_Pin|M_ADC_CS4_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, M_ADC_CS2_Pin, GPIO_PIN_SET);
			hAD7791x->PinDin = AD7791_Pin_Init(M_ADC_DIN_GPIO_Port, M_ADC_DIN_Pin);
			hAD7791x->PinSclk = AD7791_Pin_Init(M_ADC_SCLK_GPIO_Port, M_ADC_SCLK_Pin);
			hAD7791x->PinDout = AD7791_Pin_Init(M_ADC_DOUT_GPIO_Port, M_ADC_DOUT_Pin);
			
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_RESET);
			
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_RESET);
			M_AD7791_Reset(&(*hAD7791x));
			HAL_Delay(1);
			M_AD7791_WriteModeReg(&(*hAD7791x),0x02);
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);
	}
	else if(index==1)
	{
			hAD7791x->PinCS = AD7791_Pin_Init(M_ADC_CS2_GPIO_Port, M_ADC_CS2_Pin);
			HAL_GPIO_WritePin(GPIOC, M_ADC_CS1_Pin|M_ADC_CS3_Pin|M_ADC_CS4_Pin, GPIO_PIN_SET);
			hAD7791x->PinDin = AD7791_Pin_Init(M_ADC_DIN_GPIO_Port, M_ADC_DIN_Pin);
			hAD7791x->PinSclk = AD7791_Pin_Init(M_ADC_SCLK_GPIO_Port, M_ADC_SCLK_Pin);
			hAD7791x->PinDout = AD7791_Pin_Init(M_ADC_DOUT_GPIO_Port, M_ADC_DOUT_Pin);
			
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_RESET);
			
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_RESET);
			M_AD7791_Reset(&(*hAD7791x));
			HAL_Delay(1);
			M_AD7791_WriteModeReg(&(*hAD7791x),0x02);
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);
	}		
	else if(index==2)
	{
			hAD7791x->PinCS = AD7791_Pin_Init(M_ADC_CS3_GPIO_Port, M_ADC_CS3_Pin);
			HAL_GPIO_WritePin(GPIOC, M_ADC_CS1_Pin|M_ADC_CS4_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, M_ADC_CS2_Pin, GPIO_PIN_SET);
			hAD7791x->PinDin = AD7791_Pin_Init(M_ADC_DIN_GPIO_Port, M_ADC_DIN_Pin);
			hAD7791x->PinSclk = AD7791_Pin_Init(M_ADC_SCLK_GPIO_Port, M_ADC_SCLK_Pin);
			hAD7791x->PinDout = AD7791_Pin_Init(M_ADC_DOUT_GPIO_Port, M_ADC_DOUT_Pin);
			
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_RESET);
			
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_RESET);
			M_AD7791_Reset(&(*hAD7791x));
			HAL_Delay(1);
			M_AD7791_WriteModeReg(&(*hAD7791x),0x02);
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);
	}		
	else
	{
			hAD7791x->PinCS = AD7791_Pin_Init(M_ADC_CS4_GPIO_Port, M_ADC_CS4_Pin);
			HAL_GPIO_WritePin(GPIOC, M_ADC_CS1_Pin|M_ADC_CS3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, M_ADC_CS2_Pin, GPIO_PIN_SET);
			hAD7791x->PinDin = AD7791_Pin_Init(M_ADC_DIN_GPIO_Port, M_ADC_DIN_Pin);
			hAD7791x->PinSclk = AD7791_Pin_Init(M_ADC_SCLK_GPIO_Port, M_ADC_SCLK_Pin);
			hAD7791x->PinDout = AD7791_Pin_Init(M_ADC_DOUT_GPIO_Port, M_ADC_DOUT_Pin);
			
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_RESET);
			
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_RESET);
			M_AD7791_Reset(&(*hAD7791x));
			HAL_Delay(1);
			M_AD7791_WriteModeReg(&(*hAD7791x),0x02);
			HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);
	}

		
}

/*AD7791复位函数*/
void M_AD7791_Reset(AD7791_HandleTypeDef *hAD7791x)
{
	uint8_t i;
	HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_SET);
	delay_us(10);
	for(i=0;i<32;i++)
	{
		HAL_GPIO_WritePin(hAD7791x->PinSclk.GPIOx, hAD7791x->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
		delay_us(10);
		HAL_GPIO_WritePin(hAD7791x->PinSclk.GPIOx, hAD7791x->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		delay_us(10);
	}
	HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_RESET);
}
/*AD7791写寄存器函数*/
uint8_t M_AD7791_WriteReg(AD7791_HandleTypeDef *hAD7791x,uint8_t WR_DATA)	
{
	uint8_t temp;
	uint8_t i;
	temp=0x80;
	for(i=0;i<8;i++)
	{
		if((temp&WR_DATA)==0)
		{
			HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_SET);
		}
		HAL_GPIO_WritePin(hAD7791x->PinSclk.GPIOx, hAD7791x->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
		delay_us(10);
		HAL_GPIO_WritePin(hAD7791x->PinSclk.GPIOx, hAD7791x->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		delay_us(10);
		temp=temp>>1;
	}
	HAL_GPIO_WritePin(hAD7791x->PinDin.GPIOx, hAD7791x->PinDin.GPIO_Pin, GPIO_PIN_RESET);
	return 0;
}
/*读状态寄存器*/
uint8_t M_AD7791_ReadStatusReg(AD7791_HandleTypeDef *hAD7791x)				
{
	uint8_t j;
	uint8_t temp=0;
	M_AD7791_WriteReg(&(*hAD7791x),0x08);						//给指定芯片读命令
	//SPI_AD7791_WriteReg(0x08);						//读状态寄存器命令						
	for(j=0;j<8;j++)
	{
		HAL_GPIO_WritePin(hAD7791x->PinSclk.GPIOx, hAD7791x->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
		delay_us(10);
		if(HAL_GPIO_ReadPin(hAD7791x->PinDout.GPIOx, hAD7791x->PinDout.GPIO_Pin)==0)
		{
			temp=temp<<1;
		}
		else
		{
			temp=temp<<1;
			temp=temp+0x01;
		}
		delay_us(10);
		HAL_GPIO_WritePin(hAD7791x->PinSclk.GPIOx, hAD7791x->PinSclk.GPIO_Pin, GPIO_PIN_SET);
		delay_us(10);
	}
	return temp;
}

static void M_AD7791_WriteModeReg(AD7791_HandleTypeDef *hAD7791x,uint8_t ModeReg)
{
	M_AD7791_WriteReg(&(*hAD7791x),0x10);
	M_AD7791_WriteReg(&(*hAD7791x),ModeReg);		//0x3c
}
/*读取监控端电压原始值，index用来选择监控端M_GATE\M_SOURCE\M_DRAIN\M_SUB   */
uint32_t M_AD7791_ReadDataReg(AD7791_HandleTypeDef *hAD7791x,int8_t index)
{
	if(index==0)
	{ 
		HAL_GPIO_WritePin(GPIOA, M_ADC_CS2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, M_ADC_CS3_Pin|M_ADC_CS4_Pin, GPIO_PIN_SET);
		hAD7791x->PinCS = AD7791_Pin_Init(M_ADC_CS1_GPIO_Port, M_ADC_CS1_Pin);
	}
	else if(index==1)
	{
		HAL_GPIO_WritePin(GPIOC, M_ADC_CS1_Pin|M_ADC_CS3_Pin|M_ADC_CS4_Pin, GPIO_PIN_SET);
		hAD7791x->PinCS = AD7791_Pin_Init(M_ADC_CS2_GPIO_Port, M_ADC_CS2_Pin);
	}		
	else if(index==2)
	{
		HAL_GPIO_WritePin(GPIOA, M_ADC_CS2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, M_ADC_CS1_Pin|M_ADC_CS4_Pin, GPIO_PIN_SET);
		hAD7791x->PinCS = AD7791_Pin_Init(M_ADC_CS3_GPIO_Port, M_ADC_CS3_Pin);
	}		
	else
	{
		HAL_GPIO_WritePin(GPIOA, M_ADC_CS2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, M_ADC_CS1_Pin|M_ADC_CS3_Pin, GPIO_PIN_SET);
		hAD7791x->PinCS = AD7791_Pin_Init(M_ADC_CS4_GPIO_Port, M_ADC_CS4_Pin);
	}
	uint32_t AD7791_ReadVal=0;

	HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_RESET);	
	while(HAL_GPIO_ReadPin(hAD7791x->PinDout.GPIOx, hAD7791x->PinDout.GPIO_Pin)==0);
	
	M_AD7791_WriteReg(&(*hAD7791x),0x38);				    //读数据寄存器
	
	while(HAL_GPIO_ReadPin(hAD7791x->PinDout.GPIOx, hAD7791x->PinDout.GPIO_Pin)!=0);
	AD7791_ReadVal=M_Get_AD_Data(&(*hAD7791x));			//连续读三组8 bit数据	
							
	HAL_GPIO_WritePin(hAD7791x->PinCS.GPIOx, hAD7791x->PinCS.GPIO_Pin, GPIO_PIN_SET);	
	return AD7791_ReadVal;
}

uint32_t M_Get_AD_Data(AD7791_HandleTypeDef *hAD7791x)
{
	uint32_t tempresult;
	uint8_t i,j;
	uint8_t temp=0;
	uint32_t AD_Data=0;
	for(i=3;i>0;i--)
	{
		for(j=0;j<8;j++)
		{
			HAL_GPIO_WritePin(hAD7791x->PinSclk.GPIOx, hAD7791x->PinSclk.GPIO_Pin, GPIO_PIN_RESET);
			delay_us(20);
			if(HAL_GPIO_ReadPin(hAD7791x->PinDout.GPIOx, hAD7791x->PinDout.GPIO_Pin)==0)
			{
				temp=temp<<1;
			}
			else
			{
				temp=temp<<1;
				temp=temp+0x01;
			}
			delay_us(20);
			HAL_GPIO_WritePin(hAD7791x->PinSclk.GPIOx, hAD7791x->PinSclk.GPIO_Pin, GPIO_PIN_SET);
			delay_us(20);
		}
		tempresult=temp;
		tempresult=tempresult<<((i-1)*8);
		AD_Data+=tempresult;
	}
	return AD_Data;
}
/*计算得到监控端电压值mV ,index用来选择监控端M_GATE\M_SOURCE\M_DRAIN\M_SUB*/
float M_AD7791_ReadData(AD7791_HandleTypeDef *hAD7791x,int8_t index)
{
	uint32_t readdata=0;
	float result=0;
	readdata=M_AD7791_ReadDataReg(&(*hAD7791x),index);
	if(readdata<0x800000)
	{
		result=-(float)(0x800000-readdata);
	}	
	else
	{
		result=(float)(readdata-0x800000);
	}
	
	result=result/0x800000*2500;
	return result;
}

