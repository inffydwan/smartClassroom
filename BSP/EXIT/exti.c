#include "exti.h"
#include "led.h"

void extiInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	//KEY1
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init (GPIOA, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	//KEY2
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	HAL_GPIO_Init (GPIOC, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}


void key1_irqHandler()
{
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
	{
		LED_B_TOGGLE;
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	}
}


void key2_irqHandler()
{
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
	{
		LED_G_TOGGLE;
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	}
}