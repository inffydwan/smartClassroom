#include "i2c_ee.h"

static I2C_HandleTypeDef  I2C_Handle; 


void HAL_I2C_MspInit(I2C_HandleTypeDef * I2C_Handle)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	
	__HAL_RCC_I2C1_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();
	
}



static void I2C_Mode_Config(void)
{
	I2C_Handle.Instance = I2C1;
	I2C_Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2C_Handle.Init.ClockSpeed = 400000;
	I2C_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2C_Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2C_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2C_Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	I2C_Handle.Init.OwnAddress1 = I2C_OWN_ADDRESS7 ;
	I2C_Handle.Init.OwnAddress2 = 0;
	HAL_I2C_Init(&I2C_Handle);
//	HAL_I2CEx_AnalogFilter_Config(&I2C_Handle, I2C_ANALOGFILTER_ENABLE);
}


 void I2C_EE_Init(void)
 {
	 I2C_Mode_Config();
 }
 
 
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_I2C_Mem_Write(&I2C_Handle, EEPROM_ADDRESS, (uint16_t)WriteAddr,
															I2C_MEMADD_SIZE_8BIT, pBuffer, 1, 100);
	if (status != HAL_OK) {
	}
	
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY) {
	}
	
	while (HAL_I2C_IsDeviceReady(&I2C_Handle, EEPROM_ADDRESS,
		EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);
	
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY) {
	}
	
	return status;
}


/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */ 
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, 
														uint8_t NumByteToWrite)

{
	HAL_StatusTypeDef status = HAL_OK;
	
	status=HAL_I2C_Mem_Write(&I2C_Handle, EEPROM_ADDRESS,WriteAddr,
													I2C_MEMADD_SIZE_8BIT, (uint8_t*)(pBuffer),NumByteToWrite, 100);
	
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY) {}
		
		while (HAL_I2C_IsDeviceReady(&I2C_Handle, EEPROM_ADDRESS,
									EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);
		
		while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY) {}
			
			return status;
}




void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
	 uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	
	
	
	NumOfPage = NumByteToWrite / EEPROM_PAGESIZE;
	
	
	if (NumOfPage == 0)   //不到一页的数据
	{
		I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
	}
	else 
	{
		Addr = WriteAddr % EEPROM_PAGESIZE;  //写入地址是否页对齐
		
		if (Addr == 0)
		{
			while (NumOfPage--)
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, EEPROM_PAGESIZE);
				WriteAddr += EEPROM_PAGESIZE;
				pBuffer += EEPROM_PAGESIZE;
			}
			
			NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;
			
			if (NumOfSingle != 0) I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			
		}
		else
		{
			count = EEPROM_PAGESIZE - Addr;
			
			NumByteToWrite -= count;
			
			I2C_EE_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr += count;
			pBuffer += count;
	
			while (NumOfPage--)
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, EEPROM_PAGESIZE);
				WriteAddr += EEPROM_PAGESIZE;
				pBuffer += EEPROM_PAGESIZE;
			}
			
			NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;
			if (NumOfSingle != 0) I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
}
			







uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
	HAL_StatusTypeDef status = HAL_OK;

	status=HAL_I2C_Mem_Read(&I2C_Handle,EEPROM_ADDRESS,ReadAddr,
				I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, NumByteToRead, 1000);
	
	return status;
}
	
	
