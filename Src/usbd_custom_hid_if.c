/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @brief          : USB Device Custom HID interface file.
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"
/* USER CODE BEGIN INCLUDE */
/* USER CODE END INCLUDE */
/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_CUSTOM_HID 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_TYPES */
/* USER CODE END PRIVATE_TYPES */ 
/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Private_Defines
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */
  
/**
  * @}
  */ 

/** @defgroup USBD_CUSTOM_HID_Private_Macros
  * @{
  */ 
/* USER CODE BEGIN PRIVATE_MACRO */
/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */ 

/** @defgroup USBD_AUDIO_IF_Private_Variables
 * @{
 */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */ 
  0x05, 0x8c,            /* USAGE_PAGE (Generic Desktop)     	*/                   
	0x09, 0x01,            /* USAGE 0(Undefined function, HID) 	*/    
  0xa1, 0x01,            /* COLLECTION (Application)       		*/            
  /* 6 */
	// The Input report

	0x09,0x03, // USAGE ID - Vendor defined
	0x15,0x00, // LOGICAL_MINIMUM (0)
	0x26,0x00, 0xFF, // LOGICAL_MAXIMUM (255)
	0x75,0x08, // REPORT_SIZE (8)	
	0x95,0x1F, // REPORT_COUNT (31)				如果32的话，上位机软件读写会有问题
	0x81,0x02, // INPUT (Data,Var,Abs)
	
	// The Output report

	0x09,0x04, // USAGE ID - Vendor defined
	0x15,0x00, // LOGICAL_MINIMUM (0)
	0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255)
	0x75,0x08, // REPORT_SIZE (8)
	0x95,0x1F, // REPORT_COUNT (31)
	0x91,0x02, // OUTPUT (Data,Var,Abs)
	//32
  /* USER CODE END 0 */ 
  0xC0    /*     END_COLLECTION	      */
   
}; 

/* USB handler declaration */
/* Handle for USB Full Speed IP */
  USBD_HandleTypeDef  *hUsbDevice_0;

/* USER CODE BEGIN PRIVATE_VARIABLES */
/* USER CODE END PRIVATE_VARIABLES */
/**
  * @}
  */ 
  
/** @defgroup USBD_CUSTOM_HID_IF_Exported_Variables
  * @{
  */ 
  extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE BEGIN EXPORTED_VARIABLES */
	USB_DataTypedef USB_InData;
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */ 
  
/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @{
  */
static int8_t CUSTOM_HID_Init_FS     (void);
static int8_t CUSTOM_HID_DeInit_FS   (void);
static int8_t CUSTOM_HID_OutEvent_FS (uint8_t event_idx, uint8_t state);
 

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS = 
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS,
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  CUSTOM_HID_Init_FS
  *         Initializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{ 
  /* USER CODE BEGIN 4 */ 
	hUsbDevice_0 = &hUsbDeviceFS;
  return (0);
  /* USER CODE END 4 */ 
}

/**
  * @brief  CUSTOM_HID_DeInit_FS
  *         DeInitializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */ 
  return (0);
  /* USER CODE END 5 */ 
}

/**
  * @brief  CUSTOM_HID_OutEvent_FS
  *         Manage the CUSTOM HID class events       
  * @param  event_idx: event index
  * @param  state: event state
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS  (uint8_t event_idx, uint8_t state)		//第一字节，第二字节
{ 
  /* USER CODE BEGIN 6 */ 
	USBD_CUSTOM_HID_HandleTypeDef     *hhid = (USBD_CUSTOM_HID_HandleTypeDef*)hUsbDeviceFS.pClassData;
		
	if(event_idx<0x10)								//如果USB数据为设置命令
	{
		USB_InData.testMode=(enum TestMode)event_idx;
		
		USB_InData.wDrainVoltageStart=GetTwoByteValue(&(hhid->Report_buf[2]));

		USB_InData.wDrainVoltageEnd=GetTwoByteValue(&(hhid->Report_buf[4]));
		
		USB_InData.wGateVoltageStart=GetTwoByteValue(&(hhid->Report_buf[6]));
		
		USB_InData.wGateVoltageEnd=GetTwoByteValue(&(hhid->Report_buf[8]));

		USB_InData.wSourceVoltageStart=GetTwoByteValue(&(hhid->Report_buf[10]));
		
		USB_InData.wSourceVoltageEnd=GetTwoByteValue(&(hhid->Report_buf[12]));

		USB_InData.wVoltageStep=GetTwoByteValue(&(hhid->Report_buf[14]));

		USB_InData.wTimeStep=GetTwoByteValue(&(hhid->Report_buf[16]));
		
		USB_InData.wSubVoltageStart=GetTwoByteValue(&(hhid->Report_buf[18]));
		
		USB_InData.wSubVoltageEnd=GetTwoByteValue(&(hhid->Report_buf[20]));
		
		USB_InData.wSampleRate=GetTwoByteValue(&(hhid->Report_buf[22]));
		
		USB_InData.wAverageNumber=GetTwoByteValue(&(hhid->Report_buf[24]));
		
		USB_InData.wSilentTime=GetTwoByteValue(&(hhid->Report_buf[26]));
		
		USB_InData.wRangeSelect=hhid->Report_buf[28];
		
		USB_InData.eMsgType=USB_MSG_TYPE_SETTING;									//USB有收到命令，置位设置标志位			
	}
	else if((event_idx>0x10) && (event_idx<0x20))																	//USB命令为行动命令
	{
		//HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);			//Communication LED ON	
		USB_InData.testCmd=(enum TestCMD)event_idx;
		USB_InData.eMsgType=USB_MSG_TYPE_COMMAND;									//USB有收到命令，置位行动标志位
	}
	else 
	{
		USB_InData.cali_mode=(enum CaliMode)state;
		USB_InData.eMsgType=USB_MSG_TYPE_CALIBRATION;									//USB有收到命令，置位行动标志位
	}
	
  return (0);
  /* USER CODE END 6 */ 
}

void InitUSBVar(void)													//初始化USB通信结构参数
{
	USB_InData.eMsgType=USB_MSG_TYPE_NULL;
}
	
/* USER CODE BEGIN 7 */ 
static uint16_t GetTwoByteValue(uint8_t* pData)
{
	uint16_t tempw16;
	tempw16=*pData;
	tempw16<<=8;
	tempw16|=*(pData+1);
	return tempw16;
}

/**
  * @brief  USBD_CUSTOM_HID_SendReport_FS
  *         Send the report to the Host       
  * @param  report: the report to be sent
  * @param  len: the report length
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
/*  
static int8_t USBD_CUSTOM_HID_SendReport_FS ( uint8_t *report,uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len); 
}
*/
/* USER CODE END 7 */ 

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */ 

/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
