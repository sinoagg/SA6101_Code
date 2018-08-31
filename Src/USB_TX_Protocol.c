#include "USB_TX_protocol.h"

uint8_t UsartTest_SendBuf[4]={0};	

uint8_t HID_SendBuf[USB_TX_LEN]={0};		//USB transmission buffer


void USB_TX_Message(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult, uint8_t EndOfTest)
{
	if(pTestPara->testMode==NO_SWEEP_IT||pTestPara->testMode==NO_SWEEP_RT||pTestPara->testMode==ID_T)
	{
		HID_SendBuf[2]=(uint8_t)(((-pTestPara->VdStart*10+25000)&0xFF00)>>8);
		HID_SendBuf[3]=(uint8_t)((-pTestPara->VdStart*10+25000)&0x00FF);
		HID_SendBuf[4]=(uint8_t)(((-pTestPara->VgStart*10+25000)&0xFF00)>>8);
		HID_SendBuf[5]=(uint8_t)((-pTestPara->VgStart*10+25000)&0x00FF);
		HID_SendBuf[6]=(uint8_t)(((-pTestPara->VsStart*10+25000)&0xFF00)>>8);
		HID_SendBuf[7]=(uint8_t)((-pTestPara->VsStart*10+25000)&0x00FF);		
	}
	else if(pTestPara->testMode==SWEEP_GATE_VOL)
	{
		HID_SendBuf[2]=(uint8_t)(((-pTestPara->VdStart*10+25000)&0xFF00)>>8);
		HID_SendBuf[3]=(uint8_t)((-pTestPara->VdStart*10+25000)&0x00FF);
		HID_SendBuf[4]=(uint8_t)(((-pTestPara->VgNow*10+25000)&0xFF00)>>8);
		HID_SendBuf[5]=(uint8_t)((-pTestPara->VgNow*10+25000)&0x00FF);
		HID_SendBuf[6]=(uint8_t)(((-pTestPara->VsStart*10+25000)&0xFF00)>>8);
		HID_SendBuf[7]=(uint8_t)((-pTestPara->VsStart*10+25000)&0x00FF);	
	}
	else if(pTestPara->testMode==SWEEP_DRAIN_VOL)
	{
		HID_SendBuf[2]=(uint8_t)(((-pTestPara->VdNow*10+25000)&0xFF00)>>8);
		HID_SendBuf[3]=(uint8_t)((-pTestPara->VdNow*10+25000)&0x00FF);
		HID_SendBuf[4]=(uint8_t)(((-pTestPara->VgStart*10+25000)&0xFF00)>>8);
		HID_SendBuf[5]=(uint8_t)((-pTestPara->VgStart*10+25000)&0x00FF);
		HID_SendBuf[6]=(uint8_t)(((-pTestPara->VsStart*10+25000)&0xFF00)>>8);
		HID_SendBuf[7]=(uint8_t)((-pTestPara->VsStart*10+25000)&0x00FF);		
	}
	HID_SendBuf[1]=EndOfTest;																							//数据是否为最后一组数据
	HID_SendBuf[8]=pTestResult->I_avg.numUchar[3];																		
	HID_SendBuf[9]=pTestResult->I_avg.numUchar[2];																		
	HID_SendBuf[10]=pTestResult->I_avg.numUchar[1];																	
	HID_SendBuf[11]=pTestResult->I_avg.numUchar[0];																	
	//HID_SendBuf[12]=range_select;
	HID_SendBuf[13]=0;
	HID_SendBuf[14]=0;	
	HID_SendBuf[15]=0;
	HID_SendBuf[16]=0;	
	HID_SendBuf[17]=0;
	HID_SendBuf[18]=0;	
	//HID_SendBuf[19]=(uint8_t)((pTestResult->water_level&0xFF00)>>8);
	//HID_SendBuf[20]=(uint8_t)(pTestResult->water_level&0x00FF);	
	HID_SendBuf[21]=pTestResult->test_ADC_DrainVol.numUchar[3];																		
	HID_SendBuf[22]=pTestResult->test_ADC_DrainVol.numUchar[2];																		
	HID_SendBuf[23]=pTestResult->test_ADC_DrainVol.numUchar[1];																	
	HID_SendBuf[24]=pTestResult->test_ADC_DrainVol.numUchar[0];
	HID_SendBuf[25]=pTestResult->test_ADC_GateVol.numUchar[3];																		
	HID_SendBuf[26]=pTestResult->test_ADC_GateVol.numUchar[2];																		
	HID_SendBuf[27]=pTestResult->test_ADC_GateVol.numUchar[1];																	
	HID_SendBuf[28]=pTestResult->test_ADC_GateVol.numUchar[0];
//	HID_SendBuf[29]=channel_index;
	
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_SendBuf, USB_TX_LEN);
}

