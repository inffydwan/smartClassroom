#ifndef __I2C_EE_H
#define __I2C_EE_H

#include "stm32f4xx_hal.h"

#define I2C_OWN_ADDRESS7      0X0A 

#define EEPROM_ADDRESS        0xA0

#define EEPROM_MAX_TRIALS               300

#define I2Cx_TIMEOUT_MAX                300

#define EEPROM_PAGESIZE  8

#define  DATA_Size			256

#define  EEP_Firstpage      0x00

#define EEPROM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

												

 void I2C_EE_Init(void);
 uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr);
 
 uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, 
														uint8_t NumByteToWrite);
														
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
				
 uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
 
 

#endif