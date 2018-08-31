#include "calibration.h"
#include "process_ctrl.h"
#include "relay.h"
#include "math.h"
#define FLASH_USER_START_ADDR        ((uint32_t)0x08071000)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR          ((uint32_t)0x0807FFFF)   /* End   @ of user Flash area */
#define FLASH_USER_PAGE1_START_ADDR  ((uint32_t)0x08071000)   /* Start Page1 @ of user Flash area */
#define FLASH_USER_PAGE1_END_ADDR    ((uint32_t)0x080717FF)   /* End Page1 @ of user Flash area */

#define OUTPUT_OFFSET_ADDR 0x00            	//�Ķ������ѹУ׼ƫ�����洢��ַ
#define OUTPUT_LINEAR_ADDR 0x04*4          	//�Ķ������ѹУ׼�������洢��ַ
#define OUTPUT_RELAY_OFFSET_ADDR 0x04*8    //�Ŵ����У׼ƫ�����洢��ַ
#define OUTPUT_RELAY_LINEAR_ADDR 0x04*24   	//�Ŵ�������У׼�������洢��ַ
#define OUTPUT_NRELAY_LINEAR_ADDR 0x04*36  	//�Ŵ󸺵���У׼�������洢��ַ

FloatUnion Adj_OutputOffset[4];	//gate, source, drain, sub, ���ƫ��У��ϵ��
FloatUnion Adj_OutputLinear[4]; //gate, source, drain, sub, ���Զ�ƫ��У��ϵ��
FloatUnion Adj_RelayOffset[9];  //{-0.000255,-0.00002717,-0.000624,-0.000425,-0.002283,-0.002923,-0.00835,0,0};//�̵���ƫ��У��ϵ��
FloatUnion Adj_PosRelayLinear[9];  //{-1.00475,-0.99349,-0.9966,-0.999425,-0.99112,-0.99703,-0.99011,-1,-1};//�̵������Զ�У��ϵ��
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
				//input_I=1/Adj_NegRelayLinear[pRelay->rangeNow-1].numFloat*(input_I-Adj_RelayOffset[pRelay->rangeNow-1].numFloat);//�õ�ԭʼ����ֵ
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

/*У׼�����ѹ*/
void Cali_SetRangeRelay(FloatUnion* arrary, uint8_t size, int16_t outputVoltage)
{
	OutputAdjVoltage(DRAIN, outputVoltage);   										//����ض���ѹ
	HAL_Delay(2000);	                    												//��ʱ�ȴ������ѹ�����ض�
	//���εõ�GATE��SOURCE��DRAIN��SUB��ض˵�ѹmV
	(*(arrary)).numFloat = GetMonitorVoltage(&hMonitorCH_Gate, 1000);
	(*(arrary+1)).numFloat = GetMonitorVoltage(&hMonitorCH_Source, 1000);
	(*(arrary+2)).numFloat = GetMonitorVoltage(&hMonitorCH_Drain, 1000);
	(*(arrary+3)).numFloat = GetMonitorVoltage(&hMonitorCH_Sub, 1000);
	ClearAllVoltage();
}

static void OutputVoltageCalibration(void)
{
	//У׼���
	OutputVoltage(DRAIN, 0);				//ʹ��ԭʼ��ѹ���
	OutputVoltage(GATE, 0);				//ʹ��ԭʼ��ѹ���
	HAL_Delay(200);
	Adj_OutputOffset[2].numFloat=GetMonitorVoltage(&hMonitorCH_Drain, 2000)*10;
	Adj_OutputOffset[0].numFloat=GetMonitorVoltage(&hMonitorCH_Gate, 2000)*10;
	//У׼���Զ�,���Զ��޷�У׼
	//OutputAdjVoltage(DRAIN, 2);
	//OutputAdjVoltage(GATE, 2);
	//Adj_OutputLinear[2].numFloat=GetMonitorVoltage(&hMonitorCH_Drain, 2000);
	//Adj_OutputLinear[0].numFloat=GetMonitorVoltage(&hMonitorCH_Gate, 2000);	
}

