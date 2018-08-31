#include "calibration.h"
#include "process_ctrl.h"
#include "relay.h"
#include "math.h"
#define FLASH_USER_START_ADDR        ((uint32_t)0x08071000)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR          ((uint32_t)0x0807FFFF)   /* End   @ of user Flash area */
#define FLASH_USER_PAGE1_START_ADDR  ((uint32_t)0x08071000)   /* Start Page1 @ of user Flash area */
#define FLASH_USER_PAGE1_END_ADDR    ((uint32_t)0x080717FF)   /* End Page1 @ of user Flash area */

#define OUTPUT_OFFSET_ADDR 0x00            	//四端输出电压校准偏移量存储地址
#define OUTPUT_LINEAR_ADDR 0x04*4          	//四端输出电压校准线性量存储地址
#define OUTPUT_RELAY_OFFSET_ADDR 0x04*8    //放大电流校准偏移量存储地址
#define OUTPUT_RELAY_LINEAR_ADDR 0x04*24   	//放大正电流校准线性量存储地址
#define OUTPUT_NRELAY_LINEAR_ADDR 0x04*36  	//放大负电流校准线性量存储地址

FloatUnion Adj_OutputOffset[4];	//gate, source, drain, sub, 零点偏移校正系数
FloatUnion Adj_OutputLinear[4]; //gate, source, drain, sub, 线性度偏移校正系数
FloatUnion Adj_RelayOffset[9];  //{-0.000255,-0.00002717,-0.000624,-0.000425,-0.002283,-0.002923,-0.00835,0,0};//继电器偏移校正系数
FloatUnion Adj_PosRelayLinear[9];  //{-1.00475,-0.99349,-0.9966,-0.999425,-0.99112,-0.99703,-0.99011,-1,-1};//继电器线性度校正系数
FloatUnion Adj_NegRelayLinear[9];
//FloatUnion Adj_MeasOffset[4];
//FloatUnion Adj_MeasLinear[4];
extern uint8_t Uart1RxBuf[512];
float CommitAdjustment(float input_I, RelayTypeDef* pRelay)
{
	if(input_I>0)
	{

				if(pRelay->rangeNow==1)
					input_I=1/Adj_PosRelayLinear[pRelay->rangeNow-1].numFloat*(input_I-Adj_RelayOffset[pRelay->rangeNow-1].numFloat/51);
				else
					input_I=1/Adj_PosRelayLinear[pRelay->rangeNow-1].numFloat*(input_I-Adj_RelayOffset[pRelay->rangeNow-1].numFloat/MyPow(10,(pRelay->rangeNow)));	
					//input_I=1/Adj_PosRelayLinear[pRelay->rangeNow-1].numFloat*(input_I-Adj_RelayOffset[pRelay->rangeNow-1].numFloat);
				//input_I=1/Adj_PosRelayLinear[pRelay->rangeNow-1].numFloat*(input_I);

	}		
	else
	{
				if(pRelay->rangeNow==1)
					input_I=1/Adj_NegRelayLinear[pRelay->rangeNow-1].numFloat*(input_I-Adj_RelayOffset[pRelay->rangeNow-1].numFloat/51);
				else
					input_I=1/Adj_NegRelayLinear[pRelay->rangeNow-1].numFloat*(input_I-Adj_RelayOffset[pRelay->rangeNow-1].numFloat/MyPow(10,(pRelay->rangeNow)));
				//input_I=1/Adj_NegRelayLinear[pRelay->rangeNow-1].numFloat*(input_I);
				//input_I=1/Adj_NegRelayLinear[pRelay->rangeNow-1].numFloat*(input_I-Adj_RelayOffset[pRelay->rangeNow-1].numFloat);//得到原始电流值
	}
	return input_I;
}

float MyPow(float base,uint8_t time)
{
	uint8_t i;
	float rtn=1;
	for(i=0;i<time;i++)
		rtn*=base;
	return rtn;
}	


