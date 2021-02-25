#ifndef __RTC_H
#define __RTC_H


#include "stm32f4xx_hal.h"
#include <stdio.h>

#define RTC_CLOCK_SOURCE_LSE      
//#define RTC_CLOCK_SOURCE_LSI  


// ʱ��궨��
#define RTC_H12_AMorPM			  RTC_HOURFORMAT12_AM  
#define HOURS                     18          // 0~23
#define MINUTES                   9          // 0~59
#define SECONDS                   00          // 0~59


// ���ں궨��
#define WEEKDAY                   3         // 1~7
#define DATE                      24         // 1~31
#define MONTH                     2         // 1~12
#define YEAR                      21         // 0~99

// ������Ĵ����궨��
#define RTC_BKP_DRX          RTC_BKP_DR0
// д�뵽���ݼĴ��������ݺ궨��
#define RTC_BKP_DATA         0X32F2 

extern RTC_HandleTypeDef  Rtc_Handle;

void RTC_CLK_Config(void);
void RTC_TimeAndDate_Set(void);
void RTC_TimeAndDate_Show(void);
	

#endif