/*У׼��·����*/
static void ZeroCrrentCalibration(void)
{ 
	OutputAdjVoltage(DRAIN, 0);   				//����ض���ѹ
	OutputAdjVoltage(DRAIN, 0);   				//����ض���ѹ
	for(uint8_t i=0;i<9;i++)							//9����λ
	{
		float temp_value=0, temp_sum=0;
		SetRangeRelay(i+1);
		HAL_Delay(1000+i*1000);
		for(uint8_t j=0;j<10;j++)		//10�βɼ�ȡƽ��ֵ
		{
			temp_value=(float)AD7988_1_ReadData(&hAD7988_11)*2.5/65535-1.25;//ADC�ɼ�����
			temp_sum+=temp_value;
			HAL_Delay(2);
		}
		Adj_RelayOffset[i].numFloat=(float)(temp_sum/10);//AD�ɼ�ƽ��ֵ
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
	  HAL_Delay(200); //��Drain������
		SetRangeRelay(Uart1RxBuf[3]);
		HAL_Delay(200);//�л���λ��ע����λ��һ������ȷ�ĵ�λ
		OutputAdjVoltage(DRAIN,1000);
		HAL_Delay(200);
		//temp=GetMonitorVoltage(&hMonitorCH_Drain, 2000);
		for(uint8_t j=0;j<10;j++)		//10�βɼ�ȡƽ��ֵ
		{
			temp_value=(float)AD7988_1_ReadData(&hAD7988_11)*2.5/65535-1.25;//ADC�ɼ�����
			temp_sum+=temp_value;
			HAL_Delay(2);
		}
		temp=(float)(temp_sum/10);//AD�ɼ�ƽ��ֵ
		Cali_Crrent_Pos(Adj_PosRelayLinear,temp,Uart1RxBuf[3]);
		
		OutputAdjVoltage(DRAIN,-1000);   //����ض���ѹ
		HAL_Delay(200);
		temp_value=0, temp_sum=0;
		for(uint8_t j=0;j<10;j++)		//10�βɼ�ȡƽ��ֵ
		{
			temp_value=(float)AD7988_1_ReadData(&hAD7988_11)*2.5/65535-1.25;//ADC�ɼ�����
			temp_sum+=temp_value;
			HAL_Delay(2);
		}
		temp=(float)(temp_sum/10);//AD�ɼ�ƽ��ֵ
		Cali_Crrent_Neg(Adj_NegRelayLinear,temp,Uart1RxBuf[3]);
	}
	  ClearAllVoltage();
	  DisconnectAllOutput();
}  
//У׼������
void Cali_Crrent_Pos(FloatUnion* arrary,float temp,uint8_t index)
{
	(*(arrary+index-1)).numFloat=(temp-Adj_RelayOffset[index-1].numFloat)/1;
}
//У׼������
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

