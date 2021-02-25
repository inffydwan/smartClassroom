#ifndef __UART_H
#define __UART_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

#define DEBUG_USART_IRQHandler USART1_IRQHandler

void uart1_init();
void uart1_sendString(uint8_t *str);
extern UART_HandleTypeDef UartHandle;



#endif