#include "lcd.h"
#include "font24.c"
#include "font20.c"
#include "font16.c"
#include "font12.c"
#include "font8.c"

/*
LTDC ���𲻶�ˢ��Һ���������ϰ��Դ�ռ�����ݴ��䵽Һ����������ʾ����
DMA2D ����ͼ�����ݰ��ˡ���ϼ���ʽת�����������õ����ݴ����Դ棩��RAM �洢����Ϊ�Դ档
�����Դ����ʹ�� STM32 оƬ�ڲ��� SRAM ������ SDRAM/SRAM��ֻҪ�����㹻�󼴿�(����Ҫ�ܴ洢һ֡ͼ������)��
*/

static LTDC_HandleTypeDef  Ltdc_Handler;
static DMA2D_HandleTypeDef Dma2d_Handler;


/* Default LCD configuration with LCD Layer 1 (backGround)*/
static uint32_t            ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];

/**
  * @brief  ��ʼ������LCD��IO
  * @param  ��
  * @retval ��
  */
static void LCD_GPIO_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
  
  /* ʹ��LCDʹ�õ�������ʱ�� */
                          //��ɫ������
  LTDC_R0_GPIO_CLK_ENABLE();LTDC_R1_GPIO_CLK_ENABLE();LTDC_R2_GPIO_CLK_ENABLE();\
  LTDC_R3_GPIO_CLK_ENABLE();LTDC_R4_GPIO_CLK_ENABLE();LTDC_R5_GPIO_CLK_ENABLE();\
  LTDC_R6_GPIO_CLK_ENABLE();LTDC_R7_GPIO_CLK_ENABLE();LTDC_G0_GPIO_CLK_ENABLE();\
  LTDC_G1_GPIO_CLK_ENABLE();LTDC_G2_GPIO_CLK_ENABLE();LTDC_G3_GPIO_CLK_ENABLE();\
  LTDC_G3_GPIO_CLK_ENABLE();LTDC_G5_GPIO_CLK_ENABLE();LTDC_G6_GPIO_CLK_ENABLE();\
  LTDC_G7_GPIO_CLK_ENABLE();LTDC_B0_GPIO_CLK_ENABLE();LTDC_B1_GPIO_CLK_ENABLE();\
  LTDC_B2_GPIO_CLK_ENABLE();LTDC_B3_GPIO_CLK_ENABLE();LTDC_B4_GPIO_CLK_ENABLE();\
  LTDC_B5_GPIO_CLK_ENABLE();LTDC_B6_GPIO_CLK_ENABLE();LTDC_B7_GPIO_CLK_ENABLE();\
  LTDC_CLK_GPIO_CLK_ENABLE();LTDC_HSYNC_GPIO_CLK_ENABLE();LTDC_VSYNC_GPIO_CLK_ENABLE();\
  LTDC_DE_GPIO_CLK_ENABLE();LTDC_DISP_GPIO_CLK_ENABLE();LTDC_BL_GPIO_CLK_ENABLE();
