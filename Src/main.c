/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "hardware.h"
#include "process_ctrl.h"
#include "relay.h"
#include "calculation.h"
#include "calibration.h"
#include "USB_TX_protocol.h"
#include "InternalFlash.h"
#include "AD7791.h"
#include "voltage_monitor.h"
#include <stdio.h>

#define ID "6101001071101010017"

/* Private variables ---------------------------------------------------------*/ 
enum MsgType msgType=MSG_TYPE_NULL;
enum ADC_Status ADC_status=ADC_BUSY;
TestPara_TypeDef TestPara;
TestResult_TypeDef TestResult;

void OutputNextVd(void);
void OutputNextVg(void);
void StartNextSampling(void);

uint8_t Uart1RxBuf[512]={0};                                            //������λ�����ݻ�������
uint8_t Uart1TxBuf[512]={0};                                            //���͸���λ�����ݻ�������
uint8_t RxComplete=0;
uint8_t TxComplete=0;
int16_t quietTimeTick=0;                      													//ADC�ɼ�״̬
uint8_t firstDataReady=0;										//��һ�����ݾ�λ��ֻ�е�һ�����ݾ�λ��ɨ���ѹ�������ӣ��������ɨ���ѹ������һ��������

int main(void)
{
	uint8_t* pTxBuf=Uart1TxBuf;																									//��ɵĲ�������
	Hardware_Init();																											//��ʼ��Ӳ��
	//InitUSBVar();																												//��ʼ��USBͨ�Žṹ����																								//��ʼ��485��Ϣ
	InitTestPara(&TestPara);																							//��ʼ�����Խṹ����� 																											//��Drain Sub Gate�̵�������																										
	HAL_UART_Receive_DMA(&huart1, Uart1RxBuf, UART_RX_LEN);					//�������մ���1 RS485����
	
	while (1)
  {	
		if(msgType!=MSG_TYPE_NULL)																			//���485�����յ���Ϣ
		{
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);									//ָʾ����˸
			if(msgType==MSG_TYPE_SETTING)																	//�������������
			{
				GetTestPara(&TestPara, &Relay, Uart1RxBuf);									//���ղ�ת�����Բ���
				//GetRelayPara(&Relay);��ȡ���̵�λ��������ʱ���ٵ���ʱ��
				OutputAdjVoltage(GATE, TestPara.VgNow);												//���������ѹ���ʹ��ѹ�ȶ�
				OutputAdjVoltage(DRAIN, TestPara.VdNow);
			}
			else if(msgType==MSG_TYPE_RUN)																					//�������������
			{
				TestPara.testCmd=CMD_START;	
				TestResult.endOfTest=0;
				pTxBuf=Uart1TxBuf;																													//��մ��㿪ʼ
				firstDataReady=0;
				ADC_status=ADC_BUSY;
				GetRelayPara(&Relay);
				ConnectAllOutput();	
				InitTestResult(&TestResult);
				SetTimerPara(&TestPara);																							//���ö�ʱ������	
				SetTimerAction(&TestPara);													      						//������ʱ��
			}
			else if(msgType==MSG_TYPE_STOP)																					//�����ֹͣ����
			{
				TestPara.testCmd=CMD_STOP;
				SetTimerAction(&TestPara);
				ADC_status=ADC_BUSY;										//ֹͣ��ʱ��
				DisconnectAllOutput();
			}
			//else if(msgType==MSG_TYPE_QUERY)																					//����ǲ�ѯ����
			//{
				
			//}
			else if(msgType==MSG_TYPE_REPORT_ID)
			{
				Uart1TxBuf[0]=DEV_ADDR;
				sprintf((char*)Uart1TxBuf+1, (const char*)ID);
				HAL_Delay(Uart1TxBuf[UART_TX_LEN-1]*50+50);						
				//���һλ�����к� ��50ms��ʼ��һֱ��550ms������ʱ
				HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET);//ʹ�ܴ���1����
				HAL_UART_Transmit(&huart1, Uart1TxBuf, UART_TX_LEN, 1000);
				HAL_Delay(10);
				HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_RESET);
			}
			else if(msgType==MSG_TYPE_CALIBRATION)
			{
					RunCalibration(Uart1RxBuf);
			}
			msgType=MSG_TYPE_NULL;
		}
		if(RxComplete==1)
		{
			RxComplete=0;
			msgType=(enum MsgType)GetMsgType(Uart1RxBuf);
			if(msgType==MSG_TYPE_QUERY)
			{
				if((pTxBuf!=Uart1TxBuf)&&(TxComplete==1))
				{
					TxComplete=0;	//��ʾDMA�������ڷ���
					Uart1TxBuf[0]=DEV_ADDR;                        												//�豸��ַ	
					HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET);		//ʹ�ܷ���
					HAL_UART_Transmit_DMA(&huart1, Uart1TxBuf, pTxBuf-Uart1TxBuf);
					HAL_Delay(2);			//�����ӳ�2ms��485�����ٶ���������������ݴ�λ																										//���н���ѷ��ͣ�����
					pTxBuf=Uart1TxBuf;
				}
			}
			HAL_UART_Receive_DMA(&huart1, Uart1RxBuf, UART_RX_LEN+1);		//�˴��������ӽ��ճ���	
		}
		//���ݲɼ�����
		if(ADC_status==ADC_READY)																								//���ADC��ʱ���ɵ���Ч����	
		{																								//�����־λ
			if(RelayCheck(TestPara.testMode, &Relay, TestResult.I_sample)==0)												//���ҵ����ʵ�λ
			{
				if(Do_Calculation(&TestPara, &TestResult, &Relay)==0)							//����ڲ��Թ����в�������ȷ����
				{
					HAL_TIM_Base_Stop_IT(&htim2);															//��ǰ���ݻ�ȡ��ϣ�ֹͣ������ʱ��
					HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
					Relay.rangeChangeTimes=0;																	//�Զ�������������
					TestResult.I_avg.numFloat=CommitAdjustment(TestResult.I_avg.numFloat, &Relay);	//�Բɼ����ĵ����������ݴ���
					//TestResult.test_ADC_DrainVol.numFloat=GetMonitorVoltage(&hMonitorCH_Drain, 20);  				//�ϴ���ص���©����ѹ
					//TestResult.test_ADC_GateVol.numFloat=GetMonitorVoltage(&hMonitorCH_Gate, 20);   				//�ϴ���ص���դ����ѹ	
					if((quietTimeTick==-1) && (pTxBuf-Uart1TxBuf<500))				//��ֹ���
					{
						pTxBuf=prepareTxData(&TestPara, &TestResult, pTxBuf);//�������������ĵ�һ��λ��ָ�봫��
						firstDataReady=1;
						InitTestResult(&TestResult);
						if(TestPara.testMode==SWEEP_DRAIN_VOL||TestPara.testMode==SWEEP_IV)
							OutputNextVd();
						else if(TestPara.testMode==SWEEP_GATE_VOL)
							OutputNextVg();
					}
					else
						InitTestResult(&TestResult);
				}
			}
			else
			{
				InitTestResult(&TestResult);											//������Ҫ��ղ������
				if(TestPara.testMode==SWEEP_DRAIN_VOL||TestPara.testMode==SWEEP_GATE_VOL||TestPara.testMode==SWEEP_IV)
				{
					htim3.Instance->CNT=0;
					HAL_TIM_Base_Start_IT(&htim3);
				}
				StartNextSampling();
			}
			ADC_status=ADC_BUSY;	
		}	
	}		
}


