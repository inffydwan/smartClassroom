#ifndef __DMA_UART_H
#define __DMA_UART_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

#define SENDBUFF_SIZE                     		10//发送的数据量

void USART_DMA_Config(void);


#endif