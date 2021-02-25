#include "wwdg.h"
#include "led.h"

WWDG_HandleTypeDef WWDG_Handle;


static void WWDG_NVIC_Config(void)
{
	HAL_NVIC_SetPriority(WWDG_IRQn,0,0);
	HAL_NVIC_EnableIRQ(WWDG_IRQn);
}


void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{
	__WWDG_CLK_ENABLE();
	WWDG_NVIC_Config();
	WWDG_Handle.Instance = WWDG;
	WWDG_Handle.Init.Prescaler = prv;
	WWDG_Handle.Init.Window = wr;
	WWDG_Handle.Init.Counter = tr;
	WWDG_Handle.Init.EWIMode = WWDG_EWI_ENABLE;
	HAL_WWDG_Init(&WWDG_Handle);
	
}




void WWDG_IRQHandler(void)
{
	HAL_WWDG_IRQHandler(&WWDG_Handle);
	
}


void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
	LED_YELLOW;
}


void WWDG_Feed(void)
{
	HAL_WWDG_Refresh(&WWDG_Handle);
}