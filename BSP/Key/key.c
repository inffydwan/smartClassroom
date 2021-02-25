#include "key.h"
#include "beeper.h"
#include "touchKey.h"

uint8_t keyState = NONE;

void keyInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	
	//KEY1
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init (GPIOA, &GPIO_InitStructure);
	
	//KEY2
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	HAL_GPIO_Init (GPIOC, &GPIO_InitStructure);
}


void keyScan()
{
	static uint8_t oldKey = NONE;
	
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)
	{
		if (oldKey != KEY1_SHORT)
		{
			keyState = KEY1_SHORT;
			oldKey = keyState;
			
			if (shortBeep_cnt > 0) shortBeep_cnt = 0;
			else shortBeep = 1;
		}
		else  keyState = NONE;
	}
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1)
	{
		if (oldKey != KEY2_SHORT)
		{
			keyState = KEY2_SHORT;
			oldKey = keyState;
			
			if (shortBeep_cnt > 0) shortBeep_cnt = 0;
			else shortBeep = 1;
		}
		else keyState = NONE;
	}
	else if (TPAD_Scan())
	{
		if (oldKey != TOUCH_KEY)
		{
//			delay = 1;
//			if (delay_cnt >= 20)
			{
				delay = 0;
				delay_cnt = 0;
				keyState = TOUCH_KEY;
				oldKey = keyState;
				
				if (shortBeep_cnt > 0) shortBeep_cnt = 0;
					else shortBeep = 1;
			}
		}
		else keyState = NONE;
	}
	else
	{
//		if (oldKey == TOUCH_KEY)
//		{
//			delay = 1;
//			if (delay_cnt >= 20)
//			{
//				delay = 0;
//				delay_cnt = 0;
//				keyState = NONE;
//				oldKey = keyState;
//			}
//		}
//		else
		{
			keyState = NONE;
			oldKey = keyState;
		}
	}
}



	