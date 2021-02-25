#include "touchKey.h"


TIM_HandleTypeDef TIM_Handle;
uint16_t tpad_default_val;


 static void TIMx_CHx_Cap_Init(uint32_t arr,uint16_t psc)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 __GPIOA_CLK_ENABLE();
	 GPIO_InitStructure.Pin = GPIO_PIN_5;
	 GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	 GPIO_InitStructure.Alternate = GPIO_AF1_TIM2;
	 GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	 GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	 __TIM2_CLK_ENABLE(); 	
	TIM_Handle.Instance = TIM2;
	 TIM_Handle.Init.Prescaler = psc;
	 TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	 TIM_Handle.Init.RepetitionCounter = 0;  //分频
	 TIM_Handle.Init.Period = arr;
	 TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_IC_Init(&TIM_Handle);

	 TIM_IC_InitTypeDef sConfigIC;
	 sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	 sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	 sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&TIM_Handle, &sConfigIC, TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&TIM_Handle, TIM_CHANNEL_1);
	 
 }
 
 
static void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin ( GPIOA, GPIO_PIN_5 ,GPIO_PIN_RESET);
	HAL_Delay(5);
	
	__HAL_TIM_CLEAR_FLAG(&TIM_Handle,TIM_FLAG_CC1);
	__HAL_TIM_CLEAR_FLAG(&TIM_Handle,TIM_FLAG_UPDATE);
	
	__HAL_TIM_SET_COUNTER(&TIM_Handle,0);
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	 GPIO_InitStructure.Alternate = GPIO_AF1_TIM2;
	 GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	 GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}


 static uint16_t TPAD_Get_Val(void)
 {
	 TPAD_Reset();
	 
	 while (__HAL_TIM_GET_FLAG(&TIM_Handle,TIM_FLAG_CC1) == RESET)
	 {
		 if (__HAL_TIM_GET_COUNTER( &TIM_Handle)>TPAD_ARR_MAX_VAL-500)  //防止一直捕获不到（硬件故障)
		 {
			 return __HAL_TIM_GET_COUNTER( &TIM_Handle);
		 }
	 }
	 return HAL_TIM_ReadCapturedValue(&TIM_Handle, TIM_CHANNEL_1);
	 
 }
 
 
 static uint16_t TPAD_Get_MaxVal(uint8_t n)
 {
	 uint16_t temp=0;
	 uint16_t res=0;
	 while (n--) 
	{
		temp=TPAD_Get_Val();
		if (temp>res)res=temp;
	}
	return res;
}
 
uint8_t TPAD_Init(void)
{
	uint16_t buf[10];
	uint16_t temp;
	uint8_t j,i;
	
	TIMx_CHx_Cap_Init(TPAD_ARR_MAX_VAL,20-1);
	for (i=0; i<10; i++)
	{
		buf[i]=TPAD_Get_Val();
		HAL_Delay(10);
	}
	
	for (i=0; i<9; i++) 
	{
		for (j=i+1; j<10; j++)
		{
			if (buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	
	temp=0;
	for (i=2; i<8; i++) 
	{
		temp+=buf[i];
	}
	
	tpad_default_val=temp/6;
	
	printf("tpad_default_val:%d\r\n",tpad_default_val);
	
	if (tpad_default_val>TPAD_ARR_MAX_VAL/2)
	{
		return 1;
	}
		
		return 0;
}



uint8_t TPAD_Scan()
{
	
	static uint8_t keyen=0;
	static uint8_t oldKey = 0; 
	
	uint8_t sample = 3;
	
	uint16_t rval;
	
	rval=TPAD_Get_MaxVal(sample);
	
	printf("scan_rval=%d\n",rval);
	

		if (rval>(tpad_default_val+TPAD_GATE_VAL)) //触控有效
		{
			
			return 1;
		}
		else return 0;
	


}