/* USER CODE BEGIN 4 */
void EnterEndofTest(void)
{
	HAL_TIM_Base_Stop_IT(&htim3);																		//DAC��������жϹر�		
	HAL_TIM_Base_Stop_IT(&htim2);
	ADC_status=ADC_BUSY;										//�����������Ļ�������Ȼ��ready״̬	
	TestPara.testStatus=OFF;
	ClearAllVoltage();			//Disconnect();																				//�����ѹ����
}

void StartNextSampling(void)
{
	//delay_us(2);											//��TIM3�Ŀ�ʼ��ʱʱ������
	htim2.Instance->CNT=0;						//����������
	//HAL_NVIC_DisableIRQ(USART1_IRQn);	//�ɼ���ʼ��ֹͣ��Ӧ�����ж�		
	//HAL_UART_AbortReceive(&huart1);
	HAL_TIM_Base_Start_IT(&htim2);		//���һ�βſ�ʼAD�ɼ����ж�
		
}

void OutputNextVd(void)		//return value endOfTest
{
	if(TestPara.VdStart<=TestPara.VdEnd) 
	{						
		if(firstDataReady==1)			//ֻ�й��˾�Ĭʱ��������Ӳ��ҵ�һ�����ݾ�λ
			TestPara.VdNow+=abs(TestPara.VdStep); //�����µ�©����ѹ
		if(TestPara.VdNow>TestPara.VdEnd) EnterEndofTest();
		else
		{
			OutputAdjVoltage(DRAIN, TestPara.VdNow);//����µ�©����ѹ
		}
	
		if(TestPara.VdNow+abs(TestPara.VdStep)>TestPara.VdEnd) TestResult.endOfTest=1;
	}
	else
	{
			if(firstDataReady==1)
				TestPara.VdNow-=abs(TestPara.VdStep); //�����µ�©����ѹ
			if(TestPara.VdNow<TestPara.VdEnd)	EnterEndofTest();
			else
			{
				OutputAdjVoltage(DRAIN, TestPara.VdNow);	//����µ�©����ѹ
			}
		if(TestPara.VdNow-abs(TestPara.VdStep)<TestPara.VdEnd) TestResult.endOfTest=1;																											
	}
}

