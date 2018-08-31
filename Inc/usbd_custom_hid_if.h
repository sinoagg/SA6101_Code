/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if_if.h
  * @brief          : header file for the usbd_custom_hid_if.c file
  ******************************************************************************
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __USBD_CUSTOM_HID_IF_H_
#define __USBD_CUSTOM_HID_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid.h"
/* USER CODE BEGIN INCLUDE */
/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_CUSTOM_HID
  * @brief header 
  * @{
  */ 

/** @defgroup USBD_CUSTOM_HID_Exported_Defines
  * @{
  */ 
/* USER CODE BEGIN EXPORTED_DEFINES */
enum TestMode
{
	SWEEP_DRAIN_VOL=1,
	SWEEP_GATE_VOL=2,
	SWEEP_IV=3,
	NO_SWEEP_IT=4,
	NO_SWEEP_RT=5,
	ID_T=6
};

enum TestCMD
{
	CMD_START=0x11,
	CMD_PAUSE=0x12,
	CMD_STOP=0x13,
	CMD_CONTINUE=0x14
};

enum USBMsgType
{
	USB_MSG_TYPE_NULL=0,
	USB_MSG_TYPE_SETTING=1,					
	USB_MSG_TYPE_COMMAND=2,
	USB_MSG_TYPE_CALIBRATION=3
};

enum CaliMode										//校准模式
{
	SHORT=0,
	LOAD100=1,
	LOAD10k=2,
	LOAD1M=3,
	LOAD100M=4,
	OUTPUTVOLTAGE_ZERO=0x80,
	OUTPUTVOLTAGE_20V=0x81,
	OPEN=0xFF
};

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Exported_Types
  * @{
  */  
/* USER CODE BEGIN EXPORTED_TYPES */
typedef  struct  usb_data
{
    enum TestMode testMode;			//设置命令模式,     
		enum TestCMD testCmd;				//行动命令
		enum CaliMode cali_mode;			//校准模式
    int16_t   wDrainVoltageStart; //漏端电压float型范围-16.5~16.5
		int16_t		wDrainVoltageEnd;
    int16_t  	wGateVoltageStart;  //单点模式下的栅极电压，或扫描模式下的起始栅极电压                           
    int16_t  	wGateVoltageEnd;		//扫描模式下的结束电压 
		int16_t		wSourceVoltageStart;//单点模式下的源级电压，或扫描模式下的起始源级电压  
		int16_t		wSourceVoltageEnd;
		int16_t		wSubVoltageStart;		//单点模式下的Sub电压，或扫描模式下的起始Sub电压
		int16_t		wSubVoltageEnd;
    uint16_t  wVoltageStep;				//扫描模式下的step电压
		uint16_t  wTimeStep;					//扫描模式下的两个测量点时间间隔,指DAC的输出间隔
		uint16_t	wSampleRate;				//ADC的采样速率
		uint16_t	wAverageNumber;			//ADC采集结果平均个数
		uint16_t	wSilentTime;				//初始静默时间
	  uint8_t   wRangeSelect;       //档位模式和固定档位量程数值
		enum USBMsgType eMsgType;			//接收命令属性
}USB_DataTypedef;
/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Exported_Macros
  * @{
  */ 
/* USER CODE BEGIN EXPORTED_MACRO */
/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Exported_Variables
  * @{
  */ 
  extern USBD_CUSTOM_HID_ItfTypeDef  USBD_CustomHID_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
	extern USB_DataTypedef USB_InData;
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Exported_FunctionsPrototype
  * @{
  */ 

/* USER CODE BEGIN EXPORTED_FUNCTIONS */
static uint16_t GetTwoByteValue(uint8_t* pData);
void InitUSBVar(void);													//初始化USB通信结构参数
/* USER CODE END EXPORTED_FUNCTIONS */
/**
  * @}
  */
   
/**
  * @}
  */ 

/**
* @}
*/
 
#ifdef __cplusplus
}
#endif

#endif /* __USBD_CUSTOM_HID_IF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
