#include "uart.h"

UART_HandleTypeDef UartHandle;

void uart1_init()
{
	 __USART1_CLK_ENABLE();
	UartHandle.Instance = USART1;
	UartHandle.Init.BaudRate = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	//UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;

	HAL_UART_Init(&UartHandle);

}



void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	__GPIOA_CLK_ENABLE();

	GPIO_InitStructure.Pin = GPIO_PIN_9;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART1;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 
	HAL_GPIO_Init (GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init (GPIOA, &GPIO_InitStructure);

	HAL_NVIC_SetPriority(USART1_IRQn ,0,1); 
	HAL_NVIC_EnableIRQ(USART1_IRQn);

}


void uart1_sendString(uint8_t *str)
{
	unsigned int k = 0;
	do {
		HAL_UART_Transmit( &UartHandle,(uint8_t *)(str + k) ,1,1000);
		k++;
	} while (*(str + k)!='\0');
}



void DEBUG_USART_IRQHandler(void)
{
	uint8_t ch = 0;
	if (__HAL_UART_GET_FLAG( &UartHandle, UART_FLAG_RXNE ) != RESET)
	{
		ch = (uint16_t)READ_REG(UartHandle.Instance->DR);
		WRITE_REG(UartHandle.Instance->DR, ch);  //重新发送给源设备
	}
}


int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
	return (ch);
}

int fgetc(FILE *f)
{
	int ch;
	while (__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) == RESET);
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
	return (ch);
}



