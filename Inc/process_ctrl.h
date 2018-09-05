#ifndef __PROCESS_CTRL_H
#define __PROCESS_CTRL_H

#include "stm32f1xx_hal.h"
#include "usbd_custom_hid_if.h"
#include "relay.h"
#include "hardware.h"
#include "AD5687R.h"
#include "prj_typedef.h"
#include "InternalFlash.h"

#define GATE 0
#define SOURCE 1
#define DRAIN 2
#define SUB 3

#define DEV_ADDR 0x01																										//此设备485总线地址，同总线产品地址不能相同
#define BROCST_ADDR 0x00
#define UART_RX_LEN 30																									//上位机配置数据长度
#define UART_TX_LEN 20																									//向上位机传输数据长度

#define TIM_ON 1
#define TIM_OFF 0

extern uint8_t silenttime_past;							//静默时间过去标志位，0表示未过

enum ADC_Status
{
	ADC_READY=0,
	ADC_BUSY=1
};

void InitTestPara(TestPara_TypeDef* pTestPara);
void InitTestResult(TestResult_TypeDef* pTestResult);

void GetTestPara(TestPara_TypeDef* pTestPara, RelayTypeDef* pRelay, uint8_t* pUartRxBuf);

void SetInitialVoltage(TestPara_TypeDef* pTestPara);
void OutputAdjVoltage(uint8_t port, int16_t outputValue);
void OutputVoltage(uint8_t port, int16_t outputValue);	//输出特定未校准电压mV
void ClearAllVoltage(void);

void SetTimerPara(TestPara_TypeDef* pTestPara);
void SetTimerAction(TestPara_TypeDef* pTestPara);
static void ChangeTimer(TestPara_TypeDef* pTestPara, uint8_t TIM_ONOFF);

uint8_t* prepareTxData(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult, uint8_t* pUartRxBuf);
uint8_t GetMsgType(uint8_t *UartRxBuf);
#endif 

