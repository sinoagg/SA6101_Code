#include "process_ctrl.h"
#include "calibration.h"

uint8_t silenttime_past=0;							//静默时间过去标志位，0表示未过

/***********************************************
*初始化测试参数
************************************************/
void InitTestPara(TestPara_TypeDef* pTestPara)
{
	pTestPara->testStatus=OFF;																														//目前的测试状态，清空，准备接收USB命令
	
	#ifdef MANUAL_ADJUSTMENT_1
		BOARD_INDEX=1;
		Adj_OutputOffset[GATE].numFloat=-9.7;                 //-1号机箱数据
		Adj_OutputOffset[SOURCE].numFloat=0;
		Adj_OutputOffset[DRAIN].numFloat=-7.4;       
		Adj_OutputOffset[SUB].numFloat=0;      
		Adj_OutputLinear[GATE].numFloat=1;
		Adj_OutputLinear[SOURCE].numFloat=1;
		Adj_OutputLinear[DRAIN].numFloat=1;
		Adj_OutputLinear[SUB].numFloat=1;
	
//		Adj_MeasOffset[GATE].numFloat=9.69;       
//		Adj_MeasOffset[SOURCE].numFloat=0;
//		Adj_MeasOffset[DRAIN].numFloat=4.2;     
//		Adj_MeasOffset[SUB].numFloat=0;
	
		Adj_RelayOffset[0].numFloat=-8.20172e-4;             
		Adj_RelayOffset[1].numFloat=-8.20172e-4;             
		Adj_RelayOffset[2].numFloat=-8.20172e-4;              
		Adj_RelayOffset[3].numFloat=-8.20172e-4;              
		Adj_RelayOffset[4].numFloat=-8.39e-4;              
		Adj_RelayOffset[5].numFloat=-8.39e-4;            
		Adj_RelayOffset[6].numFloat=-8.58e-4;              
		Adj_RelayOffset[7].numFloat=-8.4e-4;             
		Adj_RelayOffset[8].numFloat=-8.77e-4;              
		
		Adj_PosRelayLinear[0].numFloat=1;;
		Adj_PosRelayLinear[1].numFloat=0.9988;
		Adj_PosRelayLinear[2].numFloat=0.99933;
		Adj_PosRelayLinear[3].numFloat=0.99893;
		Adj_PosRelayLinear[4].numFloat=1;
		Adj_PosRelayLinear[5].numFloat=1.0143;//0.998;
		Adj_PosRelayLinear[6].numFloat=1.02264;          
		Adj_PosRelayLinear[7].numFloat=1.0101;          
		Adj_PosRelayLinear[8].numFloat=1.0563;    
		
		Adj_NegRelayLinear[0].numFloat=1.0042;
		Adj_NegRelayLinear[1].numFloat=1.002736;
		Adj_NegRelayLinear[2].numFloat=1.00341;
		Adj_NegRelayLinear[3].numFloat=1.00286;
		Adj_NegRelayLinear[4].numFloat=1.00393;
		Adj_NegRelayLinear[5].numFloat=1.0136;//1.0025;
		Adj_NegRelayLinear[6].numFloat=1.0262;        
		Adj_NegRelayLinear[7].numFloat=1.022;          
		Adj_NegRelayLinear[8].numFloat=1.1261;           
	#endif
	
	#ifdef NO_ADJUSTMENT
		ClearArray(Adj_OutputOffset, 4);
		SetArray(Adj_OutputLinear, 4);
		//ClearArray(Adj_MeasOffset, 4);
		//SetArray(Adj_MeasLinear, 4);
		ClearArray(Adj_RelayOffset, 9);
		SetArray(Adj_PosRelayLinear, 9);
		SetArray(Adj_NegRelayLinear, 9);
	#endif
	
	#ifdef AUTO_ADJUSTMENT
		//一次读出32bit
		GetFlashData_Float(Adj_OutputOffset, FLASH_USER_PAGE1_START_ADDR+OUTPUT_OFFSET_ADDR, 4);					//读取Output OFFSET，4个float型数 
		//GetFlashData_Float(Adj_OutputLinear, FLASH_USER_PAGE1_START_ADDR+OUTPUT_LINEAR_ADDR, 4);					//读取Output LINEAR，4个float型数
		GetFlashData_Float(Adj_RelayOffset, FLASH_USER_PAGE1_START_ADDR+OUTPUT_RELAY_OFFSET_ADDR, 9);			//读取RELAY OFFSET，9个float型数
		GetFlashData_Float(Adj_PosRelayLinear, FLASH_USER_PAGE1_START_ADDR+OUTPUT_RELAY_LINEAR_ADDR, 9);	//读取POSITIVE RELAY LINEAR，9个float型数 
		GetFlashData_Float(Adj_NegRelayLinear, FLASH_USER_PAGE1_START_ADDR+OUTPUT_NRELAY_LINEAR_ADDR, 9);	//读取NEGATIVE RELAY LINEAR，9个float型数  
	#endif
	
}


