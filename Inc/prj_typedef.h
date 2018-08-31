#ifndef __PRJ_TYPEDEF_H
#define __PRJ_TYPEDEF_H

#include "stm32f1xx_hal.h"
#include "usbd_custom_hid_if.h"

enum MsgType
{
	MSG_TYPE_NULL=0,
	MSG_TYPE_SETTING=0x11,					
	MSG_TYPE_RUN=0x12,
	MSG_TYPE_STOP=0x13,
	MSG_TYPE_QUERY=0x14,
	MSG_TYPE_CALIBRATION=0x15,
	MSG_TYPE_REPORT_ID=0xFF
};

enum TestStatus
{
	ON=0,
	OFF=1,
	PAUSED=2
};

typedef union										//����ֵ������֯��ʽ
{
	uint8_t numUchar[4]; 
	float numFloat;
}FloatUnion;

typedef struct
{
	enum TestStatus testStatus;						//����״̬
	enum TestMode testMode;
	enum TestCMD	testCmd;
	enum CaliMode caliMode;
	uint16_t timeStep;										//timestep decides the time delay between each output point and tansmission, default is 1000ms
	uint16_t sampleRate;									//samplerate decides main current monitor ADC sampling speed, default is 1000  		
	int16_t VgStart;											//voltage gate start
	int16_t VgEnd;												//voltage gate end
	int16_t VgNow;												//the current voltage of gate output
	int16_t VdStart;											//voltage drain start
	int16_t VdEnd;												//voltage drain end
	int16_t VdNow;												//the current voltage of drain output
	int16_t VsStart;
	int16_t VsEnd;
	int16_t VsNow;
	int16_t VbStart;
	int16_t VbEnd;
	int16_t VbNow;
	int16_t VgStep;												//voltaege step for sweeping use
	int16_t VdStep;			
	uint16_t avgNumber;										//Ĭ��16����ȡƽ��ֵ
	uint16_t quietTime;										//Ĭ�Ͼ�Ĭʱ����2s
	int16_t startDelay;
	uint8_t rangeSelect;             			//��λģʽ�͹̶���λģʽ�µĵ�λ����
	uint8_t maxRange;          						//�Զ��������λѡ��
	uint8_t minRange;
}TestPara_TypeDef;

typedef struct
{
	uint16_t I_sample;					//���β���ֵ
	uint32_t I_sum;
	FloatUnion I_avg;							//definition of current value
	uint32_t sampleCount;				//��������
	FloatUnion test_ADC_GateVol;		//ADC �ɼ�����դ����ѹ				
	FloatUnion test_ADC_DrainVol;		//ADC �ɼ�����©����ѹ	
	FloatUnion test_ADC_SourceVol;	//ADC �ɼ�����Դ����ѹ	
	uint8_t endOfTest;
	uint8_t currentResultValid;			//�Ѿ��õ�Ŀǰ�Ĳ������ֵ
}TestResult_TypeDef;

#endif

