#ifndef __BEEPER_H
#define __BEEPER_H


#include "stm32f4xx_hal.h"




#define BEEPER_ON   HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_SET)
#define BEEPER_OFF  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_RESET)


void beeperInit();


extern uint8_t shortBeep;
extern uint8_t shortBeep_cnt;



#endif