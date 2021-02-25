#ifndef __LED_H
#define __LED_H

#include "stm32f4xx_hal.h"


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED_R_TOGGLE		digitalToggle(GPIOH, GPIO_PIN_10)
#define LED_R_OFF		digitalHi(GPIOH, GPIO_PIN_10)
#define LED_R_ON			digitalLo(GPIOH, GPIO_PIN_10)

#define LED_G_TOGGLE		digitalToggle(GPIOH, GPIO_PIN_11)
#define LED_G_OFF		digitalHi(GPIOH, GPIO_PIN_11)
#define LED_G_ON			digitalLo(GPIOH, GPIO_PIN_11)

#define LED_B_TOGGLE		digitalToggle(GPIOH, GPIO_PIN_12)
#define LED_B_OFF		digitalHi(GPIOH, GPIO_PIN_12)
#define LED_B_ON			digitalLo(GPIOH, GPIO_PIN_12)


//红
#define LED_RED  \
					{LED_R_ON\
					LED_G_OFF\
					LED_B_OFF}

//绿
#define LED_GREEN		\
					{LED_R_OFF\
					LED_G_ON\
					LED_B_OFF}

//蓝
#define LED_BLUE	\
					{LED_R_OFF\
					LED_G_OFF\
					LED_B_ON}

					
//黄(红+绿)					
#define LED_YELLOW	\
					{LED_R_ON\
					LED_G_ON\
					LED_B_OFF}
//紫(红+蓝)
#define LED_PURPLE	\
					LED_R_ON\
					LED_G_OFF\
					LED_B_ON

//青(绿+蓝)
#define LED_CYAN \
					LED_R_OFF\
					LED_G_ON\
					LED_B_ON
					
//白(红+绿+蓝)
#define LED_WHITE	\
					LED_R_ON\
					LED_G_ON\
					LED_B_ON
					
#define LED_RGBOFF \
						{LED_R_OFF\
					LED_G_OFF\
					LED_B_OFF}

void led_init(void);

#endif
