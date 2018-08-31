#ifndef __VOLTAGE_MONITOR_H
#define __VOLTAGE_MONITOR_H

#include "stm32f1xx_hal.h"
#include "AD7791.h"


typedef struct
{
	uint8_t index;
	uint8_t relayState;
	AD7791_HandleTypeDef *phAD7791x;
}MonitorCH_TypeDef;

extern MonitorCH_TypeDef hMonitorCH_Drain;
extern MonitorCH_TypeDef hMonitorCH_Gate;
extern MonitorCH_TypeDef hMonitorCH_Source;
extern MonitorCH_TypeDef hMonitorCH_Sub;

void MonitorCH_Init(void);
float GetMonitorVoltage(MonitorCH_TypeDef *pMonitorCH, uint16_t relayDelay_ms);
	
#endif
