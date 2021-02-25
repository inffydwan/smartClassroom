#include "adc.h"

ADC_HandleTypeDef ADC_Handle;

ADC_ChannelConfTypeDef ADC_Config;
DMA_HandleTypeDef DMA_Init_Handle;

uint16_t __IO ADC_ConvertedValue[3];

static void Rheostat_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL ;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	__GPIOA_CLK_ENABLE();
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}




static void Rheostat_ADC_Mode_Config(void)
{
	__ADC1_CLK_ENABLE();
	
	__DMA2_CLK_ENABLE();
	
	DMA_Init_Handle.Instance = DMA2_Stream0;
	DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE;
	DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
	DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;
	DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	DMA_Init_Handle.Init.Channel = DMA_CHANNEL_0;
	HAL_DMA_Init(&DMA_Init_Handle);
	
	HAL_DMA_Start (&DMA_Init_Handle,RHEOSTAT_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue, 3);

	__ADC1_CLK_ENABLE();
	ADC_Handle.Instance = ADC1;
	ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;  //pclk2 90 / 4
	ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	ADC_Handle.Init.ScanConvMode = ENABLE;
	ADC_Handle.Init.ContinuousConvMode = ENABLE;
	ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
	ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_Handle.Init.NbrOfConversion = 3;
	ADC_Handle.Init.DMAContinuousRequests = ENABLE;
	ADC_Handle.Init.EOCSelection = DISABLE;
	HAL_ADC_Init(&ADC_Handle);
	
	ADC_Config.Channel = ADC_CHANNEL_13;
	ADC_Config.Rank = 1;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);

	ADC_Config.Channel = ADC_CHANNEL_4;
	ADC_Config.Rank = 2;
	HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
	
		ADC_Config.Channel = ADC_CHANNEL_6;
	ADC_Config.Rank = 3;
	HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
	
	HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADC_ConvertedValue,1);
	
}


// 配置中断优先级
//static void Rheostat_ADC_NVIC_Config(void)
//{
//  HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(ADC_IRQn);
//}

void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
  //  Rheostat_ADC_NVIC_Config();
}

//void ADC_IRQHandler(void)
//{
//	HAL_ADC_IRQHandler(&ADC_Handle);
//	
//}


//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
//{
//	ADC_ConvertedValue = HAL_ADC_GetValue(AdcHandle);
//}
