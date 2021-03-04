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
	/* LCD��ʼ�� */
	LCD_Init();

	
  __HAL_RCC_CRC_CLK_ENABLE(); // STemWin GUI_Initǰ�����CRC
  /* ��ʼ��GUI */
	GUI_Init();
	/* ���������� */
	WM_MULTIBUF_Enable(1);
  
  while(1)
  {

    MainTask();
  }
}
	




/* �򵥵���ʱ���� */
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}