/* GPIO���� */

 /* ��ɫ������ */                        
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  
  GPIO_InitStruct.Pin =   LTDC_R0_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R0_AF;
  HAL_GPIO_Init(LTDC_R0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_R1_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R1_AF;
  HAL_GPIO_Init(LTDC_R1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_R2_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R2_AF;
  HAL_GPIO_Init(LTDC_R2_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_R3_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R3_AF;
  HAL_GPIO_Init(LTDC_R3_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_R4_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R4_AF;
  HAL_GPIO_Init(LTDC_R4_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_R5_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R5_AF;
  HAL_GPIO_Init(LTDC_R5_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_R6_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R6_AF;
  HAL_GPIO_Init(LTDC_R6_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_R7_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R7_AF;
  HAL_GPIO_Init(LTDC_R7_GPIO_PORT, &GPIO_InitStruct);
  
  //��ɫ������
  GPIO_InitStruct.Pin =   LTDC_G0_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_G0_AF;
  HAL_GPIO_Init(LTDC_G0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_G1_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_G1_AF;
  HAL_GPIO_Init(LTDC_G1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_G2_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G2_AF;
  HAL_GPIO_Init(LTDC_G2_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_G3_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G3_AF;
  HAL_GPIO_Init(LTDC_G3_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_G4_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G4_AF;
  HAL_GPIO_Init(LTDC_G4_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_G5_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G5_AF;
  HAL_GPIO_Init(LTDC_G5_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_G6_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G6_AF;
  HAL_GPIO_Init(LTDC_G6_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_G7_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G7_AF;
  HAL_GPIO_Init(LTDC_G7_GPIO_PORT, &GPIO_InitStruct);
  
  //��ɫ������
  GPIO_InitStruct.Pin =   LTDC_B0_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B0_AF;
  HAL_GPIO_Init(LTDC_B0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =   LTDC_B1_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B1_AF;
  HAL_GPIO_Init(LTDC_B1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_B2_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B2_AF;
  HAL_GPIO_Init(LTDC_B2_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_B3_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B3_AF;
  HAL_GPIO_Init(LTDC_B3_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_B4_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B4_AF;
  HAL_GPIO_Init(LTDC_B4_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_B5_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B5_AF;
  HAL_GPIO_Init(LTDC_B5_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_B6_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B6_AF;
  HAL_GPIO_Init(LTDC_B6_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin =   LTDC_B7_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B7_AF;
  HAL_GPIO_Init(LTDC_B7_GPIO_PORT, &GPIO_InitStruct);
  
  //�����ź���
  GPIO_InitStruct.Pin = LTDC_CLK_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_CLK_AF;
  HAL_GPIO_Init(LTDC_CLK_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = LTDC_HSYNC_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_HSYNC_AF;
  HAL_GPIO_Init(LTDC_HSYNC_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = LTDC_VSYNC_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_VSYNC_AF;
  HAL_GPIO_Init(LTDC_VSYNC_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = LTDC_DE_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_DE_AF;
  HAL_GPIO_Init(LTDC_DE_GPIO_PORT, &GPIO_InitStruct);
  
  //����BL ��Һ��ʹ���ź�DISP
  GPIO_InitStruct.Pin = LTDC_DISP_GPIO_PIN;                             
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  
  HAL_GPIO_Init(LTDC_DISP_GPIO_PORT, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.Pin = LTDC_BL_GPIO_PIN; 
  HAL_GPIO_Init(LTDC_BL_GPIO_PORT, &GPIO_InitStruct);
  
}




void LCD_Init(void)
{
	  RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;  
   
    /* ��ʼ��SDRAM ����LCD �Դ�*/
    SDRAM_Init();
	
	    /* Һ����ʱ������ */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
    periph_clk_init_struct.PLLSAI.PLLSAIR = 5;
    periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
	
	 
    /* ��ʼ��LCD���� */
    LCD_GPIO_Config();
	/* ʹ��LTDCʱ�� */
    __HAL_RCC_LTDC_CLK_ENABLE();
    /* ʹ��DMA2Dʱ�� */
    __HAL_RCC_DMA2D_CLK_ENABLE();
    /* ����LTDC���� */
    Ltdc_Handler.Instance = LTDC;  
	  /* �������� */
    /* ��ʼ����ͬ�����ԣ��͵�ƽ��Ч */
    Ltdc_Handler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    /* ��ʼ����ͬ�����ԣ��͵�ƽ��Ч */
    Ltdc_Handler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    /* ��ʼ��������Ч���ԣ��͵�ƽ��Ч */
    Ltdc_Handler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    /* ��ʼ��������ʱ�Ӽ��ԣ�ͬ����ʱ�� */
    Ltdc_Handler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    /* ������ͬ���źſ���(HSW-1) */
    Ltdc_Handler.Init.HorizontalSync =HSW-1;
    /* ���ô�ֱͬ���źſ���(VSW-1) */
    Ltdc_Handler.Init.VerticalSync = VSW-1;
    /* ����(HSW+HBP-1) */
    Ltdc_Handler.Init.AccumulatedHBP = HSW+HBP-1;
    /* ����(VSW+VBP-1) */
    Ltdc_Handler.Init.AccumulatedVBP = VSW+VBP-1;
    /* ����(HSW+HBP+��Ч���ؿ���-1) */
    Ltdc_Handler.Init.AccumulatedActiveW = HSW+HBP+LCD_PIXEL_WIDTH-1;
    /* ����(VSW+VBP+��Ч���ظ߶�-1) */
    Ltdc_Handler.Init.AccumulatedActiveH = VSW+VBP+LCD_PIXEL_HEIGHT-1;
    /* �����ܿ���(HSW+HBP+��Ч���ؿ���+HFP-1) */
    Ltdc_Handler.Init.TotalWidth =HSW+ HBP+LCD_PIXEL_WIDTH + HFP-1; 
    /* �����ܸ߶�(VSW+VBP+��Ч���ظ߶�+VFP-1) */
    Ltdc_Handler.Init.TotalHeigh =VSW+ VBP+LCD_PIXEL_HEIGHT + VFP-1;
		    /* ����LCD���������ɫ��Ĭ�Ϻ�ɫ */
    Ltdc_Handler.Init.Backcolor.Red = 0;
    Ltdc_Handler.Init.Backcolor.Green = 0;
    Ltdc_Handler.Init.Backcolor.Blue = 0;
		  /* ��ʼ��LCD�����ؿ��Ⱥ͸߶� */
    Ltdc_Handler.LayerCfg->ImageWidth  = LCD_PIXEL_WIDTH;
    Ltdc_Handler.LayerCfg->ImageHeight = LCD_PIXEL_HEIGHT;
		
		HAL_LTDC_Init(&Ltdc_Handler);

    /* ��ʼ������ */
    LCD_SetFont(&LCD_DEFAULT_FONT);
		 /* LCD ��һ���ʼ�� */ 
    LCD_LayerInit(0, LCD_FB_START_ADDRESS,ARGB8888);
	/* LCD �ڶ����ʼ�� */ 
    LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),ARGB8888);
    /* ʹ��LCD������������ */ 
    LCD_DisplayOn(); 

    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(0);

	/* ��һ����������ʾȫ�� */ 
    LCD_Clear(LCD_COLOR_BLACK);  

    /* ѡ��LCD�ڶ��� */
    LCD_SelectLayer(1);

    /* �ڶ�����������ʾ */ 
    LCD_Clear(LCD_COLOR_TRANSPARENT);

    /* ���õ�һ�͵ڶ����͸����,��СֵΪ0(��ȫ͸���������ֵΪ255*/
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);
		
		LCD_SelectLayer(0);
		LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}


/**
  * @brief  ��ȡLCD��ǰ��Ч��X��Ĵ�С
  * @retval X��Ĵ�С
  */
uint32_t LCD_GetXSize(void)
{
  return Ltdc_Handler.LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  ��ȡLCD��ǰ��Ч��Y��Ĵ�С
  * @retval Y��Ĵ�С
  */
uint32_t LCD_GetYSize(void)
{
  return Ltdc_Handler.LayerCfg[ActiveLayer].ImageHeight;
}

/**
  * @brief  ����LCD��ǰ��Ч��Y��Ĵ�С
  * @param  imageWidthPixels��ͼ��������ظ���
  * @retval ��
  */
void LCD_SetXSize(uint32_t imageWidthPixels)
{
  Ltdc_Handler.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
  * @brief  ����LCD��ǰ��Ч��Y��Ĵ�С
  * @param  imageHeightPixels��ͼ��߶����ظ���
  * @retval None
  */
void LCD_SetYSize(uint32_t imageHeightPixels)
{
  Ltdc_Handler.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

/**
  * @brief  ��ʼ��LCD�� 
  * @param  LayerIndex:  ǰ����(��1)���߱�����(��0)
  * @param  FB_Address:  ÿһ���Դ���׵�ַ
  * @param  PixelFormat: ������ظ�ʽ
  * @retval ��
  */
void LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address,uint32_t PixelFormat)
{     
  LTDC_LayerCfgTypeDef  layer_cfg;

  /* ���ʼ�� */
  layer_cfg.WindowX0 = 0;				//������ʼλ��X����
  layer_cfg.WindowX1 = LCD_GetXSize();	//���ڽ���λ��X����
  layer_cfg.WindowY0 = 0;				//������ʼλ��Y����
  layer_cfg.WindowY1 = LCD_GetYSize();  //���ڽ���λ��Y����
  layer_cfg.PixelFormat = PixelFormat;	//���ظ�ʽ
  layer_cfg.FBStartAdress = FB_Address; //���Դ��׵�ַ
  layer_cfg.Alpha = 255;				//���ڻ�ϵ�͸���ȳ�������Χ��0��255��0Ϊ��ȫ͸��
  layer_cfg.Alpha0 = 0;					//Ĭ��͸���ȳ�������Χ��0��255��0Ϊ��ȫ͸��
  layer_cfg.Backcolor.Blue = 0;			//�㱳����ɫ��ɫ����
  layer_cfg.Backcolor.Green = 0;		//�㱳����ɫ��ɫ����
  layer_cfg.Backcolor.Red = 0;			//�㱳����ɫ��ɫ����
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;//����ϵ��1
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;//����ϵ��2
  layer_cfg.ImageWidth = LCD_GetXSize();//����ͼ�����
  layer_cfg.ImageHeight = LCD_GetYSize();//����ͼ��߶�
  
  HAL_LTDC_ConfigLayer(&Ltdc_Handler, &layer_cfg, LayerIndex); //����ѡ�еĲ����

  DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;//���ò�����屳����ɫ
  DrawProp[LayerIndex].pFont     = &LCD_DEFAULT_FONT;//���ò����������
  DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK; //���ò��������ɫ
  
  __HAL_LTDC_RELOAD_CONFIG(&Ltdc_Handler);//���ز�����ò���
}

/**
  * @brief  ѡ��LCD��
  * @param  LayerIndex: ǰ����(1)���߱�����(0)
  * @retval ��
  */
void LCD_SelectLayer(uint32_t LayerIndex)
{
  ActiveLayer = LayerIndex;
}

/**
  * @brief  ����LCD��Ŀ��ӻ�
  * @param  LayerIndex: ǰ����(1)���߱�����(0)
  * @param  State: ���ܻ���ʹ��
  * @retval ��
  */
void LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
  if(State == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&Ltdc_Handler, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&Ltdc_Handler, LayerIndex);
  }
  __HAL_LTDC_RELOAD_CONFIG(&Ltdc_Handler);
} 

/**
  * @brief  ����LCD��͸���ȳ���(���ڻ�ϣ�
  * @param  LayerIndex: ǰ����(1)���߱�����(0)
  * @param  Transparency: ͸���ȣ���Χ��0��255��0Ϊ��ȫ͸��
  * @retval ��
  */
void LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{    
  HAL_LTDC_SetAlpha(&Ltdc_Handler, Transparency, LayerIndex);
}

/**
  * @brief  ����LCD����֡���׵�ַ 
  * @param  LayerIndex: ǰ����(1)���߱�����(0)
  * @param  Address: LCD����֡���׵�ַ     
  * @retval ��
  */
void LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{
  HAL_LTDC_SetAddress(&Ltdc_Handler, Address, LayerIndex);
}

/**
  * @brief  ������ʾ����
  * @param  LayerIndex: ǰ����(��1)���߱�����(��0)
  * @param  Xpos: LCD 	X����ʼλ��
  * @param  Ypos: LCD 	Y����ʼλ��
  * @param  Width: LCD  ���ڴ�С
  * @param  Height: LCD ���ڴ�С  
  * @retval None
  */
void LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* �������ô��ڴ�С */
  HAL_LTDC_SetWindowSize(&Ltdc_Handler, Width, Height, LayerIndex);
  
  /* �������ô��ڵ���ʼλ�� */
  HAL_LTDC_SetWindowPosition(&Ltdc_Handler, Xpos, Ypos, LayerIndex); 
}

/**
  * @brief  ����LCD��ǰ��������ɫ
  * @param  Color: ������ɫ
  * @retval ��
  */
void LCD_SetTextColor(uint32_t Color)
{
  DrawProp[ActiveLayer].TextColor = Color;
}

/**
  * @brief  ��ȡLCD��ǰ��������ɫ
  * @retval ������ɫ
  */
uint32_t LCD_GetTextColor(void)
{
  return DrawProp[ActiveLayer].TextColor;
}

/**
  * @brief  ����LCD��ǰ������ֱ�����ɫ
  * @param  Color: ���ֱ�����ɫ
  * @retval ��
  */
void LCD_SetBackColor(uint32_t Color)
{
  DrawProp[ActiveLayer].BackColor = Color;
}

/**
  * @brief  ��ȡLCD��ǰ������ֱ�����ɫ
  * @retval ���ֱ�����ɫ
  */
uint32_t LCD_GetBackColor(void)
{
  return DrawProp[ActiveLayer].BackColor;
}


/**
 * @brief  ����LCD���ֵ���ɫ�ͱ�������ɫ
 * @param  TextColor: ָ��������ɫ
 * @param  BackColor: ָ��������ɫ
 * @retval ��
 */
void LCD_SetColors(uint32_t TextColor, uint32_t BackColor)
{
     LCD_SetTextColor (TextColor);
     LCD_SetBackColor (BackColor);
}
/**
  * @brief  ����LCD��ǰ����ʾ������
  * @param  fonts: ��������
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  DrawProp[ActiveLayer].pFont = fonts;
}

/**
  * @brief  ��ȡLCD��ǰ����ʾ������
  * @retval ��������
  */
sFONT *LCD_GetFont(void)
{
  return DrawProp[ActiveLayer].pFont;
}

/**
  * @brief  ��LCD������ֵ
  * @param  Xpos: X ������λ��
  * @param  Ypos: Y ������λ��
  * @retval RGB����ֵ
  */
uint32_t LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  uint32_t ret = 0;
  
  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    /* ��SDRAM�Դ��ж�ȡ��ɫ���� */
    ret = *(__IO uint32_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos)));
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    /* ��SDRAM�Դ��ж�ȡ��ɫ���� */
    ret  = (*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+2) & 0x00FFFFFF);
		ret |= (*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+1) & 0x00FFFFFF);
		ret |= (*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))) & 0x00FFFFFF);
  }
  else if((Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    /* ��SDRAM�Դ��ж�ȡ��ɫ���� */
    ret = *(__IO uint16_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));    
  }
  else
  {
    /* ��SDRAM�Դ��ж�ȡ��ɫ���� */
    ret = *(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + ((Ypos*LCD_GetXSize() + Xpos)));    
  }
  
  return ret;
}


/**
  * @brief  ���һ��������
  * @param  LayerIndex: ��ǰ��
  * @param  pDst: ָ��Ŀ�껺����ָ��
  * @param  xSize: ����������
  * @param  ySize: �������߶�
  * @param  OffLine: ƫ����
  * @param  ColorIndex: ��ǰ��ɫ
  * @retval None
  */
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex) 
{

  Dma2d_Handler.Init.Mode         = DMA2D_R2M;
  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_RGB565;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB8888;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_RGB888;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB1555;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444)
  { 
    Dma2d_Handler.Init.ColorMode    = DMA2D_ARGB4444;
  }
  Dma2d_Handler.Init.OutputOffset = OffLine;      
  
  Dma2d_Handler.Instance = DMA2D;
  
  /* DMA2D ��ʼ�� */
  if(HAL_DMA2D_Init(&Dma2d_Handler) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&Dma2d_Handler, LayerIndex) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&Dma2d_Handler, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* DMA��ѯ���� */  
        HAL_DMA2D_PollForTransfer(&Dma2d_Handler, 100);
      }
    }
  } 
}

/**
  * @brief  LCD��ǰ������
  * @param  Color: ������ɫ
  * @retval None
  */
void LCD_Clear(uint32_t Color)
{ 
  /* ���� */ 
  LL_FillBuffer(ActiveLayer, (uint32_t *)(Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress), LCD_GetXSize(), LCD_GetYSize(), 0, Color);
}


/**
  * @brief  ���һ��ʵ�ľ���
  * @param  Xpos: X����ֵ
  * @param  Ypos: Y����ֵ
  * @param  Width:  ���ο��� 
  * @param  Height: ���θ߶�
  * @retval ��
  */
void LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  uint32_t  x_address = 0;
  
  /* ����������ɫ */
  LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
  
  /* ���þ��ο�ʼ��ַ */
    if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    x_address = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    x_address = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 3*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if((Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    x_address = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }
  else
  {
    x_address = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + (LCD_GetXSize()*Ypos + Xpos);
  }	
  /* ������ */
  LL_FillBuffer(ActiveLayer, (uint32_t *)x_address, Width, Height, (LCD_GetXSize() - Width), DrawProp[ActiveLayer].TextColor);
}


/**
  * @brief  ����һ����
  * @param  Xpos:   X������
  * @param  Ypos:   Y������
  * @param  RGB_Code: ������ɫֵ
  * @retval ��
  */
void LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{

  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    *(__IO uint32_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos))) = RGB_Code;
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    *(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+2) = 0xFF&(RGB_Code>>16);
	*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))+1) = 0xFF&(RGB_Code>>8);
	*(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (3*(Ypos*LCD_GetXSize() + Xpos))) = 0xFF&RGB_Code;
  }
  else if((Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    *(__IO uint16_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos))) = (uint16_t)RGB_Code;   
  }
  else
  {
    *(__IO uint8_t*) (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress + ((Ypos*LCD_GetXSize() + Xpos))) = (uint8_t)RGB_Code;   
  }

}

