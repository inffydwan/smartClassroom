#include "main.h"


int main(void)
{
	uint8_t wwdg_tr, wwdg_wr;

  SystemClock_Config();

	uart1_init();
	led_init();	
	beeperInit();

	LCD_Init();
   
	RTC_CLK_Config();
	
	

	while(1)
	{
		
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




/* 简单的延时函数 */
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


