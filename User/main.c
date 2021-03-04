#include "main.h"

static void SysTick_Init(void);



int main(void)
{
	HAL_Init();

  SystemClock_Config();

	uart1_init();
	led_init();	
	beeperInit();


	RTC_CLK_Config();
		GTP_Init_Panel();
	SDRAM_Init();
	/* LCD初始化 */
	LCD_Init();

	
  __HAL_RCC_CRC_CLK_ENABLE(); // STemWin GUI_Init前必须打开CRC
  /* 初始化GUI */
	GUI_Init();
	/* 开启三缓冲 */
	WM_MULTIBUF_Enable(1);
  
  while(1)
  {

    MainTask();
  }
}
	




/* 简单的延时函数 */
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}