void InitTestResult(TestResult_TypeDef* pTestResult)
{
	pTestResult->test_ADC_DrainVol.numFloat=0;
	pTestResult->test_ADC_GateVol.numFloat=0;
	pTestResult->test_ADC_SourceVol.numFloat=0;
	pTestResult->I_avg.numFloat=0;
	pTestResult->I_sample=0;
	pTestResult->I_sum=0;
	pTestResult->sampleCount=0;
}
/***********************************************
*获取485传输的测试参数
************************************************/
void GetTestPara(TestPara_TypeDef* pTestPara, RelayTypeDef* pRelay, uint8_t* pUartRxBuf)
{
		if(pTestPara->testStatus==OFF)																	//如果设备没有测试进行中
		{
			pTestPara->testMode=(enum TestMode)*(pUartRxBuf+2);
			pTestPara->timeStep=((uint16_t)*(pUartRxBuf+18))<<8|*(pUartRxBuf+19);
			pTestPara->sampleRate=((uint16_t)*(pUartRxBuf+22))<<8|*(pUartRxBuf+23);
			pTestPara->avgNumber=((uint16_t)*(pUartRxBuf+24))<<8|*(pUartRxBuf+25);
			pTestPara->quietTime=((uint16_t)*(pUartRxBuf+16))<<8|*(pUartRxBuf+17);
			pTestPara->quietTime/=100;				//MCU端以100ms分辨率计数
			pTestPara->rangeSelect=*(pUartRxBuf+26);					
			pTestPara->VgStart=((uint16_t)*(pUartRxBuf+10))<<8|*(pUartRxBuf+11);
			pTestPara->VgEnd=((uint16_t)*(pUartRxBuf+12))<<8|*(pUartRxBuf+13);
			pTestPara->VdStart=((uint16_t)*(pUartRxBuf+4))<<8|*(pUartRxBuf+5);
			pTestPara->VdEnd=((uint16_t)*(pUartRxBuf+6))<<8|*(pUartRxBuf+7);
			
			pTestPara->VdStep=((uint16_t)*(pUartRxBuf+8))<<8|*(pUartRxBuf+9);
			pTestPara->VgStep=((uint16_t)*(pUartRxBuf+14))<<8|*(pUartRxBuf+15);
			pTestPara->VdNow=pTestPara->VdStart;
			pTestPara->VgNow=pTestPara->VgStart;
			pTestPara->startDelay=pTestPara->timeStep - 1000*pTestPara->avgNumber/pTestPara->sampleRate-10;			//给计算传输留出10ms空闲
			if(pTestPara->startDelay<0) pTestPara->startDelay=0;
			
			pRelay->rangeMode=*(pUartRxBuf+26);
			pRelay->maxRange=*(pUartRxBuf+27);
			pRelay->minRange=*(pUartRxBuf+28);
			pRelay->drainConnect=*(pUartRxBuf+3)&0x08;
			pRelay->gateConnect=*(pUartRxBuf+3)&0x04;
			pRelay->subConnect=*(pUartRxBuf+3)&0x02;
		}	
}

void SetInitialVoltage(TestPara_TypeDef* pTestPara)
{
	pTestPara->VgNow=pTestPara->VgStart;			//栅极电压当前的值
	pTestPara->VdNow=pTestPara->VdStart;			//漏极电压当前的值
	//设置好其他电压
	AD5687R_WriteIR(&hAD5687R1, GATE,((float)(pTestPara->VgStart/Adj_OutputLinear[GATE].numFloat+Adj_OutputOffset[GATE].numFloat/10)/5000*65536));			//gate voltage
	AD5687R_WriteIR(&hAD5687R1, SOURCE,((float)(pTestPara->VsStart/Adj_OutputLinear[SOURCE].numFloat+Adj_OutputOffset[SOURCE].numFloat/10)/5000*65536));		//source voltage
	AD5687R_WriteIR(&hAD5687R2, DRAIN,((float)(pTestPara->VdStart/Adj_OutputLinear[DRAIN].numFloat+Adj_OutputOffset[DRAIN].numFloat/10)/5000*65536));    	//drain voltage
	AD5687R_WriteIR(&hAD5687R2, SUB,((float)(pTestPara->VbStart/Adj_OutputLinear[GATE].numFloat+Adj_OutputOffset[SUB].numFloat/10)/5000*65536));    		//sub voltage
}

void ClearAllVoltage(void)				//输出电压清零
{
	OutputAdjVoltage(DRAIN, 0);
	OutputAdjVoltage(GATE, 0);
	OutputAdjVoltage(SUB, 0);
}

