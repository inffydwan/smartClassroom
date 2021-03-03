#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"
#include "rcc.h"
#include "uart.h"
#include "dma_uart.h"
#include "i2c_ee.h"
#include "spi_flash.h"
#include "led.h"
#include "key.h"
#include "rtc.h"
#include "sdram.h"
#include "adc.h"
#include "timer.h"
#include "touchKey.h"
#include "beeper.h"
#include "iwdg.h"
#include "wwdg.h"
#include "lcd.h"
#include "ff.h"
#include <string.h>
#include <stdio.h>
#include "i2c_touch.h"
#include "GUI.h"
#include "MainTask.h"
#include "DIALOG.h"
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;


__IO TestStatus TransferStatus1 = FAILED;


void Delay(__IO uint32_t nCount);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);




#endif
