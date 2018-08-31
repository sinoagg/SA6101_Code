#ifndef __CALCULATION_H
#define __CALCULATION_H

#include "stm32f1xx_hal.h"	 
#include "process_ctrl.h"	 
#include "relay.h"
	 
uint8_t Do_Calculation(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult, RelayTypeDef* pRelay);
#endif
	 