void OutputAdjVoltage(uint8_t port, int16_t outputValue)	//输出特定电压mV
{
	switch(port)
	{
		case GATE:
			AD5687R_WriteIR(&hAD5687R1, CH_A,((float)(-(outputValue-25000-Adj_OutputOffset[GATE].numFloat))/10)/5000*65536);			  //gate voltage
			break;
		case DRAIN:
			AD5687R_WriteIR(&hAD5687R2, CH_A,((float)(-(outputValue-25000-Adj_OutputOffset[DRAIN].numFloat))/10)/5000*65536);    	//drain voltage
			break;
		case SUB:
			AD5687R_WriteIR(&hAD5687R2, CH_B,((float)(-(outputValue-25000-Adj_OutputOffset[SUB].numFloat))/10)/5000*65536);  
			break;
		default:
			break;
	}
}

void OutputVoltage(uint8_t port, int16_t outputValue)	//输出特定电压mV
{
	switch(port)
	{
		case GATE:
			AD5687R_WriteIR(&hAD5687R1, CH_A,((float)(-(outputValue-25000))/10)/5000*65536);			  //gate voltage
			break;
		case DRAIN:
			AD5687R_WriteIR(&hAD5687R2, CH_A,((float)(-(outputValue-25000))/10)/5000*65536);    	//drain voltage
			break;
		case SUB:
			AD5687R_WriteIR(&hAD5687R2, CH_B,((float)(-(outputValue-25000))/10)/5000*65536);  
			break;
		default:
			break;
	}
}

void SetTimerPara(TestPara_TypeDef* pTestPara)
{
		MX_TIM3_Init(pTestPara->timeStep);									//DAC输出定时器,单位ms
		HAL_TIM_Base_Stop_IT(&htim3);    										//关闭DAC输出中断
		MX_TIM2_Init(10000/(pTestPara->sampleRate));			//ADC采集定时器，单位100us
		HAL_TIM_Base_Stop_IT(&htim2);     									//关闭ADC采集中断
		MX_TIM4_Init();																		//固定100ms中断一次
		HAL_TIM_Base_Stop_IT(&htim4);
}

void SetTimerAction(TestPara_TypeDef* pTestPara)
{
	if(pTestPara->testCmd==CMD_START)										//开始采集的命令				
	{		
		pTestPara->testStatus=ON;													//测试开始,有测试在运行中
		ChangeTimer(pTestPara, TIM_ON);										//打开相应定时器
	}
	else if(pTestPara->testCmd==CMD_PAUSE)								//暂停采集的命令，只适用于模式1
	{
		pTestPara->testStatus=PAUSED;
		ChangeTimer(pTestPara, TIM_OFF);
	}
	else if(pTestPara->testCmd==CMD_CONTINUE)						//继续采集的命令，只适用于模式1
	{
		if(pTestPara->testStatus==PAUSED)									//如果此时设备有暂停中的测试，才可以使用继续的命令,其他状态下该命令无效
		{
			pTestPara->testStatus=ON;
			ChangeTimer(pTestPara, TIM_ON);										//打开相应定时器
		}
	}
	else if(pTestPara->testCmd==CMD_STOP)								//停止采集的命令
	{
		pTestPara->testStatus=OFF;					
		ChangeTimer(pTestPara, TIM_OFF);		
	}
}
static void ChangeTimer(TestPara_TypeDef* pTestPara, uint8_t TIM_ONOFF)
{
	if(TIM_ONOFF == TIM_ON)
	{
		//HAL_TIM_Base_Start_IT(&htim3);								//在timer4定时器中断里启动Tim3	
		HAL_TIM_Base_Start_IT(&htim2);								//ADC采集数据并回传中断启动
		HAL_TIM_Base_Start_IT(&htim4);								//启动QuietTime定时器
	}
	else
	{
		HAL_TIM_Base_Stop_IT(&htim3);									//采样中断停止
		HAL_TIM_Base_Stop_IT(&htim2);									//ADC采集数据并回传中断停止
		HAL_TIM_Base_Stop_IT(&htim4);
	}
}

void prepareTxData(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult, uint8_t* pUartRxBuf)
{
	uint8_t xorCheck=0;
	*pUartRxBuf=DEV_ADDR;
	*(pUartRxBuf+1)=pTestResult->endOfTest;
	*(pUartRxBuf+2)=(uint8_t)(pTestPara->VdNow>>8);
	*(pUartRxBuf+3)=(uint8_t)(pTestPara->VdNow&0xFF);
	*(pUartRxBuf+4)=(uint8_t)(pTestPara->VgNow>>8);
	*(pUartRxBuf+5)=(uint8_t)(pTestPara->VgNow&0xFF);
	*(pUartRxBuf+6)=pTestResult->I_avg.numUchar[3];																		
	*(pUartRxBuf+7)=pTestResult->I_avg.numUchar[2];																		
	*(pUartRxBuf+8)=pTestResult->I_avg.numUchar[1];																	
	*(pUartRxBuf+9)=pTestResult->I_avg.numUchar[0];																	
	*(pUartRxBuf+18)=Relay.rangeNow;
	for(uint8_t j=0;j<UART_TX_LEN-1;j++)                //将接收到的数据按照协议进行校验
	{
		xorCheck^=*(pUartRxBuf+j);
	}
	*(pUartRxBuf+19)=xorCheck;
}





