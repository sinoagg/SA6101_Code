#include "voltage_monitor.h"
#include "relay.h"

MonitorCH_TypeDef hMonitorCH_Drain;
MonitorCH_TypeDef hMonitorCH_Gate;
MonitorCH_TypeDef hMonitorCH_Source;
MonitorCH_TypeDef hMonitorCH_Sub;

static void MonitorCH_InitSingle(MonitorCH_TypeDef *pMonitorCH, AD7791_HandleTypeDef *pAD7791, uint8_t terminal)
{
	M_AD7791_Init(pAD7791, terminal);
	pMonitorCH->phAD7791x=pAD7791;
	pMonitorCH->index=terminal;
	pMonitorCH->relayState=0;
}
	
void MonitorCH_Init(void)
{
	MonitorCH_InitSingle(&hMonitorCH_Drain, &hAD7791_Drain, M_DRAIN);
	MonitorCH_InitSingle(&hMonitorCH_Gate, &hAD7791_Gate, M_GATE);
	//MonitorCH_InitSingle(&hMonitorCH_Source, &hAD7791_Source, M_SOURCE);
	MonitorCH_InitSingle(&hMonitorCH_Sub, &hAD7791_Sub, M_SUB);
//	M_AD7791_Init(&hAD7791_Drain, M_DRAIN);
//	M_AD7791_Init(&hAD7791_Gate, M_GATE);
//	M_AD7791_Init(&hAD7791_Source, M_SOURCE);
//	M_AD7791_Init(&hAD7791_Sub, M_SUB);
//	hMonitorCH_Drain.phAD7791x=&hAD7791_Drain; 
//	hMonitorCH_Drain.index=M_DRAIN;
//	hMonitorCH_Drain.relayState=0;
//	hMonitorCH_Gate.phAD7791x=&hAD7791_Gate; 
//	hMonitorCH_Gate.index=M_GATE;
//	hMonitorCH_Gate.relayState=0;
//	hMonitorCH_Source.phAD7791x=&hAD7791_Source; 
//	hMonitorCH_Source.index=M_SOURCE;
//	hMonitorCH_Source.relayState=0;
//	hMonitorCH_Sub.phAD7791x=&hAD7791_Sub; 
//	hMonitorCH_Sub.index=M_SUB;
//	hMonitorCH_Sub.relayState=0;
}
/*判断监控端档位是否使用正确，并回传正确数据*/
/*relayDelay_ms 在校准时为较大数值以使读数准确，测量时为较小数值以使读数迅速*/
float GetMonitorVoltage(MonitorCH_TypeDef *phMonitorCH, uint16_t relayDelay_ms)
{
	float readdata=0;															//校准时等待输出稳定	
	readdata=M_AD7791_ReadData(phMonitorCH->phAD7791x, phMonitorCH->index);
	if(phMonitorCH->relayState==0)								//如果继电器是开的状态
	{
		if((readdata<220)&&(readdata>-220))					//如果读到的值太小，则关闭继电器
		{
			SetMonitorRelay(phMonitorCH->index);							
			phMonitorCH->relayState=1;
			HAL_Delay(relayDelay_ms);
			readdata=M_AD7791_ReadData(phMonitorCH->phAD7791x, phMonitorCH->index);
		}
	}
	else                                        	//如果继电器是闭合状态
	{		
		if((readdata>2300)||(readdata<-2300))				//如果读到的值太大，则打开继电器			                                  
		{
			ReleaseMonitorRelay(phMonitorCH->index); 				
			phMonitorCH->relayState=0;
			HAL_Delay(relayDelay_ms);
			readdata=M_AD7791_ReadData(phMonitorCH->phAD7791x, phMonitorCH->index);	
		}
	}
	
	if(phMonitorCH->relayState==0)								//如果继电器断开，返回真实电压返回10.1倍
		readdata*=10.1;
	
	return readdata;
}
