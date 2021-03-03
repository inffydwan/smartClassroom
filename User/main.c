#include "main.h"


int main(void)
{
	HAL_Init();

  SystemClock_Config();

	uart1_init();
	led_init();	
	beeperInit();


	RTC_CLK_Config();
	
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
	













static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;
  
  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}




/* �򵥵���ʱ���� */
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}


