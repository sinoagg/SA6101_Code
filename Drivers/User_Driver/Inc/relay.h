#ifndef __RELAY_H
#define __RELAY_H	 

#include "stm32f1xx_hal.h"	   
#include "mxconstants.h"	
#include "prj_typedef.h"
#include "usbd_custom_hid_if.h"
#include "hardware.h"

#define M_GATE  0
#define M_SOURCE 1
#define M_DRAIN 2
#define M_SUB 	3
#define M_ALL   4
	 
#define RANGE_CHANGE_DELAY 100								//delay time between each relay change in ms
#define RELAY_RANGE_AUTO 0                   //自动换挡模式
#define RELAY_RANGE_51R 1
#define RELAY_RANGE_100R 2
#define RELAY_RANGE_1K 3
#define RELAY_RANGE_10K 4
#define RELAY_RANGE_100K 5
#define RELAY_RANGE_1M 6
#define RELAY_RANGE_10M 7
#define RELAY_RANGE_100M 8
#define RELAY_RANGE_1G 9

typedef struct
{
	uint8_t rangeMode;								//自动或手动换挡：0为自动换挡，1-10为手动换挡的档位值
	uint8_t	maxRange;									//最大挡位
	uint8_t minRange;									//最小挡位
	uint8_t tempMaxRange;							//临时最大挡位，用于换挡判断使用
	uint8_t tempMinRange;							//临时最小挡位，用于换挡判断使用
	uint8_t rangeNow;									//目前所用挡位
	uint8_t drainConnect;
	uint8_t gateConnect;
	uint8_t subConnect;
	uint8_t rangeChangeTimes;
}RelayTypeDef;

extern RelayTypeDef Relay;

void ClearRangeRelay(void);
void SetRangeRelay(uint8_t range_select);       //设置源漏电流放大范围
void GetRelayPara(RelayTypeDef* pRelay);

void SetMonitorRelay(int8_t index);  						//设置监控端继电器
void ReleaseMonitorRelay(int8_t index); 				//释放监控端继电器

uint8_t RelayCheck(enum TestMode testMode, RelayTypeDef* pRelay, uint16_t originADC_Val);//检查是否需要调整继电器

void ConnectDrain(void);
void DisconnectDrain(void);
void ConnectGate(void);
void DisconnectGate(void);
void ConnectSub(void);
void DisconnectSub(void);
void ConnectAllOutput(void);
void DisconnectAllOutput(void);

#endif

