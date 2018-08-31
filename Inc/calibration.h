#ifndef __CALIBRATION_H
#define __CALIBRATION_H

#include "stm32f1xx_hal.h"
#include "prj_typedef.h"
#include "voltage_monitor.h"
#include "relay.h"

#define OUTPUT_VOL_CALIBRATION 0
#define ZERO_CURRENT_CALIBRATION	1
#define CURRENT_RANGE_CALIBRATION 2
#define SAVE_CALIBRATION  0xff

#define VOL_0V  0               //输出0V电压
#define VOL_100mV 100           //输出0.1V电压
#define VOL_N100mV -100         //输出-0.1V电压
#define VOL_200mV 200           //输出0.2V电压
#define VOL_N200mV -200         //输出-0.2V电压
#define VOL_500mV 500           //输出0.5V电压
#define VOL_N500mV -500         //输出-0.5V电压
#define VOL_1400mV 1400         //输出1.4V电压
#define VOL_N1400mV -1400       //输出-1.4V电压
#define VOL_1500mV 1500         //输出1.5V电压
#define VOL_N1500mV -1500       //输出-1.5V电压
#define VOL_2V 2000             //输出2V电压
#define VOL_N2V -2000           //输出-2V电压
#define VOL_5V 5000             //输出5V电压
#define VOL_N5V -5000           //输出-5V电压
#define VOL_20V 20000           //输出20V电压

extern FloatUnion Adj_OutputOffset[4];	//gate, source, drain, sub, 输出偏移校正系数
extern FloatUnion Adj_OutputLinear[4];

extern FloatUnion Adj_RelayOffset[9];
extern FloatUnion Adj_PosRelayLinear[9];
extern FloatUnion Adj_NegRelayLinear[9];
extern FloatUnion Adj_MeasOffset[4];
extern FloatUnion Adj_MeasLinear[4];


float CommitAdjustment(float input_I, RelayTypeDef* pRelay);
float MyPow(float base,uint8_t time);
void LED_Indicator(void);                                                  										//指示灯
void ClearArray(FloatUnion* arrary, uint8_t size);               											//清空数组
void SetArray(FloatUnion* arrary, uint8_t size);
void Cali_SetRangeRelay(FloatUnion* arrary, uint8_t size, int16_t output_voltage);  		//校准输出电压
void Cali_OpenCrrent(FloatUnion* arrary, uint8_t size, int16_t output_voltage);     		//校准开路电流
float Cali_TestCrrent_Init(FloatUnion* arrary, uint8_t index, int16_t output_voltage); 	//校准测试电流
void Cali_LOAD100_Crrent_Pos(FloatUnion* arrary, uint8_t index);
void Cali_LOAD100_Crrent_Neg(FloatUnion* arrary, uint8_t index);
void Cali_LOAD10K_1M_Crrent_Pos(FloatUnion* arrary, uint8_t index);
void Cali_LOAD10K_1M_Crrent_Neg(FloatUnion* arrary, uint8_t index);
void Cali_LOAD100M_Crrent_Pos(FloatUnion* arrary, uint8_t index);
void Cali_LOAD100M_Crrent_Neg(FloatUnion* arrary, uint8_t index);
void FlashSaveCaliData(uint32_t start_address, uint32_t end_address);   											//保存校准数据
void Do_Calibration(TestPara_TypeDef* pTestPara);                          										//校准过程

void RunCalibration(uint8_t* UartRxBuf);
void Cali_Crrent_Pos(FloatUnion* arrary,float temp,uint8_t index);
void Cali_Crrent_Neg(FloatUnion* arrary,float temp,uint8_t index);

void RunCalibration(uint8_t* UartRxBuf);


#endif

