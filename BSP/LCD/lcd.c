#include "lcd.h"
#include "font24.c"
#include "font20.c"
#include "font16.c"
#include "font12.c"
#include "font8.c"

/*
LTDC ���𲻶�ˢ��Һ���������ϰ��Դ�ռ�����ݴ��䵽Һ����������ʾ����
DMA2D ����ͼ�����ݰ��ˡ���ϼ���ʽת����������õ����ݴ����Դ棩��RAM �洢����Ϊ�Դ档
�����Դ����ʹ�� STM32 оƬ�ڲ��� SRAM ������ SDRAM/SRAM��ֻҪ�����㹻�󼴿�(����Ҫ�ܴ洢һ֡ͼ������)��
*/

LTDC_HandleTypeDef  Ltdc_Handler;
static DMA2D_HandleTypeDef Dma2d_Handler;


/* Default LCD configuration with LCD Layer 1 (backGround)*/
static uint32_t            ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];

LCD_TypeDef cur_lcd = INCH_5;

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

  HAL_GPIO_WritePin(LTDC_BL_GPIO_PORT, LTDC_BL_GPIO_PIN, GPIO_PIN_SET);  /* ������*/
  
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
    /* ������ͬ���źſ��(HSW-1) */
    Ltdc_Handler.Init.HorizontalSync =HSW-1;
    /* ���ô�ֱͬ���źſ��(VSW-1) */
    Ltdc_Handler.Init.VerticalSync = VSW-1;
    /* ����(HSW+HBP-1) */
    Ltdc_Handler.Init.AccumulatedHBP = HSW+HBP-1;
    /* ����(VSW+VBP-1) */
    Ltdc_Handler.Init.AccumulatedVBP = VSW+VBP-1;
    /* ����(HSW+HBP+��Ч���ؿ��-1) */
    Ltdc_Handler.Init.AccumulatedActiveW = HSW+HBP+LCD_PIXEL_WIDTH-1;
    /* ����(VSW+VBP+��Ч���ظ߶�-1) */
    Ltdc_Handler.Init.AccumulatedActiveH = VSW+VBP+LCD_PIXEL_HEIGHT-1;
    /* �����ܿ��(HSW+HBP+��Ч���ؿ��+HFP-1) */
    Ltdc_Handler.Init.TotalWidth =HSW+ HBP+LCD_PIXEL_WIDTH + HFP-1; 
    /* �����ܸ߶�(VSW+VBP+��Ч���ظ߶�+VFP-1) */
    Ltdc_Handler.Init.TotalHeigh =VSW+ VBP+LCD_PIXEL_HEIGHT + VFP-1;
		
		HAL_LTDC_Init(&Ltdc_Handler);

}


/**
  * @brief  Initializes the LTDC Layer according to the specified parameters
  *         in the LTDC_LayerStruct.
  * @note   This function can be used only when the LTDC is disabled.
  * @param  LTDC_layerx: Select the layer to be configured, this parameter can be 
  *         one of the following values: LTDC_Layer1, LTDC_Layer2    
  * @param  LTDC_LayerStruct: pointer to a LTDC_LayerTypeDef structure that contains
  *         the configuration information for the specified LTDC peripheral.
  * @retval None
  */

