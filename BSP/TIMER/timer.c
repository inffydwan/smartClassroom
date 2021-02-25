#include "timer.h"
#include "led.h"

TIM_HandleTypeDef TIM_TimeBaseStructure;

static void TIM_Mode_Config(void)
{
	__TIM6_CLK_ENABLE();
	
	TIM_TimeBaseStructure.Instance = TIM6;
	TIM_TimeBaseStructure.Init.Period = 5000-1;
	
	TIM_TimeBaseStructure.Init.Prescaler = 9000-1;  //10KHz
	TIM_TimeBaseStructure.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&TIM_TimeBaseStructure);
	HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure);
	

}


void TIM6_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&TIM_TimeBaseStructure);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim==(&TIM_TimeBaseStructure)) LED_R_TOGGLE;
	
}


static void TIMx_NVIC_Configuration(void)
{
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 3);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}


void tim6_init()
{
	TIMx_NVIC_Configuration();
	TIM_Mode_Config();
}