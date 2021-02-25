#include "timer.h"



/*
�߼���ʱ�������ܣ�
1������ȽϽṹ�� TIM_OCInitTypeDef ��������Ƚ�ģʽ
2�����벶��ṹ�� TIM_IC_InitTypeDef �������벶��ģʽ
3����·�������ṹ�� TIM_BreakDeadTimeConfigTypeDef ���ڶ�·����������������
*/


TIM_HandleTypeDef TIM_PWMOUTPUT_Handle;
TIM_HandleTypeDef TIM_PWMINPUT_Handle;
uint32_t IC1Value, IC2Value;
float DutyCycle, Frequency;
static TIM_BreakDeadTimeConfigTypeDef  TIM_BDTRInitStructure;

static void TIMx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = GPIO_PIN_5;   
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Alternate = GPIO_AF3_TIM8;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  //TIM8_CH1N

}

 static void TIMx_NVIC_Configuration(void)
 {
		HAL_NVIC_SetPriority(TIM8_CC_IRQn, 0, 3);
	 HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
 }
 
static void TIM_PWMOUTPUT_Config(void)
{
 
 __TIM2_CLK_ENABLE();
 TIM_PWMOUTPUT_Handle.Instance = TIM2;
 TIM_PWMOUTPUT_Handle.Init.Period = 10000-1;
 TIM_PWMOUTPUT_Handle.Init.Prescaler = 90-1;
 TIM_PWMOUTPUT_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
 TIM_PWMOUTPUT_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
 TIM_PWMOUTPUT_Handle.Init.RepetitionCounter=0;
 HAL_TIM_PWM_Init(&TIM_PWMOUTPUT_Handle);
 
 TIM_OC_InitTypeDef TIM_OCInitStructure;
 TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
 TIM_OCInitStructure.Pulse = 5000;
 TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
 TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
 TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
 TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;
HAL_TIM_PWM_ConfigChannel(&TIM_PWMOUTPUT_Handle,&TIM_OCInitStructure,TIM_CHANNEL_1);
 HAL_TIM_PWM_Start(&TIM_PWMOUTPUT_Handle,TIM_CHANNEL_1);
 
}
 
static void TIM_PWMINPUT_Config(void)
{
	
	__TIM8_CLK_ENABLE();
	TIM_PWMINPUT_Handle.Instance = TIM8;
	
	TIM_PWMINPUT_Handle.Init.Period = 0xFFFF;
	TIM_PWMINPUT_Handle.Init.Prescaler = 180-1;
	TIM_PWMINPUT_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM_PWMINPUT_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
	HAL_TIM_IC_Init(&TIM_PWMINPUT_Handle);
	
	
	
	TIM_IC_InitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;
	TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_DIRECTTI;
	TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
	//�����Ҫ���������źŵ�ÿ����Ч���أ������� 1 ��Ƶ����
	TIM_ICInitStructure.ICFilter = 0x0;
	HAL_TIM_IC_ConfigChannel(&TIM_PWMINPUT_Handle,&TIM_ICInitStructure,TIM_CHANNEL_1);
	
	//IC2 ����ͨ��2
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_FALLING;
	TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.ICFilter = 0x0;
	HAL_TIM_IC_ConfigChannel(&TIM_PWMINPUT_Handle,&TIM_ICInitStructure,TIM_CHANNEL_2);
	
	TIM_SlaveConfigTypeDef TIM_SlaveConfigStructure;
	TIM_SlaveConfigStructure.SlaveMode = TIM_SLAVEMODE_RESET;
	TIM_SlaveConfigStructure.InputTrigger = TIM_TS_TI1FP1;
	HAL_TIM_SlaveConfigSynchronization(&TIM_PWMINPUT_Handle,&TIM_SlaveConfigStructure);
	
	HAL_TIM_IC_Start_IT(&TIM_PWMINPUT_Handle,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&TIM_PWMINPUT_Handle,TIM_CHANNEL_2);
}




void tim8_init()
{
	TIMx_GPIO_Config();
	TIMx_NVIC_Configuration();
	TIM_PWMOUTPUT_Config();
	TIM_PWMINPUT_Config();
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
 {
	 IC1Value = HAL_TIM_ReadCapturedValue(&TIM_PWMINPUT_Handle,TIM_CHANNEL_1);
	 IC2Value = HAL_TIM_ReadCapturedValue(&TIM_PWMINPUT_Handle,TIM_CHANNEL_2);
	 
	 if (IC1Value != 0)
	 {
		 DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);
		 Frequency = 180000000/180/(float)(IC1Value+1);
	 }
 }
 
}


void TIM8_CC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TIM_PWMINPUT_Handle);
}