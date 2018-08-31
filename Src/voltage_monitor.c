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
/*�жϼ�ض˵�λ�Ƿ�ʹ����ȷ�����ش���ȷ����*/
/*relayDelay_ms ��У׼ʱΪ�ϴ���ֵ��ʹ����׼ȷ������ʱΪ��С��ֵ��ʹ����Ѹ��*/
float GetMonitorVoltage(MonitorCH_TypeDef *phMonitorCH, uint16_t relayDelay_ms)
{
	float readdata=0;															//У׼ʱ�ȴ�����ȶ�	
	readdata=M_AD7791_ReadData(phMonitorCH->phAD7791x, phMonitorCH->index);
	if(phMonitorCH->relayState==0)								//����̵����ǿ���״̬
	{
		if((readdata<220)&&(readdata>-220))					//���������ֵ̫С����رռ̵���
		{
			SetMonitorRelay(phMonitorCH->index);							
			phMonitorCH->relayState=1;
			HAL_Delay(relayDelay_ms);
			readdata=M_AD7791_ReadData(phMonitorCH->phAD7791x, phMonitorCH->index);
		}
	}
	else                                        	//����̵����Ǳպ�״̬
	{		
		if((readdata>2300)||(readdata<-2300))				//���������ֵ̫����򿪼̵���			                                  
		{
			ReleaseMonitorRelay(phMonitorCH->index); 				
			phMonitorCH->relayState=0;
			HAL_Delay(relayDelay_ms);
			readdata=M_AD7791_ReadData(phMonitorCH->phAD7791x, phMonitorCH->index);	
		}
	}
	
	if(phMonitorCH->relayState==0)								//����̵����Ͽ���������ʵ��ѹ����10.1��
		readdata*=10.1;
	
	return readdata;
}
