#include "beeper.h"

void beeperInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__GPIOI_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init (GPIOI, &GPIO_InitStructure);
	
}