/**
  * @brief  ��ʾһ���ַ�
  * @param  Xpos: ��ʾ�ַ�����λ��
  * @param  Ypos: ����ʼλ��
  * @param  c: ָ���������ݵ�ָ��
  * @retval ��
  */
static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t  offset;
  uint8_t  *pchar;
  uint32_t line;
  
  height = DrawProp[ActiveLayer].pFont->Height;//��ȡ����ʹ������߶�
  width  = DrawProp[ActiveLayer].pFont->Width; //��ȡ����ʹ���������
  
  offset =  8 *((width + 7)/8) -  width ;//�����ַ���ÿһ�����ص�ƫ��ֵ��ʵ�ʴ洢��С-������ʾ����
  
  for(i = 0; i < height; i++)//��������߶Ȼ��
  {
    pchar = ((uint8_t *)c + (width + 7)/8 * i);//�����ַ���ÿһ�����ص�ƫ�Ƶ�ַ
    
    switch(((width + 7)/8))//���������������ȡ��ͬ�����ʵ������ֵ
    {
      
    case 1:
      line =  pchar[0];      //��ȡ�������С�ڻ����8���ַ�������ֵ
      break;
      
    case 2:
      line =  (pchar[0]<< 8) | pchar[1]; //��ȡ������ȴ���8С�ڻ����16���ַ�������ֵ     
      break;
      
    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2]; //��ȡ������ȴ���16С�ڻ����24���ַ�������ֵ     
      break;
    } 
    
    for (j = 0; j < width; j++)//����������Ȼ��
    {
      if(line & (1 << (width - j + offset- 1))) //����ÿһ�е�����ֵ��ƫ��λ�ð��յ�ǰ������ɫ���л��
      {
        LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].TextColor);
      }
      else//�����һ��û�������������ձ�����ɫ���
      {
        LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].BackColor);
      } 
    }
    Ypos++;
  }
}
/**
  * @brief  ���һ��
  * @param  Line: ��
  * @retval None
  */