void ClearArray(FloatUnion* pArray, uint8_t size)
{
	uint8_t i;
	for(i=0;i<size;i++)
	{
		(*(pArray+i)).numFloat=0;
	}
}
void SetArray(FloatUnion* pArray, uint8_t size)
{
	uint8_t i;
	for(i=0;i<size;i++)
	{
		(*(pArray+i)).numFloat=1;
	}
}

void LED_Indicator(void)
{
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
}

/*校准输出电压*/
void Cali_SetRangeRelay(FloatUnion* arrary, uint8_t size, int16_t outputVoltage)
{
	OutputAdjVoltage(DRAIN, outputVoltage);   										//输出特定电压
	HAL_Delay(2000);	                    												//延时等待输出电压到达监控端
	//依次得到GATE，SOURCE，DRAIN，SUB监控端电压mV
	(*(arrary)).numFloat = GetMonitorVoltage(&hMonitorCH_Gate, 1000);
	(*(arrary+1)).numFloat = GetMonitorVoltage(&hMonitorCH_Source, 1000);
	(*(arrary+2)).numFloat = GetMonitorVoltage(&hMonitorCH_Drain, 1000);
	(*(arrary+3)).numFloat = GetMonitorVoltage(&hMonitorCH_Sub, 1000);
	ClearAllVoltage();
}

static void OutputVoltageCalibration(void)
{
	//校准零点
	OutputVoltage(DRAIN, 0);				//使用原始电压输出
	OutputVoltage(GATE, 0);				//使用原始电压输出
	HAL_Delay(200);
	Adj_OutputOffset[2].numFloat=GetMonitorVoltage(&hMonitorCH_Drain, 2000)*10;
	Adj_OutputOffset[0].numFloat=GetMonitorVoltage(&hMonitorCH_Gate, 2000)*10;
	//校准线性度,线性度无法校准
	//OutputAdjVoltage(DRAIN, 2);
	//OutputAdjVoltage(GATE, 2);
	//Adj_OutputLinear[2].numFloat=GetMonitorVoltage(&hMonitorCH_Drain, 2000);
	//Adj_OutputLinear[0].numFloat=GetMonitorVoltage(&hMonitorCH_Gate, 2000);	
}

/*校准开路电流*/
static void ZeroCrrentCalibration(void)
{ 
	OutputAdjVoltage(DRAIN, 0);   				//输出特定电压
	OutputAdjVoltage(DRAIN, 0);   				//输出特定电压
	for(uint8_t i=0;i<9;i++)							//9个挡位
	{
		float temp_value=0, temp_sum=0;
		SetRangeRelay(i+1);
		HAL_Delay(1000+i*1000);
		for(uint8_t j=0;j<10;j++)		//10次采集取平均值
		{
			temp_value=(float)AD7988_1_ReadData(&hAD7988_11)*2.5/65535-1.25;//ADC采集数据
			temp_sum+=temp_value;
			HAL_Delay(2);
		}
		Adj_RelayOffset[i].numFloat=(float)(temp_sum/10);//AD采集平均值
	}
	ClearAllVoltage();
}
static void CurrentRangeCalibration(void)
{
	float temp;
	float temp_value=0, temp_sum=0;

	if((Uart1RxBuf[1]==0x15)&&(Uart1RxBuf[2]==0x02))
	{
    ConnectAllOutput();
	  HAL_Delay(200); //打开Drain的连接
		SetRangeRelay(Uart1RxBuf[3]);
		HAL_Delay(200);//切换档位，注意上位机一定是正确的档位
		OutputAdjVoltage(DRAIN,1000);
		HAL_Delay(200);
		//temp=GetMonitorVoltage(&hMonitorCH_Drain, 2000);
		for(uint8_t j=0;j<10;j++)		//10次采集取平均值
		{
			temp_value=(float)AD7988_1_ReadData(&hAD7988_11)*2.5/65535-1.25;//ADC采集数据
			temp_sum+=temp_value;
			HAL_Delay(2);
		}
		temp=(float)(temp_sum/10);//AD采集平均值
		Cali_Crrent_Pos(Adj_PosRelayLinear,temp,Uart1RxBuf[3]);
		
		OutputAdjVoltage(DRAIN,-1000);   //输出特定电压
		HAL_Delay(200);
		temp_value=0, temp_sum=0;
		for(uint8_t j=0;j<10;j++)		//10次采集取平均值
		{
			temp_value=(float)AD7988_1_ReadData(&hAD7988_11)*2.5/65535-1.25;//ADC采集数据
			temp_sum+=temp_value;
			HAL_Delay(2);
		}
		temp=(float)(temp_sum/10);//AD采集平均值
		Cali_Crrent_Neg(Adj_NegRelayLinear,temp,Uart1RxBuf[3]);
	}
	  ClearAllVoltage();
	  DisconnectAllOutput();
}  
//校准正电流
void Cali_Crrent_Pos(FloatUnion* arrary,float temp,uint8_t index)
{
	(*(arrary+index-1)).numFloat=(temp-Adj_RelayOffset[index-1].numFloat)/1;
}
//校准负电流
void Cali_Crrent_Neg(FloatUnion* arrary,float temp,uint8_t index)
{
	(*(arrary+index-1)).numFloat=(temp*1000-Adj_RelayOffset[index-1].numFloat)/(1);
}
void RunCalibration(uint8_t* UartRxBuf)
{
	switch(UartRxBuf[2])
	{
		case OUTPUT_VOL_CALIBRATION:
			OutputVoltageCalibration();
				
			break;
		case ZERO_CURRENT_CALIBRATION:
			ZeroCrrentCalibration();
			break;
		case CURRENT_RANGE_CALIBRATION:
			CurrentRangeCalibration();
			break;
		case SAVE_CALIBRATION:
			FlashSaveCaliData(FLASH_USER_PAGE1_START_ADDR, FLASH_USER_PAGE1_END_ADDR);	
			break;
	}
}