/*У׼���Ե�����ʼ��*/
/*float Cali_TestCrrent_Init(FloatUnion* arrary, uint8_t index, int16_t outputVoltage)
{
	float temp,temp_value=0,temp_sum=0;

	(*(arrary+index)).numFloat=0;
	range_select=index+1;
	OutputAdjVoltage(DRAIN, outputVoltage);   //����ض���ѹ
	HAL_Delay(100);	 //��ʱ�ȴ������ѹ�����ض�
	SetRangeRelay(range_select);
	HAL_Delay(100);
	for(uint8_t i=0;i<10;i++)
	{
		temp_value=(float)AD7988_1_ReadData(&hAD7988_11)*2.5/65535-1.25;//ADC�ɼ�����
		temp_sum+=temp_value;
		HAL_Delay(10);
	}
	temp=(float)(temp_sum/10);//AD�ɼ�ƽ��ֵ
	ClearAllVoltage();
	//range_select=4;
	//SetRangeRelay(range_select);
	//HAL_Delay(100);
	return temp;
}*/
//100ŷ����У׼������
/*void Cali_LOAD100_Crrent_Pos(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_1400mV);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/0.714;
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/0.2;
}
//100ŷ����У׼������
void Cali_LOAD100_Crrent_Neg(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_N1400mV);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/(-0.714);
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_N200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/(-0.2);
}
//10K��1M����У׼������
void Cali_LOAD10K_1M_Crrent_Pos(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_2V);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/0.2;
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/0.2;
}
//10K��1M����У׼������
void Cali_LOAD10K_1M_Crrent_Neg(FloatUnion* arrary, uint8_t index)
{
	float temp;
	temp=Cali_TestCrrent_Init(&(*arrary), index, VOL_N2V);
	(*(arrary+index)).numFloat=(temp-Adj_RelayOffset[index].numFloat)/(-0.2);
	
	temp=Cali_TestCrrent_Init(&(*arrary), index+1, VOL_N200mV);
	(*(arrary+index+1)).numFloat=(temp-Adj_RelayOffset[index+1].numFloat)/(-0.2);
}
//100M����У׼������
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
//100M����У׼������
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

/*У׼����*/
/*void Do_Calibration(TestPara_TypeDef* pTestPara)
{
	if(pTestPara->caliMode==OUTPUTVOLTAGE_ZERO)               //���У׼�����ѹ
		Cali_SetRangeRelay(Adj_OutputOffset, 4, VOL_0V);    
	else if(pTestPara->caliMode==OUTPUTVOLTAGE_20V)           //20VУ׼�����ѹ��Ч
		Cali_SetRangeRelay(Adj_OutputLinear, 4, VOL_20V);	
	else if(pTestPara->caliMode==OPEN)                        //��·У׼����ƫ����
		Cali_OpenCrrent(Adj_RelayOffset, 9, VOL_0V);
	else if(pTestPara->caliMode==LOAD100||pTestPara->caliMode==LOAD10k||pTestPara->caliMode==LOAD1M||pTestPara->caliMode==LOAD100M) 
	{
		                   
		if(pTestPara->caliMode==LOAD100)                        //100ŷ����У׼����
		{
			Cali_LOAD100_Crrent_Pos(Adj_PosRelayLinear, 0);
			Cali_LOAD100_Crrent_Neg(Adj_NegRelayLinear, 0);
		}		
		else if(pTestPara->caliMode==LOAD10k)                   //10K����У׼����
		{
			Cali_LOAD10K_1M_Crrent_Pos(Adj_PosRelayLinear, 2);
			Cali_LOAD10K_1M_Crrent_Neg(Adj_NegRelayLinear, 2);
		}		
		else if(pTestPara->caliMode==LOAD1M)                    //1M����У׼����
		{
			Cali_LOAD10K_1M_Crrent_Pos(Adj_PosRelayLinear, 4);
			Cali_LOAD10K_1M_Crrent_Neg(Adj_NegRelayLinear, 4);
		}
		else if(pTestPara->caliMode==LOAD100M)                 //100M����У׼����
		{
			Cali_LOAD100M_Crrent_Pos(Adj_PosRelayLinear, 6);
			Cali_LOAD100M_Crrent_Neg(Adj_NegRelayLinear, 6);
		}
			
	}
	else                                                           //��·У׼��Ч
		ClearAllVoltage();
}*/

/*����У׼����*/		
void FlashSaveCaliData(uint32_t start_address, uint32_t end_address)
{
	uint32_t data=0;	
	HAL_FLASH_Unlock();                                //����FLASHУ׼����
	FlashErase(start_address,end_address);             //�����û�ҳ,������ҳ����
	
	for(uint8_t i=0; i<4; i++)                         //���������ѹУ׼ƫ����ֵ
	{
		for(uint8_t j=0;j<4;j++)
		{
			data<<=8;
			data|=Adj_OutputOffset[i].numUchar[3-j];
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_OFFSET_ADDR+i*4, data);
	}
	
	//����������Զ��޷�����У׼���˴�����
//	for(uint8_t i=0; i<4; i++)                         //���������ѹУ׼������ֵ
//	{
//		for(uint8_t j=0;j<4;j++)
//		{
//			data<<=8;
//			data|=Adj_OutputLinear[i].numUchar[3-j];
//		}
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_LINEAR_ADDR+i*4, data);
//	}
	
	for(uint8_t i=0; i<9; i++)                         //����һ���Ŵ����У׼ƫ����ֵ
	{
		for(uint8_t j=0;j<4;j++)
		{
			data<<=8;
			data|=Adj_RelayOffset[i].numUchar[3-j];
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_RELAY_OFFSET_ADDR+i*4, data);
	}
	
	for(uint8_t i=0; i<9; i++)                         //����һ���Ŵ����У׼������ֵ
	{
		for(uint8_t j=0;j<4;j++)
		{
			data<<=8;
			data|=Adj_PosRelayLinear[i].numUchar[3-j];
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_USER_START_ADDR+OUTPUT_RELAY_LINEAR_ADDR+i*4, data);
	}
	
	for(uint8_t i=0; i<9; i++)                         //����һ���Ŵ����У׼������ֵ
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

