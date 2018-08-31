#include "calculation.h"
#include "calibration.h"

uint8_t Do_Calculation(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult, RelayTypeDef* pRelay)
{
		pTestResult->sampleCount++;																		//����������
		pTestResult->I_sum+=pTestResult->I_sample;									//�ۼӲ���ֵ
		if(pTestResult->sampleCount>=pTestPara->avgNumber)						//�������������ƽ��ֵ����
		{
			//pRelay->rangeNow=1;
			HAL_TIM_Base_Stop_IT(&htim2);	
			pTestResult->I_avg.numFloat=((float)pTestResult->I_sum)/pTestResult->sampleCount;			//����ƽ��ֵ	
			if(pRelay->rangeNow==1)
				pTestResult->I_avg.numFloat=(((float)pTestResult->I_avg.numFloat)*2.5/65535-1.25)/51;
			else
				pTestResult->I_avg.numFloat=(((float)pTestResult->I_avg.numFloat)*2.5/65535-1.25)/MyPow(10,(pRelay->rangeNow));		//ADC�ɼ�����ֵ��ȥ1.25Vƫ�ã��õ����ηŴ���ֵ																								//�����ۼӣ�����
			return 0;				
		}
		return 1;																						//û����Ч�Ľ��
}




