/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "spi_flash.h"
#include "rtc.h"

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map Ramdisk to physical drive 0 */
#define SPI_FLASH		1	/* Example: Map MMC/SD card to physical drive 1 */

/*-----------------------------------------------------------------------*/
/* ��ȡ�豸״̬                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* ������ */
)
{

	DSTATUS status = STA_NOINIT;
	
	switch (pdrv) {
		case ATA:	/* SD CARD */
			break;
    
		case SPI_FLASH:      
      /* SPI Flash״̬��⣺��ȡSPI Flash �豸ID */
      if(sFLASH_ID == SPI_FLASH_ReadID())
      {
        /* �豸ID��ȡ�����ȷ */
        status &= ~STA_NOINIT;
      }
      else
      {
        /* �豸ID��ȡ������� */
        status = STA_NOINIT;;
      }
			break;

		default:
			status = STA_NOINIT;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* �豸��ʼ��                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* ������ */
)
{
  uint16_t i;
	DSTATUS status = STA_NOINIT;	
	switch (pdrv) {
		case ATA:	         /* SD CARD */
			break;
    
		case SPI_FLASH:    /* SPI Flash */ 
      /* ��ʼ��SPI Flash */
			SPI_FLASH_Init();
      /* ��ʱһС��ʱ�� */
      i=500;
	    while(--i);	
      /* ����SPI Flash */
	    SPI_Flash_WAKEUP();
      /* ��ȡSPI FlashоƬ״̬ */
      status=disk_status(SPI_FLASH);
			break;
      
		default:
			status = STA_NOINIT;
	}
	return status;
}


/*-----------------------------------------------------------------------*/
/* ����������ȡ�������ݵ�ָ���洢��                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* �豸������(0..) */
	BYTE *buff,		/* ���ݻ����� */
	DWORD sector,	/* �����׵�ַ */
	UINT count		/* ��������(1..128) */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			break;
    
		case SPI_FLASH:
      /* ����ƫ��16MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����16MB�ռ� */
      sector+=4096;      
      SPI_FLASH_BufferRead(buff, sector <<12, count<<12);
      status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* д������������д��ָ�������ռ���                                      */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			  /* �豸������(0..) */
	const BYTE *buff,	/* ��д�����ݵĻ����� */
	DWORD sector,		  /* �����׵�ַ */
	UINT count			  /* ��������(1..128) */
)
{
  uint32_t write_addr; 
	DRESULT status = RES_PARERR;
	if (!count) {
		return RES_PARERR;		/* Check parameter */
	}

	switch (pdrv) {
		case ATA:	/* SD CARD */      
		break;

		case SPI_FLASH:
      /* ����ƫ��16MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����16MB�ռ� */
			sector+=4096;
      write_addr = sector<<12;    
      SPI_FLASH_SectorErase(write_addr);
      SPI_FLASH_BufferWrite((uint8_t *)buff,write_addr,count<<12);
      status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif



/**
  * @brief  I/O control operation  
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* ������ */
	BYTE cmd,		  /* ����ָ�� */
	void *buff		/* д����߶�ȡ���ݵ�ַָ�� */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) {
		case ATA:	/* SD CARD */
			break;
    
		case SPI_FLASH:
			switch (cmd) {
				//256(1M) * (256M(W25Q256JV) - 16M) = 64000
        case GET_SECTOR_COUNT:
          *(DWORD * )buff = 64000;		
        break;
        /* ������С  */
        case GET_SECTOR_SIZE :
          *(WORD * )buff = 4096;
        break;
        // 1 block = 16 sectors
        case GET_BLOCK_SIZE :
          *(DWORD * )buff = 65536;
        break;        
      }
      status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif



/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime (void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	// ��ȡ����
		HAL_RTC_GetTime(&Rtc_Handle, &RTC_TimeStructure, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&Rtc_Handle, &RTC_DateStructure, RTC_FORMAT_BIN);
	
	DWORD dateTime = 0;
	
	//bit31:25�꣬��1980�꿪ʼ����(0..127)
	dateTime |= ((RTC_DateStructure.Year + 20 | 0xFFFFFFFF) << 25);
	//bit24:21 ��(1..12)
	dateTime |= ((RTC_DateStructure.Month | 0xFFFFFFFF) << 21);
	//bit20:16  ��(1..31)
	dateTime |= ((RTC_DateStructure.Date | 0xFFFFFFFF) << 16);
	//bit15:11  ʱ(0..23)
	dateTime |= ((RTC_TimeStructure.Hours | 0xFFFFFFFF) << 11);
	//bit10:5 ��(0..59)
	dateTime |= ((RTC_TimeStructure.Minutes | 0xFFFFFFFF) << 5);
	//bit4:0��/2(0..29)���ɴ˿ɼ�FatFs��ʱ��ֱ���Ϊ2��
	dateTime |= (RTC_TimeStructure.Seconds / 2);
	
  return ((DWORD) (RTC_DateStructure.Year + 20) << 25) 
					| ((DWORD)RTC_DateStructure.Month << 21)
					| ((DWORD)RTC_DateStructure.Date << 16)
					| ((DWORD)RTC_TimeStructure.Hours << 11)
					| ((DWORD)RTC_TimeStructure.Minutes << 5)
					| ((DWORD)(RTC_TimeStructure.Seconds / 2));
}
//__weak DWORD get_fattime(void)
//{
//	return ((DWORD)(2020 - 1980) << 25)
//					| ((DWORD)12 << 21)
//					| ((DWORD)6 << 16)
//					| ((DWORD)20 << 11)
//					| ((DWORD)16 << 5)
//					| ((DWORD)0 << 1);
//}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

