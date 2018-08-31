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
 
uint8_t Uart1RxBuf[512]={0};                                            //接收上位机数据缓冲数组
uint8_t Uart1TxBuf[512]={0};                                            //发送给上位机数据缓冲数组
uint8_t RxComplete=0;
uint8_t TxComplete=0;
int16_t quietTimeTick=0;                      													//ADC采集状态
uint8_t firstDataReady=0;										//第一个数据就位，只有第一个数据就位后扫描电压才能增加，否则出现扫描电压跳过第一个点的情况

int main(void)
{
	uint8_t resultNum=0;																									//完成的测量次数
	Hardware_Init();																											//初始化硬件
	//InitUSBVar();																												//初始化USB通信结构参数																								//初始化485消息
	InitTestPara(&TestPara);																							//初始化测试结构体参数 																											//将Drain Sub Gate继电器连接																										

	HAL_UART_Receive_DMA(&huart1, Uart1RxBuf, 32);					//持续接收串口1 RS485数据
	while (1)
  {	
		if(msgType!=MSG_TYPE_NULL)																			//如果485总线收到信息
		{
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);									//指示灯闪烁
			if(msgType==MSG_TYPE_SETTING)																	//如果是设置命令
			{
				GetTestPara(&TestPara, &Relay, Uart1RxBuf);									//接收并转化测试参数
				OutputAdjVoltage(GATE, TestPara.VgNow);												//立即输出电压早点使电压稳定
				OutputAdjVoltage(DRAIN, TestPara.VdNow);
			}
			else if(msgType==MSG_TYPE_RUN)																					//如果是启动命令
			{
				TestPara.testCmd=CMD_START;	
				TestResult.endOfTest=0;
				resultNum=0;																													//清空从零开始
				firstDataReady=0;
				InitTestResult(&TestResult);
				GetRelayPara(&Relay);
				ConnectAllOutput();	
				SetTimerPara(&TestPara);																							//设置定时器参数	
				SetTimerAction(&TestPara);													      						//开启定时器
			}
			else if(msgType==MSG_TYPE_STOP)																					//如果是停止命令
			{
				TestPara.testCmd=CMD_STOP;
				ADC_status=ADC_BUSY;	
				SetTimerAction(&TestPara);																						//停止定时器
				DisconnectAllOutput();
			}
			else if(msgType==MSG_TYPE_QUERY)																					//如果是查询命令
			{
				if(resultNum>0)
				{
						Uart1TxBuf[0]=DEV_ADDR;                        												//设备地址	
						HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET);		//使能发送
						HAL_UART_Transmit_DMA(&huart1, Uart1TxBuf, resultNum*UART_TX_LEN);
						resultNum=0;																													//所有结果已发送，归零
				}
			}
			else if(msgType==MSG_TYPE_REPORT_ID)
			{
				Uart1TxBuf[0]=DEV_ADDR;
				sprintf((char*)Uart1TxBuf+1, (const char*)ID);
				HAL_Delay(Uart1TxBuf[UART_TX_LEN-1]*50+50);						
				//最后一位是序列号 从50ms开始，一直到550ms特征延时
				HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET);//使能串口1发送
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
		
		if(TxComplete==1)
		{
			TxComplete=0;
			//HAL_Delay(2);			//虽然单片机发完了，但是485芯片并没有发完
			HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_RESET);	//使能485接收
		}
		if(RxComplete==1)
		{
			HAL_UART_Receive_DMA(&huart1, Uart1RxBuf, 512);					//持续接收串口1 RS485数据
			RxComplete=0;
			if(Uart1RxBuf[0]==DEV_ADDR||Uart1RxBuf[0]==0x00)			//如果是设备地址或者是广播地址
			{
				uint8_t xorCheck=0;
				for(uint8_t j=0;j<UART_RX_LEN-1;j++)                //将接收到的数据按照协议进行校验
				{
					xorCheck^=Uart1RxBuf[j];
				}
				if(xorCheck==Uart1RxBuf[UART_RX_LEN-1])
				{
						msgType=(enum MsgType)Uart1RxBuf[1];
				}
			}
		}
		//数据采集部分
		
		if(ADC_status==ADC_READY)																								//如果ADC定时器采到有效数据	
		{
			ADC_status=ADC_BUSY;																									//清除标志位
		 if(RelayCheck(&Relay, TestResult.I_sample)==0)												//当找到合适档位
			{
				if(Do_Calculation(&TestPara, &TestResult, &Relay)==0)							//如果在测试过程中并且有正确数据
				{
					HAL_TIM_Base_Stop_IT(&htim2);															//当前数据获取完毕，停止采样定时器
					//只有停止采集时才响应串口中断
					HAL_NVIC_EnableIRQ(USART1_IRQn);
					HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
					Relay.rangeChangeTimes=0;																					//自动换挡次数清零
					TestResult.I_avg.numFloat=CommitAdjustment(TestResult.I_avg.numFloat, &Relay);	//对采集到的电流进行数据处理
					//TestResult.currentResultValid=1;
					//TestResult.test_ADC_DrainVol.numFloat=GetMonitorVoltage(&hMonitorCH_Drain, 20);  				//上传监控到的漏极电压
					//TestResult.test_ADC_GateVol.numFloat=GetMonitorVoltage(&hMonitorCH_Gate, 20);   				//上传监控到的栅极电压	
					if(quietTimeTick==-1 && resultNum<10)				//防止溢出
					{
						prepareTxData(&TestPara, &TestResult, Uart1TxBuf+resultNum*UART_TX_LEN);//将发送数组空余的第一个位置指针传递
						resultNum++;
						firstDataReady=1;
					}
					InitTestResult(&TestResult);
				}
			}
			else
			{
				ADC_status=ADC_BUSY;
				InitTestResult(&TestResult);											//换挡需要清空测量结果
				HAL_TIM_Base_Start_IT(&htim2);										//在relay换挡时已经停止寄存器了，需要在这里重新打开
			}
		}
	}		
}