void OutputNextVg(void)		//return value endOfTest
{
	 if(TestPara.VgStart<=TestPara.VgEnd) 
	{						
		if(firstDataReady==1)			//ֻ�й��˾�Ĭʱ��������Ӳ��ҵ�һ�����ݾ�λ
			TestPara.VgNow+=abs(TestPara.VgStep); //�����µ�դ����ѹ
		if(TestPara.VgNow>TestPara.VgEnd) EnterEndofTest();
		else
		{
			OutputAdjVoltage(GATE, TestPara.VgNow);//����µ�դ����ѹ
		}
	
		if(TestPara.VgNow+abs(TestPara.VgStep)>TestPara.VgEnd) TestResult.endOfTest=1;
	}
	else
	{
			if(firstDataReady==1)
				TestPara.VgNow-=abs(TestPara.VgStep); //�����µ�դ����ѹ
			if(TestPara.VgNow<TestPara.VgEnd)	EnterEndofTest();
			else
			{
				OutputAdjVoltage(GATE, TestPara.VgNow);	//����µ�դ����ѹ
			}
		if(TestPara.VgNow-abs(TestPara.VgStep)<TestPara.VgEnd) TestResult.endOfTest=1;																											
	}
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
		if (htim->Instance == htim2.Instance)					//ADC��ʱ��
    {
				if(ADC_status==ADC_BUSY)//����ʲô���������ɼ���ѹ�͵���
				{
					TestResult.I_sample=AD7988_1_ReadData(&hAD7988_11);//ADC�ɼ�����
					ADC_status=ADC_READY;
				}
    }
	  else if (htim->Instance == htim3.Instance)		//DAC��ʱ��
		{
				StartNextSampling();
		}
		else if (htim->Instance == htim4.Instance)		//DAC��ʱ��
		{
				quietTimeTick++;
				if(quietTimeTick>=TestPara.quietTime)
				{
					HAL_TIM_Base_Stop_IT(&htim4);
					HAL_TIM_Base_Stop_IT(&htim2);
					ADC_status=ADC_BUSY;
					InitTestResult(&TestResult);
					quietTimeTick=-1;										//�Ѿ�����quietTime����ʱ��
					HAL_TIM_Base_Start_IT(&htim3);			//��ʼɨ��
					StartNextSampling();
				}
		}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