void LCD_ClearLine(uint32_t Line)
{
  uint32_t color_backup = DrawProp[ActiveLayer].TextColor;
  DrawProp[ActiveLayer].TextColor = DrawProp[ActiveLayer].BackColor;
  
  /* ��һ��������ɫһ�µľ��������� */
  LCD_FillRect(0, (Line * DrawProp[ActiveLayer].pFont->Height), LCD_GetXSize(), DrawProp[ActiveLayer].pFont->Height);
  
  DrawProp[ActiveLayer].TextColor = color_backup;
  LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);  
}

/**
  * @brief  ��ʾһ���ַ�
  * @param  Xpos: X����ʼ����
  * @param  Ypos: Y����ʼ����
  * @param  Ascii: �ַ� ascii ��,��Χ�� 0x20 ��0x7E ��
  * @retval ��
  */
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
    DrawChar(Xpos, Ypos, &DrawProp[ActiveLayer].pFont->table[(Ascii-' ') *\
    DrawProp[ActiveLayer].pFont->Height * ((DrawProp[ActiveLayer].pFont->Width + 7) / 8)]);
}

/**
  * @brief  ʹ����ʾ
  * @retval ��
  */
void LCD_DisplayOn(void)
{
  /* ����ʾ */
  __HAL_LTDC_ENABLE(&Ltdc_Handler);
  HAL_GPIO_WritePin(LTDC_DISP_GPIO_PORT, LTDC_DISP_GPIO_PIN, GPIO_PIN_SET);/* LCD_DISPʹ��*/
  HAL_GPIO_WritePin(LTDC_BL_GPIO_PORT, LTDC_BL_GPIO_PIN, GPIO_PIN_SET);  /* ������*/
}

