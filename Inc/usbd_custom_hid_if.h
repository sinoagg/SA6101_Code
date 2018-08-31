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

enum CaliMode										//У׼ģʽ
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
    enum TestMode testMode;			//��������ģʽ,     
		enum TestCMD testCmd;				//�ж�����
		enum CaliMode cali_mode;			//У׼ģʽ
    int16_t   wDrainVoltageStart; //©�˵�ѹfloat�ͷ�Χ-16.5~16.5
		int16_t		wDrainVoltageEnd;
    int16_t  	wGateVoltageStart;  //����ģʽ�µ�դ����ѹ����ɨ��ģʽ�µ���ʼդ����ѹ                           
    int16_t  	wGateVoltageEnd;		//ɨ��ģʽ�µĽ�����ѹ 
		int16_t		wSourceVoltageStart;//����ģʽ�µ�Դ����ѹ����ɨ��ģʽ�µ���ʼԴ����ѹ  
		int16_t		wSourceVoltageEnd;
		int16_t		wSubVoltageStart;		//����ģʽ�µ�Sub��ѹ����ɨ��ģʽ�µ���ʼSub��ѹ
		int16_t		wSubVoltageEnd;
    uint16_t  wVoltageStep;				//ɨ��ģʽ�µ�step��ѹ
		uint16_t  wTimeStep;					//ɨ��ģʽ�µ�����������ʱ����,ָDAC��������
		uint16_t	wSampleRate;				//ADC�Ĳ�������
		uint16_t	wAverageNumber;			//ADC�ɼ����ƽ������
		uint16_t	wSilentTime;				//��ʼ��Ĭʱ��
	  uint8_t   wRangeSelect;       //��λģʽ�͹̶���λ������ֵ
		enum USBMsgType eMsgType;			//������������
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
void InitUSBVar(void);													//��ʼ��USBͨ�Žṹ����
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
