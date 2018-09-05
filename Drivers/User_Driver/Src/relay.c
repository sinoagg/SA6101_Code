#include "relay.h"

RelayTypeDef Relay;

void SetMonitorRelay(int8_t index)  //设置监控端继电器
{
	switch(index)
	{
		case 0:                                                             
			HAL_GPIO_WritePin(CTRL14_GPIO_Port, CTRL14_Pin , GPIO_PIN_SET);		
			break;
		case 1:                                                             
			HAL_GPIO_WritePin(CTRL15_GPIO_Port, CTRL15_Pin , GPIO_PIN_SET);
			break;
		case 2:                                                             
			HAL_GPIO_WritePin(CTRL16_GPIO_Port, CTRL16_Pin , GPIO_PIN_SET); 
			break;
		case 3:                                                             
			HAL_GPIO_WritePin(CTRL17_GPIO_Port, CTRL17_Pin , GPIO_PIN_SET); 
			break;
		case 4: 
			HAL_GPIO_WritePin(CTRL14_GPIO_Port, CTRL14_Pin , GPIO_PIN_SET);
			HAL_GPIO_WritePin(CTRL15_GPIO_Port, CTRL15_Pin , GPIO_PIN_SET);
			HAL_GPIO_WritePin(CTRL16_GPIO_Port, CTRL16_Pin , GPIO_PIN_SET); 
			HAL_GPIO_WritePin(CTRL17_GPIO_Port, CTRL17_Pin , GPIO_PIN_SET);
			break;
		default:
			break;
	}
	
}

