#include "rtc.h"
#include "lcd.h"

RTC_HandleTypeDef  Rtc_Handle;

void RTC_CLK_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	
	Rtc_Handle.Instance = RTC;
	
	__HAL_RCC_PWR_CLK_ENABLE();

	HAL_PWR_EnableBkUpAccess();
	
	#if defined (RTC_CLOCK_SOURCE_LSI)
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	
	#elif defined (RTC_CLOCK_SOURCE_LSE)
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	
	#endif
	
	__HAL_RCC_RTC_ENABLE();
	HAL_RTC_WaitForSynchro(&Rtc_Handle);
	Rtc_Handle.Init.AsynchPrediv = 0X7F;
	Rtc_Handle.Init.SynchPrediv = 0XFF;
	Rtc_Handle.Init.HourFormat = RTC_HOURFORMAT_24;
	if (HAL_RTC_Init(&Rtc_Handle) != HAL_OK)  printf("\n\r RTC 时钟初始化失败 \r\n");
}



void RTC_TimeAndDate_Set(void)
{
	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;

	RTC_TimeStructure.TimeFormat = RTC_HOURFORMAT12_AM;
	RTC_TimeStructure.Hours = HOURS;
	RTC_TimeStructure.Minutes = MINUTES;
	RTC_TimeStructure.Seconds = SECONDS;
	HAL_RTC_SetTime(&Rtc_Handle,&RTC_TimeStructure, RTC_FORMAT_BIN);
	
	RTC_DateStructure.WeekDay = WEEKDAY;
	RTC_DateStructure.Date = DATE;
	RTC_DateStructure.Month = MONTH;
	RTC_DateStructure.Year = YEAR;
	HAL_RTC_SetDate(&Rtc_Handle,&RTC_DateStructure, RTC_FORMAT_BIN);
	
	HAL_RTCEx_BKUPWrite(&Rtc_Handle,RTC_BKP_DRX, RTC_BKP_DATA);
}
	

	

/**
  * @brief  显示时间和日期
  * @param  无
  * @retval 无
  */
void RTC_TimeAndDate_Show(void)
{
	uint8_t Rtctmp=0;
	char LCDTemp[100];
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	
	while(1)
	{
		// 获取日历
		HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStructure, RTC_FORMAT_BIN);
			
			// 每秒打印一次
		if(Rtctmp != RTC_TimeStructure.Seconds)
		{
							
			// 打印日期
			printf("The Date :  Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d\r\n", 
			RTC_DateStructure.Year,
			RTC_DateStructure.Month, 
			RTC_DateStructure.Date,
			RTC_DateStructure.WeekDay);

			//液晶显示日期
			//先把要显示的数据用sprintf函数转换为字符串，然后才能用液晶显示函数显示
			sprintf(LCDTemp,"The Date :  Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d", 
			RTC_DateStructure.Year,
			RTC_DateStructure.Month, 
			RTC_DateStructure.Date,
			RTC_DateStructure.WeekDay);
			
			LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
			LCD_DisplayStringLine_EN_CN(8,(uint8_t *)LCDTemp); 
			
			// 打印时间
			printf("The Time :  %0.2d:%0.2d:%0.2d \r\n\r\n", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes, 
			RTC_TimeStructure.Seconds);
			
			//液晶显示时间
			sprintf(LCDTemp,"The Time :  %0.2d:%0.2d:%0.2d", 
			RTC_TimeStructure.Hours, 
			RTC_TimeStructure.Minutes, 
			RTC_TimeStructure.Seconds);

			LCD_DisplayStringLine_EN_CN(10,(uint8_t *)LCDTemp);
				
		  (void)RTC->DR;
		}
		Rtctmp = RTC_TimeStructure.Seconds;
	}	
}