/*校准测试电流初始化*/
/*float Cali_TestCrrent_Init(FloatUnion* arrary, uint8_t index, int16_t outputVoltage)
{
	float temp,temp_value=0,temp_sum=0;

	(*(arrary+index)).numFloat=0;
	range_select=index+1;
	OutputAdjVoltage(DRAIN, outputVoltage);   //输出特定电压
	HAL_Delay(100);	 //延时等待输出电压到达监控端
	SetRangeRelay(range_select);
	HAL_Delay(100);
	for(uint8_t i=0;i<10;i++)
	{
		temp_value=(float)AD7988_1_ReadData(&hAD7988_11)*2.5/65535-1.25;//ADC采集数据
		temp_sum+=temp_value;
		HAL_Delay(10);
	}
	temp=(float)(temp_sum/10);//AD采集平均值
	ClearAllVoltage();
	//range_select=4;
	//SetRangeRelay(range_select);
	//HAL_Delay(100);
	return temp;
}*/
//100欧电阻校准正电流
/*void Cali_LOAD100_Crrent_Pos(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_1400mV);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/0.714;
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/0.2;
}
//100欧电阻校准负电流
void Cali_LOAD100_Crrent_Neg(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_N1400mV);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/(-0.714);
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_N200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/(-0.2);
}
//10K，1M电阻校准正电流
void Cali_LOAD10K_1M_Crrent_Pos(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_2V);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/0.2;
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/0.2;
}
//10K，1M电阻校准负电流
void Cali_LOAD10K_1M_Crrent_Neg(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_N2V);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/(-0.2);
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_N200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/(-0.2);
}
//100M电阻校准正电流
void Cali_LOAD100M_Crrent_Pos(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_2V);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/0.2;
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/0.2;
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+2, VOL_100mV);
	(*(arrary+index+2)).numFloat=(temp-Adj_RelayOffset[index+2].numFloat);
}
//100M电阻校准负电流
void Cali_LOAD100M_Crrent_Neg(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_N2V);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/(-0.2);
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_N200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/(-0.2);
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+2, VOL_N100mV);
	(*(arrary+index+2)).numFloat=(temp-Adj_RelayOffset[index+2].numFloat)/(-1);
}*/

