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

typedef union										//电流值数据组织形式
{
	uint8_t numUchar[4]; 
	float numFloat;
}FloatUnion;

typedef struct
{
	enum TestStatus testStatus;						//测试状态
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
	uint16_t avgNumber;										//默认16个点取平均值
	uint16_t quietTime;										//默认静默时间是2s
	int16_t startDelay;
	uint8_t rangeSelect;             			//档位模式和固定档位模式下的档位量程
	uint8_t maxRange;          						//自动换挡最大档位选择
	uint8_t minRange;
}TestPara_TypeDef;

typedef struct
{
	uint16_t I_sample;					//单次测量值
	uint32_t I_sum;
	FloatUnion I_avg;							//definition of current value
	uint32_t sampleCount;				//采样个数
	FloatUnion test_ADC_GateVol;		//ADC 采集到的栅极电压				
	FloatUnion test_ADC_DrainVol;		//ADC 采集到的漏极电压	
	FloatUnion test_ADC_SourceVol;	//ADC 采集到的源极电压	
	uint8_t endOfTest;
	uint8_t currentResultValid;			//已经得到目前的测量点的值
}TestResult_TypeDef;

#endif