/* USER CODE BEGIN 4 */
void EnterEndofTest(void)
{
	HAL_TIM_Base_Stop_IT(&htim3);																		//DAC输出数据中断关闭		
	HAL_TIM_Base_Stop_IT(&htim2);	
	TestPara.testStatus=OFF;
	//ClearAllVoltage();			Disconnect();																				//输出电压清零
}

void StartNextSampling(void)
{
	delay_us(2);											//与TIM3的开始计时时间拉开
	htim2.Instance->CNT=0;						//计数器清零
	HAL_NVIC_DisableIRQ(USART1_IRQn);	//采集开始后停止响应串口中断		
	HAL_TIM_Base_Start_IT(&htim2);		//输出一次才开始AD采集的中断
		
}

void OutputNextVd(void)		//return value endOfTest
{
	if(TestPara.VdStart<=TestPara.VdEnd) 
	{						
		if(firstDataReady==1)			//只有过了静默时间才能增加并且第一个数据就位
			TestPara.VdNow+=abs(TestPara.VdStep); //计算新的漏极电压
		if(TestPara.VdNow>TestPara.VdEnd) EnterEndofTest();
		else
		{
			OutputAdjVoltage(DRAIN, TestPara.VdNow);//输出新的漏极电压
			StartNextSampling();								 //开始AD采集的中断
		}
	
		if(TestPara.VdNow+abs(TestPara.VdStep)>TestPara.VdEnd) TestResult.endOfTest=1;
	}
	else
	{
			if(firstDataReady==1)
				TestPara.VdNow-=abs(TestPara.VdStep); //计算新的漏极电压
			if(TestPara.VdNow<TestPara.VdEnd)	EnterEndofTest();
			else
			{
				OutputAdjVoltage(DRAIN, TestPara.VdNow);	//输出新的漏极电压
				StartNextSampling();									//开始AD采集的中断
			}
		if(TestPara.VdNow-abs(TestPara.VdStep)<TestPara.VdEnd) TestResult.endOfTest=1;																											
	}
}

void OutputNextVg(void)		//return value endOfTest
{
	 if(TestPara.VgStart<=TestPara.VgEnd) 
	{						
		if(firstDataReady==1)			//只有过了静默时间才能增加并且第一个数据就位
			TestPara.VgNow+=abs(TestPara.VgStep); //计算新的栅极电压
		if(TestPara.VgNow>TestPara.VgEnd) EnterEndofTest();
		else
		{
			OutputAdjVoltage(GATE, TestPara.VgNow);//输出新的栅极电压
			StartNextSampling();								 //开始AD采集的中断
		}
	
		if(TestPara.VgNow+abs(TestPara.VgStep)>TestPara.VgEnd) TestResult.endOfTest=1;
	}
	else
	{
			if(firstDataReady==1)
				TestPara.VgNow-=abs(TestPara.VgStep); //计算新的栅极电压
			if(TestPara.VgNow<TestPara.VgEnd)	EnterEndofTest();
			else
			{
				OutputAdjVoltage(GATE, TestPara.VgNow);	//输出新的栅极电压
				StartNextSampling();									//开始AD采集的中断
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
    
		if (htim->Instance == htim2.Instance)					//ADC定时器
    {
				
				TestResult.I_sample=AD7988_1_ReadData(&hAD7988_11);//ADC采集数据
				ADC_status=ADC_READY;
    }
	  else if (htim->Instance == htim3.Instance)		//DAC定时器
		{
			//TestResult.currentResultValid=0;
			if(TestPara.testMode==SWEEP_DRAIN_VOL||TestPara.testMode==SWEEP_IV)
			{
				OutputNextVd();	
			}
			else if(TestPara.testMode==SWEEP_GATE_VOL)
			{
				OutputNextVg();
			}
			else if(TestPara.testMode==NO_SWEEP_IT||TestPara.testMode==NO_SWEEP_RT||TestPara.testMode==ID_T)
			{
				StartNextSampling();
			}
		}
		else if (htim->Instance == htim4.Instance)		//DAC定时器
		{
				quietTimeTick++;
				if(quietTimeTick>=TestPara.quietTime)
				{
					HAL_TIM_Base_Stop_IT(&htim4);
					HAL_TIM_Base_Stop_IT(&htim2);
					ADC_status=ADC_BUSY;
					InitTestResult(&TestResult);
					quietTimeTick=-1;										//已经到达quietTime计数时间
					HAL_TIM_Base_Start_IT(&htim3);			//开始扫描
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