void LTDC_LayerInit(LTDC_Layer_TypeDef* LTDC_Layerx, LTDC_Layer_InitTypeDef* LTDC_Layer_InitStruct)
{

  uint32_t whsppos = 0;
  uint32_t wvsppos = 0;
  uint32_t dcgreen = 0;
  uint32_t dcred = 0;
  uint32_t dcalpha = 0;
  uint32_t cfbp = 0;

/* Check the parameters */
  assert_param(IS_LTDC_Pixelformat(LTDC_Layer_InitStruct->LTDC_PixelFormat));
  assert_param(IS_LTDC_BlendingFactor1(LTDC_Layer_InitStruct->LTDC_BlendingFactor_1));
  assert_param(IS_LTDC_BlendingFactor2(LTDC_Layer_InitStruct->LTDC_BlendingFactor_2));
  assert_param(IS_LTDC_HCONFIGST(LTDC_Layer_InitStruct->LTDC_HorizontalStart));
  assert_param(IS_LTDC_HCONFIGSP(LTDC_Layer_InitStruct->LTDC_HorizontalStop));
  assert_param(IS_LTDC_VCONFIGST(LTDC_Layer_InitStruct->LTDC_VerticalStart));
  assert_param(IS_LTDC_VCONFIGSP(LTDC_Layer_InitStruct->LTDC_VerticalStop));  
  assert_param(IS_LTDC_DEFAULTCOLOR(LTDC_Layer_InitStruct->LTDC_DefaultColorBlue));
  assert_param(IS_LTDC_DEFAULTCOLOR(LTDC_Layer_InitStruct->LTDC_DefaultColorGreen));
  assert_param(IS_LTDC_DEFAULTCOLOR(LTDC_Layer_InitStruct->LTDC_DefaultColorRed));
  assert_param(IS_LTDC_DEFAULTCOLOR(LTDC_Layer_InitStruct->LTDC_DefaultColorAlpha));
  assert_param(IS_LTDC_CFBP(LTDC_Layer_InitStruct->LTDC_CFBPitch));
  assert_param(IS_LTDC_CFBLL(LTDC_Layer_InitStruct->LTDC_CFBLineLength));
  assert_param(IS_LTDC_CFBLNBR(LTDC_Layer_InitStruct->LTDC_CFBLineNumber));

  /* Configures the horizontal start and stop position */
  whsppos = LTDC_Layer_InitStruct->LTDC_HorizontalStop << 16;
  LTDC_Layerx->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  LTDC_Layerx->WHPCR = (LTDC_Layer_InitStruct->LTDC_HorizontalStart | whsppos);

  /* Configures the vertical start and stop position */
  wvsppos = LTDC_Layer_InitStruct->LTDC_VerticalStop << 16;
  LTDC_Layerx->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  LTDC_Layerx->WVPCR  = (LTDC_Layer_InitStruct->LTDC_VerticalStart | wvsppos);

  /* Specifies the pixel format */
  LTDC_Layerx->PFCR &= ~(LTDC_LxPFCR_PF);
  LTDC_Layerx->PFCR = (LTDC_Layer_InitStruct->LTDC_PixelFormat);

  /* Configures the default color values */
  dcgreen = (LTDC_Layer_InitStruct->LTDC_DefaultColorGreen << 8);
  dcred = (LTDC_Layer_InitStruct->LTDC_DefaultColorRed << 16);
  dcalpha = (LTDC_Layer_InitStruct->LTDC_DefaultColorAlpha << 24);
  LTDC_Layerx->DCCR &=  ~(LTDC_LxDCCR_DCBLUE | LTDC_LxDCCR_DCGREEN | LTDC_LxDCCR_DCRED | LTDC_LxDCCR_DCALPHA);
  LTDC_Layerx->DCCR = (LTDC_Layer_InitStruct->LTDC_DefaultColorBlue | dcgreen | \
                        dcred | dcalpha);

  /* Specifies the constant alpha value */      
  LTDC_Layerx->CACR &= ~(LTDC_LxCACR_CONSTA);
  LTDC_Layerx->CACR = (LTDC_Layer_InitStruct->LTDC_ConstantAlpha);

  /* Specifies the blending factors */
  LTDC_Layerx->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);
  LTDC_Layerx->BFCR = (LTDC_Layer_InitStruct->LTDC_BlendingFactor_1 | LTDC_Layer_InitStruct->LTDC_BlendingFactor_2);

  /* Configures the color frame buffer start address */
  LTDC_Layerx->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
  LTDC_Layerx->CFBAR = (LTDC_Layer_InitStruct->LTDC_CFBStartAdress);

  /* Configures the color frame buffer pitch in byte */
  cfbp = (LTDC_Layer_InitStruct->LTDC_CFBPitch << 16);
  LTDC_Layerx->CFBLR  &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);
  LTDC_Layerx->CFBLR  = (LTDC_Layer_InitStruct->LTDC_CFBLineLength | cfbp);

  /* Configures the frame buffer line number */
  LTDC_Layerx->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
  LTDC_Layerx->CFBLNR  = (LTDC_Layer_InitStruct->LTDC_CFBLineNumber);

}