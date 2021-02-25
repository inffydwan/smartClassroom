#include "sdram.h"

SDRAM_HandleTypeDef sdramHandle;
static FMC_SDRAM_CommandTypeDef Command;

static void SDRAM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__GPIOF_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	__GPIOG_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//����Ϊ���ù���
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStructure.Alternate = GPIO_AF12_FMC;
	
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_15;	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

}

static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}
	
static void SDRAM_InitSequence(void)
{
	uint32_t tmpr = 0;
	
	/* ������������ṩ�� SDRAM ��ʱ�� */
	Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;
	
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
	
	/* Step 2: ��ʱ 100us */
	SDRAM_delay(1);
	
	/* ������������е� bank Ԥ��� */
	/* Step 5 --------------------------------------------------------------------*/
  /* ������������е�bankԤ��� */ 
  Command.CommandMode = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = 0;
/* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
	
	/* Step 6 --------------------------------------------------------------------*/
  /* ��������Զ�ˢ�� */   
  Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber = 4;
  Command.ModeRegisterDefinition = 0;
 /* Send the command */
  HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
	
	/* Step 7 --------------------------------------------------------------------*/
  /* ����sdram�Ĵ������� */
  tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                   SDRAM_MODEREG_CAS_LATENCY_3           |
                   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  /* �����������SDRAM�Ĵ��� */
  Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command.AutoRefreshNumber = 1;
  Command.ModeRegisterDefinition = tmpr;
	
	HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);
	
	
}

 void SDRAM_Init(void)
 {
	 FMC_SDRAM_TimingTypeDef SdramTiming;
	 
	 SDRAM_GPIO_Config();
	 
	 __FMC_CLK_ENABLE();

	 sdramHandle.Instance = FMC_SDRAM_DEVICE; 
	 sdramHandle.Init.SDBank = FMC_SDRAM_BANK2;
	 sdramHandle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;//SDRAM ����
	 sdramHandle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;//SDRAM ����
	 sdramHandle.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;//�������ݿ��Ϊ 16 λ
	 sdramHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;//4 ������
	 sdramHandle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;//�е�ַѡͨ����ʱ
	 sdramHandle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;//��ֹд����
	 sdramHandle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;//SDRAM ʱ�� fpclk=90M
	sdramHandle.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE; //ʹ��ͻ������ģʽ
	sdramHandle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1; //��ͨ����ʱ


 SdramTiming.LoadToActiveDelay = 2;//����ģʽ�Ĵ�������������Ч��ˢ������֮����ӳ�
	SdramTiming.ExitSelfRefreshDelay = 8;
	SdramTiming.SelfRefreshTime = 5;
	SdramTiming.RowCycleDelay = 7;
	SdramTiming.WriteRecoveryTime = 2;
	SdramTiming.RPDelay = 2;
	SdramTiming.RCDDelay = 2;
	
	HAL_SDRAM_Init(&sdramHandle, &SdramTiming);
	SDRAM_InitSequence();
}



	