void ReleaseMonitorRelay(int8_t index)       //释放监控端继电器
{
	switch(index)
	{
		case 0:                                                             
			HAL_GPIO_WritePin(CTRL14_GPIO_Port, CTRL14_Pin , GPIO_PIN_RESET);		
			break;
		case 1:                                                             
			HAL_GPIO_WritePin(CTRL15_GPIO_Port, CTRL15_Pin , GPIO_PIN_RESET);
			break;
		case 2:                                                             
			HAL_GPIO_WritePin(CTRL16_GPIO_Port, CTRL16_Pin , GPIO_PIN_RESET); 
			break;
		case 3:                                                             
			HAL_GPIO_WritePin(CTRL17_GPIO_Port, CTRL17_Pin , GPIO_PIN_RESET); 
			break;
		case 4: 
			HAL_GPIO_WritePin(CTRL14_GPIO_Port, CTRL14_Pin , GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(CTRL15_GPIO_Port, CTRL15_Pin , GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(CTRL16_GPIO_Port, CTRL16_Pin , GPIO_PIN_RESET); 
		  HAL_GPIO_WritePin(CTRL17_GPIO_Port, CTRL17_Pin , GPIO_PIN_RESET); 
			break;
		default:
			break;
	}	
}

void GetRelayPara(RelayTypeDef* pRelay)
{
	if(pRelay->rangeMode==RELAY_RANGE_AUTO)
		pRelay->rangeNow=RELAY_RANGE_1G;
	else 
		pRelay->rangeNow=pRelay->rangeMode;
	pRelay->tempMaxRange=pRelay->maxRange;
	pRelay->tempMinRange=pRelay->minRange;
	SetRangeRelay(pRelay->rangeNow);
  if(pRelay->drainConnect) ConnectDrain();
	else DisconnectDrain();
	if(pRelay->gateConnect) ConnectGate();
	else DisconnectGate();
	if(pRelay->subConnect) ConnectSub();
	else DisconnectSub();
}
void ClearRangeRelay(void)
{
	HAL_GPIO_WritePin(CTRL1_GPIO_Port, CTRL1_Pin , GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(CTRL2_GPIO_Port, CTRL2_Pin , GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(CTRL3_GPIO_Port, CTRL3_Pin , GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(CTRL4_GPIO_Port, CTRL4_Pin , GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(CTRL9_GPIO_Port, CTRL9_Pin , GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(CTRL10_GPIO_Port, CTRL10_Pin , GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(CTRL11_GPIO_Port, CTRL11_Pin , GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(CTRL12_GPIO_Port, CTRL12_Pin , GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(CTRL13_GPIO_Port, CTRL13_Pin , GPIO_PIN_RESET);			
}
void SetRangeRelay(uint8_t range_select)       //设置源漏电流放大范围
{
	ClearRangeRelay();
	switch(range_select)
	{
		case RELAY_RANGE_AUTO:																															//range=0时候为自动换挡，设置至最小档位开始
			HAL_GPIO_WritePin(CTRL13_GPIO_Port, CTRL13_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_51R:																															//开启继电器1，放大51倍
			HAL_GPIO_WritePin(CTRL13_GPIO_Port, CTRL13_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_100R:                                                            //开启继电器2，放大10^2倍
			HAL_GPIO_WritePin(CTRL1_GPIO_Port, CTRL1_Pin , GPIO_PIN_SET);		
			break;
		case RELAY_RANGE_1K:                                                             	//开启继电器3，放大10^3倍
			HAL_GPIO_WritePin(CTRL2_GPIO_Port, CTRL2_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_10K:                                                            	//开启继电器4，放大10^4倍
			HAL_GPIO_WritePin(CTRL3_GPIO_Port, CTRL3_Pin , GPIO_PIN_SET); 
			break;
		case RELAY_RANGE_100K:                                                            //开启继电器5，放大10^5倍
			HAL_GPIO_WritePin(CTRL4_GPIO_Port, CTRL4_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_1M:                                                             	//开启继电器6，放大10^6倍
			HAL_GPIO_WritePin(CTRL9_GPIO_Port, CTRL9_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_10M:                                                             //开启继电器7，放大10^7倍
			HAL_GPIO_WritePin(CTRL10_GPIO_Port, CTRL10_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_100M:                                                            //开启继电器8，放大10^8倍
			HAL_GPIO_WritePin(CTRL11_GPIO_Port, CTRL11_Pin , GPIO_PIN_SET); 
			break;
		case RELAY_RANGE_1G:                                                             	//开启继电器9，放大10^9倍
			HAL_GPIO_WritePin(CTRL12_GPIO_Port, CTRL12_Pin , GPIO_PIN_SET); 
			break;
		default:  
			break;
	}
}
/******************************************************************/
/*检查继电器档位是否正确																					*/
/*返回值： 0 为正确，1为换档																			*/
/******************************************************************/
uint8_t RelayCheck(enum TestMode testMode, RelayTypeDef* pRelay, uint16_t originADC_Val)
{
	if(Relay.rangeMode!=RELAY_RANGE_AUTO)
		return 0;
	else
	{
		if((pRelay->rangeChangeTimes>9) && (pRelay->tempMaxRange>pRelay->tempMinRange))					//如果换挡次数多于9次,将最大挡位自动降低一档
		{
			if(testMode==SWEEP_DRAIN_VOL||testMode==SWEEP_GATE_VOL||testMode==SWEEP_IV)
					HAL_TIM_Base_Stop_IT(&htim3);
				HAL_TIM_Base_Stop_IT(&htim2);
			
			pRelay->tempMaxRange--;
			pRelay->rangeNow=pRelay->tempMaxRange;
			SetRangeRelay(pRelay->rangeNow);
			HAL_Delay(RANGE_CHANGE_DELAY*(pRelay->rangeNow));
			return 1;
		}
		if((originADC_Val>0xFD80)||(originADC_Val<0x0FF0)) //if((originADC_Val>0xFE00)||(originADC_Val<0x100)) 						//如果采集到的电流过大
		{
			if(pRelay->rangeNow>pRelay->tempMinRange)																	//如果档位在1档以上，仍然可以降档
			{
				if(testMode==SWEEP_DRAIN_VOL||testMode==SWEEP_GATE_VOL||testMode==SWEEP_IV)
					HAL_TIM_Base_Stop_IT(&htim3);
				HAL_TIM_Base_Stop_IT(&htim2);
				
				pRelay->rangeNow--;
				SetRangeRelay(pRelay->rangeNow);
				pRelay->rangeChangeTimes++;
				HAL_Delay(RANGE_CHANGE_DELAY*(pRelay->rangeNow));
				return 1;
			}
			else																								//如果档位在1档，无法降档，换挡结束
				return 0;
		}
		else if((originADC_Val<0x8800)&&(originADC_Val>0x7800))//如果采集到的电流过小
		{
			if(pRelay->rangeNow<pRelay->tempMaxRange)							//如果档位在9档以下，仍然可以升档
			{
				if(testMode==SWEEP_DRAIN_VOL||testMode==SWEEP_GATE_VOL||testMode==SWEEP_IV)
					HAL_TIM_Base_Stop_IT(&htim3);
				HAL_TIM_Base_Stop_IT(&htim2);
				pRelay->rangeNow++;
				SetRangeRelay(pRelay->rangeNow);
				pRelay->rangeChangeTimes++;
				HAL_Delay(RANGE_CHANGE_DELAY*(pRelay->rangeNow));
				return 1;
			}
			else																								
				return 0;
		}
		else 
			return 0;
	}
}

void ConnectDrain(void)
{
	HAL_GPIO_WritePin(CTRL20_GPIO_Port, CTRL20_Pin , GPIO_PIN_SET);
}
void DisconnectDrain(void)
{
	HAL_GPIO_WritePin(CTRL20_GPIO_Port, CTRL20_Pin , GPIO_PIN_RESET);
}
void ConnectGate(void)
{
	HAL_GPIO_WritePin(CTRL18_GPIO_Port, CTRL18_Pin , GPIO_PIN_SET);
}
void DisconnectGate(void)
{
	HAL_GPIO_WritePin(CTRL18_GPIO_Port, CTRL18_Pin , GPIO_PIN_RESET);
}
void ConnectSub(void)
{
	HAL_GPIO_WritePin(CTRL19_GPIO_Port, CTRL19_Pin , GPIO_PIN_SET);
}
void DisconnectSub(void)
{
	HAL_GPIO_WritePin(CTRL19_GPIO_Port, CTRL19_Pin , GPIO_PIN_RESET);
}

void ConnectAllOutput(void)
{
	ConnectDrain();
	ConnectGate();
	ConnectSub();
}	
void DisconnectAllOutput(void)
{
	DisconnectDrain();
	DisconnectGate();
	DisconnectSub();
}