void USB_Respond(void)
{
	HAL_Delay(10);
	HID_SendBuf[1]=0xF1;																							//先回传指令确认收到
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_SendBuf, USB_TX_LEN);
}

void USB_Update(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult)
{
	if(pTestPara->testMode==NO_SWEEP_IT||pTestPara->testMode==NO_SWEEP_RT||pTestPara->testMode==ID_T)
	{ 
		USB_TX_Message(pTestPara, pTestResult, NOTENDOFTEST);		//USB上传数据	
	}
	else if(pTestPara->testMode==SWEEP_GATE_VOL)
	{
		if(pTestPara->VgStart>=pTestPara->VgEnd)
		{
			if(pTestPara->VgNow>=pTestPara->VgEnd)
			{
				USB_TX_Message(pTestPara, pTestResult, NOTENDOFTEST);
				pTestPara->VgNow+=pTestPara->VgStep;						//设置新的栅极电压
				if(pTestPara->VgNow<pTestPara->VgEnd)																														//扫描结束																												
				{
					HAL_TIM_Base_Stop_IT(&htim3);																		//DAC输出数据中断关闭		
					HAL_TIM_Base_Stop_IT(&htim2);	
					HAL_Delay(100);
					pTestPara->testStatus=OFF;
					USB_TX_Message(pTestPara, pTestResult, ENDOFTEST);							//结束测试,发送最后一组F3开头的命令
					ClearAllVoltage();																							//输出电压清零
				}
			}
		}
		else
		{
			if(pTestPara->VgNow<=pTestPara->VgEnd)
			{
				USB_TX_Message(pTestPara, pTestResult, NOTENDOFTEST);
				pTestPara->VgNow-=pTestPara->VgStep;						//设置新的栅极电压
				if(pTestPara->VgNow>pTestPara->VgEnd)																												//扫描结束																												
				{
					HAL_TIM_Base_Stop_IT(&htim3);																		//DAC输出数据中断关闭		
					HAL_TIM_Base_Stop_IT(&htim2);	
					//HAL_TIM_Base_Stop_IT(&htim4);                                   //RS485温湿度采集中断关闭
					HAL_Delay(100);
					pTestPara->testStatus=OFF;
					USB_TX_Message(pTestPara, pTestResult, ENDOFTEST);			//结束测试,发送最后一组F3开头的命令
					ClearAllVoltage();																							//输出电压清零
				}
			}
		}
	}	
	else if(pTestPara->testMode==SWEEP_DRAIN_VOL)
	{
		if(pTestPara->VdStart>=pTestPara->VdEnd)
		{
			if(pTestPara->VdNow>=pTestPara->VdEnd )
			{
				USB_TX_Message(pTestPara, pTestResult, NOTENDOFTEST);	
				pTestPara->VdNow+=pTestPara->VgStep;						//设置新的漏极电压
				if(pTestPara->VdNow<pTestPara->VdEnd)
				{
					HAL_TIM_Base_Stop_IT(&htim3);																		//DAC输出数据中断关闭		
					HAL_TIM_Base_Stop_IT(&htim2);					                          //ADC采集电流中断关闭
					//HAL_TIM_Base_Stop_IT(&htim4);                                   //RS485温湿度采集中断关闭
					HAL_Delay(100);
					pTestPara->testStatus=OFF;
					USB_TX_Message(pTestPara, pTestResult, ENDOFTEST);							//结束测试,发送最后一组F3开头的命令
					ClearAllVoltage();																							//输出电压清零
				}
			}
		}
		else
		{
			if(pTestPara->VdNow<=pTestPara->VdEnd )
			{
				USB_TX_Message(pTestPara, pTestResult, NOTENDOFTEST);	
				pTestPara->VdNow-=pTestPara->VgStep;						//设置新的栅极电压
				if(pTestPara->VdNow>pTestPara->VdEnd )
				{
					HAL_TIM_Base_Stop_IT(&htim3);																		//DAC输出数据中断关闭		
					HAL_TIM_Base_Stop_IT(&htim2);					                          //ADC采集电流中断关闭

					HAL_Delay(100);
					pTestPara->testStatus=OFF;
					USB_TX_Message(pTestPara, pTestResult, ENDOFTEST);							//结束测试,发送最后一组F3开头的命令
					ClearAllVoltage();																							//输出电压清零
				}
			}
		}
	}
}


