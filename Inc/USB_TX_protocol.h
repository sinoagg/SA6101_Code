#ifndef __USB_TX_PROTOCOL_H
#define __USB_TX_PROTOCOL_H

#include "stm32f1xx_hal.h"
#include "prj_typedef.h"
#include "process_ctrl.h"
#include "hardware.h"

#define USB_TX_LEN 31											//it has to be 31, if the number is 32, then the PC cannot receive anything
#define NOTENDOFTEST	0xF2
#define ENDOFTEST 0xF3

extern uint8_t UsartTest_SendBuf[4];	
extern uint8_t HID_SendBuf[USB_TX_LEN];
void USB_TX_Message(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult, uint8_t EndOfTest);
void USB_Respond(void);
void USB_Update(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult);

#endif