/**
  * @brief  ��ʾ�ַ���
  * @param  Xpos: X����ʼ����
  * @param  Ypos: Y����ʼ���� 
  * @param  Text: �ַ���ָ��
  * @param  Mode: ��ʾ���뷽ʽ��������CENTER_MODE��RIGHT_MODE��LEFT_MODE
  * @retval None
  */
void LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
  uint16_t ref_column = 1, i = 0;
  uint32_t size = 0, xsize = 0; 
  uint8_t  *ptr = Text;
  
  /* ��ȡ�ַ�����С */
  while (*ptr++) size ++ ;
  
  /* ÿһ�п�����ʾ�ַ������� */
  xsize = (LCD_GetXSize()/DrawProp[ActiveLayer].pFont->Width);
  
  switch (Mode)
  {
		case CENTER_MODE:
    {
      ref_column = Xpos + ((xsize - size)* DrawProp[ActiveLayer].pFont->Width) / 2;
      break;
    }
		case LEFT_MODE:
    {
      ref_column = Xpos;
      break;
    }
		case RIGHT_MODE:
    {
      ref_column = - Xpos + ((xsize - size)*DrawProp[ActiveLayer].pFont->Width);
      break;
    }    
		default:
    {
      ref_column = Xpos;
      break;
    }
  }
  
  /*�����ʼ���Ƿ�����ʾ��Χ�� */
  if ((ref_column < 1) || (ref_column >= 0x8000))
  {
    ref_column = 1;
  }

  /* ʹ���ַ���ʾ������ʾÿһ���ַ�*/
  while ((*Text != 0) & (((LCD_GetXSize() - (i*DrawProp[ActiveLayer].pFont->Width)) & 0xFFFF)\
			>= DrawProp[ActiveLayer].pFont->Width))
  {
    /* ��ʾһ���ַ� */
    LCD_DisplayChar(ref_column, Ypos, *Text);
    /* ���������С������һ��ƫ��λ�� */
    ref_column += DrawProp[ActiveLayer].pFont->Width;
    /* ָ��ָ����һ���ַ� */
    Text++;
    i++;
  }  
}

/**
  * @brief  ��ָ������ʾ�ַ���(���60��)
  * @param  Line: ��ʾ����
  * @param  ptr: �ַ���ָ��
  * @retval ��
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{  
  LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}

/**
  * @brief  ָ�����㻭һ����
  * @param  x1: ��һ��X������
  * @param  y1: ��һ��Y������
  * @param  x2: �ڶ���X������
  * @param  y2: �ڶ���Y������
  * @retval ��
  */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* ��x��ľ���ֵ */
  deltay = ABS(y2 - y1);        /* ��y��ľ���ֵ */
  x = x1;                       /* ��һ�����ص�x������ʼֵ */
  y = y1;                       /* ��һ�����ص�y������ʼֵ */
  
  if (x2 >= x1)                 /* x����ֵΪ���� */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* x����ֵΪ�ݼ� */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* y����ֵΪ���� */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* y����ֵΪ�ݼ� */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* ÿ�� y ����ֵ������һ��x����ֵ*/
  {
    xinc1 = 0;                  /* �����Ӵ��ڻ���ڷ�ĸʱ��Ҫ�ı� x */
    yinc2 = 0;                  /* ��ҪΪÿ�ε������� y */
    den = deltax;
    num = deltax / 2;
    num_add = deltay;
    num_pixels = deltax;         /* x��y���ֵ */
  }
  else                          /* ÿ�� x ����ֵ������һ��y����ֵ */
  {
    xinc2 = 0;                  /* ��ҪΪÿ�ε������� x */
    yinc1 = 0;                  /* �����Ӵ��ڻ���ڷ�ĸʱ��Ҫ�ı� y */
    den = deltay;
    num = deltay / 2;
    num_add = deltax;
    num_pixels = deltay;         /* y��x���ֵ */
  }
  
  for (curpixel = 0; curpixel <= num_pixels; curpixel++)
  {
    LCD_DrawPixel(x, y, DrawProp[ActiveLayer].TextColor);   /* ���Ƶ�ǰ���ص� */
    num += num_add;                            /* �ڷ����Ļ��������ӷ��� */
    if (num >= den)                           /* �����Ӵ��ڻ���ڷ�ĸ */
    {
      num -= den;                             /* �����µķ���ֵ */
      x += xinc1;                             /* xֵ���� */
      y += yinc1;                             /* yֵ���� */
    }
    x += xinc2;                               /* yֵ���� */
    y += yinc2;                               /* yֵ���� */
  }
}


/**
  * @brief  ����ˮƽ��
  * @param  Xpos: X����ʼ����
  * @param  Ypos: Y����ʼ����
  * @param  Length: �ߵĳ���
  * @retval ��
  */
void LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t  Xaddress = 0;

  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    Xaddress = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    Xaddress = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 3*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if((Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    Xaddress = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }
  else
  {
    Xaddress = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }	
  /* ������� */
  LL_FillBuffer(ActiveLayer, (uint32_t *)Xaddress, Length, 1, 0, DrawProp[ActiveLayer].TextColor);
}


/**
  * @brief  ���ƴ�ֱ��
  * @param  Xpos: X����ʼ����
  * @param  Ypos: Y����ʼ����
  * @param  Length: �ߵĳ���
  * @retval ��
  */
void LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint32_t  Xaddress = 0;
  
  if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    Xaddress = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if(Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    Xaddress = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 3*(LCD_GetXSize()*Ypos + Xpos);
  }
  else if((Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (Ltdc_Handler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))  
  {
    Xaddress = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }
  else
  {
    Xaddress = (Ltdc_Handler.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);   
  }	
  
  /* ������� */
  LL_FillBuffer(ActiveLayer, (uint32_t *)Xaddress, 1, Length, (LCD_GetXSize() - 1), DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  ����һ������
  * @param  Xpos:   X������
  * @param  Ypos:   Y������
  * @param  Width:  ���ο���  
  * @param  Height: ���θ߶�
  * @retval ��
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* ����ˮƽ�� */
  LCD_DrawHLine(Xpos, Ypos, Width);
  LCD_DrawHLine(Xpos, (Ypos+ Height), Width);
  
  /* ���ƴ�ֱ�� */
  LCD_DrawVLine(Xpos, Ypos, Height);
  LCD_DrawVLine((Xpos + Width), Ypos, Height);
}

/**
  * @brief  ����һ��Բ��
  * @param  Xpos:   X������
  * @param  Ypos:   Y������
  * @param  Radius: Բ�İ뾶
  * @retval ��
  */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t   decision;    /* ���߱��� */ 
  uint32_t  current_x;   /* ��ǰx����ֵ */
  uint32_t  current_y;   /* ��ǰy����ֵ */
  
  decision = 3 - (Radius << 1);
  current_x = 0;
  current_y = Radius;
  
  while (current_x <= current_y)
  {
    LCD_DrawPixel((Xpos + current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos - current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos + current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos - current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos + current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos - current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos + current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);
    
    LCD_DrawPixel((Xpos - current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);
    
    if (decision < 0)
    { 
      decision += (current_x << 2) + 6;
    }
    else
    {
      decision += ((current_x - current_y) << 2) + 10;
      current_y--;
    }
    current_x++;
  } 
}

/**
  * @brief  ���һ��ʵ��Բ
  * @param  Xpos: X����ֵ
  * @param  Ypos: Y����ֵ
  * @param  Radius: Բ�İ뾶
  * @retval ��
  */
void LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t   decision;    /* ���߱��� */ 
  uint32_t  current_x;   /* ��ǰx����ֵ */
  uint32_t  current_y;   /* ��ǰy����ֵ */
  
  decision = 3 - (Radius << 1);
  
  current_x = 0;
  current_y = Radius;
  
  LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
  
  while (current_x <= current_y)
  {
    if(current_y > 0) 
    {
      LCD_DrawHLine(Xpos - current_y, Ypos + current_x, 2*current_y);
      LCD_DrawHLine(Xpos - current_y, Ypos - current_x, 2*current_y);
    }
    
    if(current_x > 0) 
    {
      LCD_DrawHLine(Xpos - current_x, Ypos - current_y, 2*current_x);
      LCD_DrawHLine(Xpos - current_x, Ypos + current_y, 2*current_x);
    }
    if (decision < 0)
    { 
      decision += (current_x << 2) + 6;
    }
    else
    {
      decision += ((current_x - current_y) << 2) + 10;
      current_y--;
    }
    current_x++;
  }
  
  LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
  LCD_DrawCircle(Xpos, Ypos, Radius);
}

///////////////////////����Ϊ������ʾ����//////////////////////////////////////
#include "spi_flash.h"


/*ʹ��FLASH��ģ*/

//�����ֿ�洢��FLASH����ʼ��ַ ��
//GBKCODE_START_ADDRESS ��fonts.h�ļ�����
/**
  * @brief  ��ȡFLASH������ʾ�ֿ�����
	* @param  pBuffer:�洢�ֿ����Ļ�����
	* @param  c �� Ҫ��ȡ������
  * @retval None.
  */
int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
{ 
	unsigned char High8bit,Low8bit;
	unsigned int pos;

	static uint8_t everRead=0;

	/*��һ��ʹ�ã���ʼ��FLASH*/
	if(everRead == 0)
	{
		SPI_FLASH_Init();
		everRead = 1;
	}

	High8bit= c >> 8;     /* ȡ��8λ���� */
	Low8bit= c & 0x00FF;  /* ȡ��8λ���� */		

	/*GB2312 ��ʽ:
	��һ���ֽ�Ϊ�����ֽڡ�����Ӧ 94 ������ÿ������ 94 ��λ
	*/
	pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*24*24/8;
	SPI_FLASH_BufferRead(pBuffer,GBKCODE_START_ADDRESS+pos,24*24/8); //��ȡ�ֿ�����  
	//	  printf ( "%02x %02x %02x %02x\n", pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3]);

	return 0;  

}

/**
 * @brief  ����ʾ������ʾһ�������ַ�
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  usChar ��Ҫ��ʾ�������ַ��������룩
 * @retval ��
 */ 
static void LCD_DispChar_CN (uint16_t Xpos, uint16_t Ypos, uint16_t Text)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t  offset;
  uint8_t  *pchar;
  uint8_t  Buffer[HEIGHT_CN_CHAR*3];
  uint32_t line;
	
  GetGBKCode (Buffer, Text );
  
  height = 	HEIGHT_CN_CHAR;//ȡ��ģ����//��ȡ����ʹ������߶�
  width  =  WIDTH_CN_CHAR; //��ȡ����ʹ���������
  
  offset =  8 *((width + 7)/8) -  width ;//�����ַ���ÿһ�����ص�ƫ��ֵ(��Ч���ص��������ʵ�ʴ洢��С-������ʾ����
  
  for(i = 0; i < height; i++)//��������߶Ȼ��
  {
    pchar = ((uint8_t *)Buffer + (width + 7)/8 * i);//�����ַ���ÿһ�����ص�ƫ�Ƶ�ַ
    
    switch(((width + 7)/8))//���������������ȡ��ͬ�����ʵ������ֵ
    {
      
    case 1:
      line =  pchar[0];      //��ȡ�������С�ڵ���8���ַ�������ֵ
      break;
      
    case 2:
      line =  (pchar[0]<< 8) | pchar[1]; //��ȡ������ȴ���8С�ڵ���16���ַ�������ֵ     
      break;
      
    case 3:
    default:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2]; //��ȡ������ȴ���16С�ڵ���24���ַ�������ֵ     
      break;
    } 
    
    for (j = 0; j < width; j++)//����������Ȼ��
    {
      if(line & (1 << (width- j + offset- 1))) //����ÿһ�е�����ֵ��ƫ��λ�ð��յ�ǰ������ɫ���л��
      {
        LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].TextColor);
      }
      else//�����һ��û�������������ձ�����ɫ���
      {
        LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].BackColor);
      } 
    }
    Ypos++;
  }
}

