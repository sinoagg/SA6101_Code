#ifndef __HARDWARE_H
#define __HARDWARE_H

#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "usbd_custom_hid_if.h"
#include "delay.h"
#include "AD5687R.h"
#include "AD7988_1.h"
#include "AD7791.h"
#include "voltage_monitor.h"

#define DEFAULT_RANGE 4

extern TIM_HandleTypeDef htim2;				//handle of timer 2
extern TIM_HandleTypeDef htim3;				//handle of timer 3
extern TIM_HandleTypeDef htim4;				//handle of timer 4

extern UART_HandleTypeDef huart1;			//handle of uart1
extern UART_HandleTypeDef huart2;     //handle of uart2
extern UART_HandleTypeDef huart3;			//handle of uart3

extern AD5687R_HandleTypeDef hAD5687R1;	//handle of DAC 1
extern AD5687R_HandleTypeDef hAD5687R2;	//handle of DAC 2

extern AD7988_1_HandleTypeDef hAD7988_11;		//handle of main current monitor ADC 1
extern AD7791_HandleTypeDef hAD77911;				//handle of output voltage monitors ADC mutiples

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

void Hardware_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);
void MX_GPIO_Init(void);
void MX_TIM2_Init(uint16_t time_step);
void MX_TIM3_Init(uint16_t time_step);
void MX_TIM4_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

static void MX_DMA_Init(void);

#endif
