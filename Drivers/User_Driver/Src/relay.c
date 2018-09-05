#include "relay.h"

RelayTypeDef Relay;

void SetMonitorRelay(int8_t index)  //���ü�ض˼̵���
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

void ReleaseMonitorRelay(int8_t index)       //�ͷż�ض˼̵���
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
void SetRangeRelay(uint8_t range_select)       //����Դ©�����Ŵ�Χ
{
	ClearRangeRelay();
	switch(range_select)
	{
		case RELAY_RANGE_AUTO:																															//range=0ʱ��Ϊ�Զ���������������С��λ��ʼ
			HAL_GPIO_WritePin(CTRL13_GPIO_Port, CTRL13_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_51R:																															//�����̵���1���Ŵ�51��
			HAL_GPIO_WritePin(CTRL13_GPIO_Port, CTRL13_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_100R:                                                            //�����̵���2���Ŵ�10^2��
			HAL_GPIO_WritePin(CTRL1_GPIO_Port, CTRL1_Pin , GPIO_PIN_SET);		
			break;
		case RELAY_RANGE_1K:                                                             	//�����̵���3���Ŵ�10^3��
			HAL_GPIO_WritePin(CTRL2_GPIO_Port, CTRL2_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_10K:                                                            	//�����̵���4���Ŵ�10^4��
			HAL_GPIO_WritePin(CTRL3_GPIO_Port, CTRL3_Pin , GPIO_PIN_SET); 
			break;
		case RELAY_RANGE_100K:                                                            //�����̵���5���Ŵ�10^5��
			HAL_GPIO_WritePin(CTRL4_GPIO_Port, CTRL4_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_1M:                                                             	//�����̵���6���Ŵ�10^6��
			HAL_GPIO_WritePin(CTRL9_GPIO_Port, CTRL9_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_10M:                                                             //�����̵���7���Ŵ�10^7��
			HAL_GPIO_WritePin(CTRL10_GPIO_Port, CTRL10_Pin , GPIO_PIN_SET);
			break;
		case RELAY_RANGE_100M:                                                            //�����̵���8���Ŵ�10^8��
			HAL_GPIO_WritePin(CTRL11_GPIO_Port, CTRL11_Pin , GPIO_PIN_SET); 
			break;
		case RELAY_RANGE_1G:                                                             	//�����̵���9���Ŵ�10^9��
			HAL_GPIO_WritePin(CTRL12_GPIO_Port, CTRL12_Pin , GPIO_PIN_SET); 
			break;
		default:  
			break;
	}
}
/******************************************************************/
/*���̵�����λ�Ƿ���ȷ																					*/
/*����ֵ�� 0 Ϊ��ȷ��1Ϊ����																			*/
/******************************************************************/
uint8_t RelayCheck(enum TestMode testMode, RelayTypeDef* pRelay, uint16_t originADC_Val)
{
	if(Relay.rangeMode!=RELAY_RANGE_AUTO)
		return 0;
	else
	{
		if((pRelay->rangeChangeTimes>9) && (pRelay->tempMaxRange>pRelay->tempMinRange))					//���������������9��,�����λ�Զ�����һ��
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
		if((originADC_Val>0xFD80)||(originADC_Val<0x0FF0)) //if((originADC_Val>0xFE00)||(originADC_Val<0x100)) 						//����ɼ����ĵ�������
		{
			if(pRelay->rangeNow>pRelay->tempMinRange)																	//�����λ��1�����ϣ���Ȼ���Խ���
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
			else																								//�����λ��1�����޷���������������
				return 0;
		}
		else if((originADC_Val<0x8800)&&(originADC_Val>0x7800))//����ɼ����ĵ�����С
		{
			if(pRelay->rangeNow<pRelay->tempMaxRange)							//�����λ��9�����£���Ȼ��������
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