/**
  * @brief  ��ʾһ���ַ���������Һ�����ȣ����Զ����С�
			��Ӣ����ʱ�����Ӣ����������ΪFont24��ʽ
  * @param  Line: Ҫ��ʾ���б��LINE(0) - LINE(N)
  * @param  *ptr: Ҫ��ʾ���ַ���ָ��
  * @retval None
  */
void LCD_DisplayStringLine_EN_CN(uint16_t Line, uint8_t *ptr)
{  
  uint16_t refcolumn = 0;
  /* �ж���ʾλ�ò��ܳ���Һ���ı߽� */
  while ((refcolumn < LCD_PIXEL_WIDTH) && ((*ptr != 0) & (((refcolumn + DrawProp[ActiveLayer].pFont->Width) & 0xFFFF) >= DrawProp[ActiveLayer].pFont->Width)))
  {
	/* ʹ��LCD��ʾһ���ַ� */
	if ( * ptr <= 126 )	           	//Ӣ���ַ�
	{
				
		LCD_DisplayChar(refcolumn, LINE(Line), *ptr);
		/* ��������ƫ����ʾ��λ�� */
		refcolumn += DrawProp[ActiveLayer].pFont->Width;
		/* ָ���ַ����е���һ���ַ� */
		ptr++;
	}
	/*�� 2 ������ 127 ���ַ�����һ��ʱ���ͱ�ʾ 1 ������
	�� 1 ���ֽ�ʹ�� (0xA1-0xFE) ���룬�� 2 ���ֽ�ʹ��(0xA1-0xFE)����
	*/
	else	                            //�����ַ�
	{	
		uint16_t usCh;
		
		/*һ���������ֽ�*/
		usCh = * ( uint16_t * ) ptr;	
		/*��������˳��*/
		usCh = ( usCh << 8 ) + ( usCh >> 8 );		 //С��ת��
		
		/*��ʾ����*/
		LCD_DispChar_CN ( refcolumn,LINE(Line) , usCh );
		/*��ʾλ��ƫ��*/
		refcolumn += WIDTH_CN_CHAR;
		/* ָ���ַ����е���һ���ַ� */
		ptr += 2; 		
    }		
  }
}

////////////////����Ϊ�������Ź���/////////////////////////
uint8_t zoomBuff[ZOOMMAXBUFF] = {0};	//�������ŵĻ��棬���֧�ֵ�128*128
/**
 * @brief  ������ģ�����ź����ģ��1�����ص���8������λ����ʾ
										0x01��ʾ�ʼ���0x00��ʾ�հ���
 * @param  in_width ��ԭʼ�ַ�����
 * @param  in_heig ��ԭʼ�ַ��߶�
 * @param  out_width �����ź���ַ�����
 * @param  out_heig�����ź���ַ��߶�
 * @param  in_ptr ���ֿ�����ָ��	ע�⣺1pixel 1bit
 * @param  out_ptr �����ź���ַ����ָ�� ע��: 1pixel 8bit
 *		out_ptrʵ����û������������ĳ���ֱ�������ȫ��ָ��zoomBuff��
 * @param  en_cn ��0ΪӢ�ģ�1Ϊ����
 * @retval ��
 */
