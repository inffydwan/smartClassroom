#include "dma_uart.h"
#include "uart.h"


DMA_HandleTypeDef DMA_Handle;


void USART_DMA_Config(void)
{
	
	__DMA2_CLK_ENABLE();
	DMA_Handle.Instance = DMA2_Stream7;
	DMA_Handle.Init.Channel = DMA_CHANNEL_4;
	DMA_Handle.Init.Direction= DMA_MEMORY_TO_PERIPH;
	DMA_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;
	DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	DMA_Handle.Init.Mode = DMA_CIRCULAR;
	DMA_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;
	DMA_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_DeInit(&DMA_Handle);
	HAL_DMA_Init(&DMA_Handle);
	__HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle);
}


