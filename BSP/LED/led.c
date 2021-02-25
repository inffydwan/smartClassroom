#include "led.h"


void led_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__GPIOH_CLK_ENABLE();
	
	//LED_R
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init (GPIOH, &GPIO_InitStructure);
	
	//LED_G
	GPIO_InitStructure.Pin = GPIO_PIN_11;	
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	//LED_B
	GPIO_InitStructure.Pin = GPIO_PIN_12;	
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	LED_R_OFF;
	LED_G_OFF;
	LED_B_OFF;
}