void LCD_zoomChar(	uint16_t in_width,	//ԭʼ�ַ�����
					uint16_t in_heig,		//ԭʼ�ַ��߶�
					uint16_t out_width,	//���ź���ַ�����
					uint16_t out_heig,	//���ź���ַ��߶�
					uint8_t *in_ptr,	//�ֿ�����ָ��	ע�⣺1pixel 1bit
					uint8_t *out_ptr, //���ź���ַ����ָ�� ע��: 1pixel 8bit
					uint8_t en_cn)		//0ΪӢ�ģ�1Ϊ����	
{
	uint8_t *pts,*ots;
	//����Դ��ģ��Ŀ����ģ��С���趨����������ӣ�����16��Ϊ�˰Ѹ�������ת�ɶ�������
	unsigned int xrIntFloat_16=(in_width<<16)/out_width+1; 
    unsigned int yrIntFloat_16=(in_heig<<16)/out_heig+1;
	
	unsigned int srcy_16=0;
	unsigned int y,x;
	uint8_t *pSrcLine;
	uint8_t tempBuff[1024] = {0};
	uint32_t  uChar;
	
	//�������Ƿ�Ϸ�
	if(in_width >= 32) return;												//�ֿⲻ��������32����
	if(in_width * in_heig == 0) return;	
	if(in_width * in_heig >= 1024 ) return; 					//����������� 32*32
	
	if(out_width * out_heig == 0) return;	
	if(out_width * out_heig >= ZOOMMAXBUFF ) return; //����������� 128*128
	pts = (uint8_t*)&tempBuff;
  
	//Ϊ�������㣬�ֿ��������1 pixel 1bit ӳ�䵽1pixel 8bit
	//0x01��ʾ�ʼ���0x00��ʾ�հ���
	if(en_cn == 0x00)//Ӣ��
	{
		//������17 * 24�ֿ���Ϊ���ԣ�ÿһ�������ֽڱ�ʾ
		//Ӣ�ĺ������ֿ����±߽粻�ԣ����ڴ˴ε�������Ҫע��tempBuff��ֹ���
		pts+=in_width*4;		
	}
	
	for(y=0;y<in_heig;y++)	
	{
		/*Դ��ģ����*/
		uChar = in_ptr [ y * 3 ];
		uChar = ( uChar << 8 );
		uChar |= in_ptr [ y * 3 + 1 ];
		uChar = ( uChar << 8 );
		uChar |= in_ptr [ y * 3 + 2];
		/*ӳ��*/
		for(x=0;x<in_width;x++)
		{
			if(((uChar << x) & 0x800000) == 0x800000)
				*pts++ = 0x01;
			else
				*pts++ = 0x00;
		}
	}		


	//zoom����
	pts = (uint8_t*)&tempBuff;	//ӳ����Դ����ָ��
	ots = (uint8_t*)&zoomBuff;	//������ݵ�ָ��
	for (y=0;y<out_heig;y++)	/*�б���*/
	{
		unsigned int srcx_16=0;
			pSrcLine=pts+in_width*(srcy_16>>16);				
			for (x=0;x<out_width;x++) /*�������ر���*/
			{
					ots[x]=pSrcLine[srcx_16>>16]; //��Դ��ģ���ݸ��Ƶ�Ŀ��ָ����
					srcx_16+=xrIntFloat_16;			//������ƫ��Դ���ص�
			}
			srcy_16+=yrIntFloat_16;				  //������ƫ��Դ���ص�
			ots+=out_width;						
	}

}


/**
 * @brief  �������ź����ģ��ʾ�ַ�
 * @param  Xpos ���ַ���ʾλ��x
 * @param  Ypos ���ַ���ʾλ��y
 * @param  Font_width ���ַ�����
 * @param  Font_Heig���ַ��߶�
 * @param  c ��Ҫ��ʾ����ģ����
 * @param  DrawModel ���Ƿ�ɫ��ʾ 
 * @retval ��
 */
void LCD_DrawChar_Ex(   uint16_t Xpos, //�ַ���ʾλ��x
						uint16_t Ypos, //�ַ���ʾλ��y
						uint16_t Font_width, //�ַ�����
						uint16_t Font_Heig,  //�ַ��߶� 
						uint8_t *c,			//��ģ����
						uint16_t DrawModel)	//�Ƿ�ɫ��ʾ
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  
  height = 	Font_Heig;//ȡ��ģ����//��ȡ����ʹ������߶�
  width  =  Font_width; //��ȡ����ʹ���������
  
  for(i = 0; i < height; i++)//��������߶Ȼ��
  {
    for (j = 0; j < width; j++)//����������Ȼ��
    {
      if(*c++ != DrawModel) //ÿһ���ֽڱ�ʾһ�����أ����������ʾ
      {
        LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].TextColor);
      }
      else
      {
        LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].BackColor);
      } 
    }
    Ypos++;
  }
}

/**
 * @brief  �������ź����ģ��ʾ�ַ���
 * @param  Xpos ���ַ���ʾλ��x
 * @param  Ypos ���ַ���ʾλ��y
 * @param  Font_width ���ַ����ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
 * @param  Font_Heig���ַ��߶ȣ�ע��Ϊż��
 * @param  c ��Ҫ��ʾ���ַ���
 * @param  DrawModel ���Ƿ�ɫ��ʾ 
 * @retval ��
 */
void LCD_DisplayStringLineEx(uint16_t x, 		//�ַ���ʾλ��x
							 uint16_t y, 				//�ַ���ʾλ��y
							 uint16_t Font_width,	//Ҫ��ʾ��������ȣ�Ӣ���ַ��ڴ˻�����/2��ע��Ϊż��
							 uint16_t Font_Heig,	//Ҫ��ʾ������߶ȣ�ע��Ϊż��
							 uint8_t *ptr,					//��ʾ���ַ�����
							 uint16_t DrawModel)  //�Ƿ�ɫ��ʾ
{
	uint16_t refcolumn = x; //x����
	uint16_t Charwidth;
	uint8_t *psr;
	uint8_t Ascii;	//Ӣ��
	uint16_t usCh;  //����
	uint8_t ucBuffer [ 24*24/8 ];	
	
	while ((refcolumn < LCD_PIXEL_WIDTH) && ((*ptr != 0) & (((refcolumn + DrawProp[ActiveLayer].pFont->Width) & 0xFFFF) >= DrawProp[ActiveLayer].pFont->Width)))
	{
		if(*ptr > 0x80) //���������
		{
			Charwidth = Font_width;
			usCh = * ( uint16_t * ) ptr;				
			usCh = ( usCh << 8 ) + ( usCh >> 8 );
			GetGBKCode ( ucBuffer, usCh );	//ȡ��ģ����
			//������ģ����
			LCD_zoomChar(24,24,Charwidth,Font_Heig,(uint8_t *)&ucBuffer,psr,1); 
			//��ʾ�����ַ�
			LCD_DrawChar_Ex(refcolumn,y,Charwidth,Font_Heig,(uint8_t*)&zoomBuff,DrawModel);
			refcolumn+=Charwidth;
			ptr+=2;
		}
		else
		{
			Charwidth = Font_width / 2;
			Ascii = *ptr - 32;
			//������ģ����
			LCD_zoomChar(17,24,Charwidth,Font_Heig,(uint8_t *)&DrawProp[ActiveLayer].pFont->table[Ascii*\
						DrawProp[ActiveLayer].pFont->Height * ((DrawProp[ActiveLayer].pFont->Width + 7) / 8)],psr,0);
		   //��ʾ�����ַ�
			LCD_DrawChar_Ex(refcolumn,y,Charwidth,Font_Heig,(uint8_t*)&zoomBuff,DrawModel);
			refcolumn+=Charwidth;
			ptr++;
		}
	}
}