/*校准过程*/
/*void Do_Calibration(TestPara_TypeDef* pTestPara)
{
	if(pTestPara->caliMode==OUTPUTVOLTAGE_ZERO)               //零点校准输出电压
		Cali_SetRangeRelay(Adj_OutputOffset, 4, VOL_0V);    
	else if(pTestPara->caliMode==OUTPUTVOLTAGE_20V)           //20V校准输出电压无效
		Cali_SetRangeRelay(Adj_OutputLinear, 4, VOL_20V);	
	else if(pTestPara->caliMode==OPEN)                        //开路校准电流偏移量
		Cali_OpenCrrent(Adj_RelayOffset, 9, VOL_0V);
	else if(pTestPara->caliMode==LOAD100||pTestPara->caliMode==LOAD10k||pTestPara->caliMode==LOAD1M||pTestPara->caliMode==LOAD100M) 
	{
		                   
		if(pTestPara->caliMode==LOAD100)                        //100欧电阻校准电流
		{
			Cali_LOAD100_Crrent_Pos(Adj_PosRelayLinear, 0);
			Cali_LOAD100_Crrent_Neg(Adj_NegRelayLinear, 0);
		}		
		else if(pTestPara->caliMode==LOAD10k)                   //10K电阻校准电流
		{
			Cali_LOAD10K_1M_Crrent_Pos(Adj_PosRelayLinear, 2);
			Cali_LOAD10K_1M_Crrent_Neg(Adj_NegRelayLinear, 2);
		}		
		else if(pTestPara->caliMode==LOAD1M)                    //1M电阻校准电流
		{
			Cali_LOAD10K_1M_Crrent_Pos(Adj_PosRelayLinear, 4);
			Cali_LOAD10K_1M_Crrent_Neg(Adj_NegRelayLinear, 4);
		}
		else if(pTestPara->caliMode==LOAD100M)                 //100M电阻校准电流
		{
			Cali_LOAD100M_Crrent_Pos(Adj_PosRelayLinear, 6);
			Cali_LOAD100M_Crrent_Neg(Adj_NegRelayLinear, 6);
		}
			
	}
	else                                                           //短路校准无效
		ClearAllVoltage();
}*/

/*保存校准数据*/		
void FlashSaveCaliData(uint32_t start_address, uint32_t end_address)
{
	uint32_t data=0;	
	HAL_FLASH_Unlock();                                //存入FLASH校准数据
	FlashErase(start_address,end_address);             //擦除用户页,必须整页擦出
	
	for(uint8_t i=0; i<4; i++)                         //存入输出电压校准偏移量值
	{
		for(uint8_t j=0;j<4;j++)
		{
			data<<=8;
			data|=Adj_OutputOffset[i].numUchar[3-j];
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_OFFSET_ADDR+i*4, data);
	}
	
	//由于输出线性度无法正常校准，此处不存
//	for(uint8_t i=0; i<4; i++)                         //存入输出电压校准线性量值
//	{
//		for(uint8_t j=0;j<4;j++)
//		{
//			data<<=8;
//			data|=Adj_OutputLinear[i].numUchar[3-j];
//		}
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_LINEAR_ADDR+i*4, data);
//	}
	
	for(uint8_t i=0; i<9; i++)                         //存入一级放大电流校准偏移量值
	{
		for(uint8_t j=0;j<4;j++)
		{
			data<<=8;
			data|=Adj_RelayOffset[i].numUchar[3-j];
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_RELAY_OFFSET_ADDR+i*4, data);
	}
	
	for(uint8_t i=0; i<9; i++)                         //存入一级放大电流校准线性量值
	{
		for(uint8_t j=0;j<4;j++)
		{
			data<<=8;
			data|=Adj_PosRelayLinear[i].numUchar[3-j];
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_RELAY_LINEAR_ADDR+i*4, data);
	}
	
	for(uint8_t i=0; i<9; i++)                         //存入一级放大电流校准线性量值
	{
		for(uint8_t j=0;j<4;j++)
		{
			data<<=8;
			data|=Adj_NegRelayLinear[i].numUchar[3-j];
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_NRELAY_LINEAR_ADDR+i*4, data);
	}
	
	HAL_FLASH_Lock();
}

