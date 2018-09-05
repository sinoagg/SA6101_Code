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
#define RELAY_RANGE_AUTO 0                   //�Զ�����ģʽ
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
	uint8_t rangeMode;								//�Զ����ֶ�������0Ϊ�Զ�������1-10Ϊ�ֶ������ĵ�λֵ
	uint8_t	maxRange;									//���λ
	uint8_t minRange;									//��С��λ
	uint8_t tempMaxRange;							//��ʱ���λ�����ڻ����ж�ʹ��
	uint8_t tempMinRange;							//��ʱ��С��λ�����ڻ����ж�ʹ��
	uint8_t rangeNow;									//Ŀǰ���õ�λ
	uint8_t drainConnect;
	uint8_t gateConnect;
	uint8_t subConnect;
	uint8_t rangeChangeTimes;
}RelayTypeDef;

extern RelayTypeDef Relay;

void ClearRangeRelay(void);
void SetRangeRelay(uint8_t range_select);       //����Դ©�����Ŵ�Χ
void GetRelayPara(RelayTypeDef* pRelay);

void SetMonitorRelay(int8_t index);  						//���ü�ض˼̵���
void ReleaseMonitorRelay(int8_t index); 				//�ͷż�ض˼̵���

uint8_t RelayCheck(enum TestMode testMode, RelayTypeDef* pRelay, uint16_t originADC_Val);//����Ƿ���Ҫ�����̵���

void ConnectDrain(void);
void DisconnectDrain(void);
void ConnectGate(void);
void DisconnectGate(void);
void ConnectSub(void);
void DisconnectSub(void);
void ConnectAllOutput(void);
void DisconnectAllOutput(void);

#endif

