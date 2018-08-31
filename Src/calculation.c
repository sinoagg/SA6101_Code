#include "calculation.h"
#include "calibration.h"

uint8_t Do_Calculation(TestPara_TypeDef* pTestPara, TestResult_TypeDef* pTestResult, RelayTypeDef* pRelay)
{
		pTestResult->sampleCount++;																		//采样数增加
		pTestResult->I_sum+=pTestResult->I_sample;									//累加采样值
		if(pTestResult->sampleCount>=pTestPara->avgNumber)						//如果采样数到达平均值个数
		{
			//pRelay->rangeNow=1;
			HAL_TIM_Base_Stop_IT(&htim2);	
			pTestResult->I_avg.numFloat=((float)pTestResult->I_sum)/pTestResult->sampleCount;			//计算平均值	
			if(pRelay->rangeNow==1)
				pTestResult->I_avg.numFloat=(((float)pTestResult->I_avg.numFloat)*2.5/65535-1.25)/51;
			else
				pTestResult->I_avg.numFloat=(((float)pTestResult->I_avg.numFloat)*2.5/65535-1.25)/MyPow(10,(pRelay->rangeNow));		//ADC采集到的值减去1.25V偏置，得到二次放大后的值																								//重新累加，清零
			return 0;				
		}
		return 1;																						//没有有效的结果
}




