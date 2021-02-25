#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx_hal.h"

void tim6_init();
void tim8_init();

extern TIM_HandleTypeDef TIM8_TimeBaseStructure;
extern uint32_t IC1Value, IC2Value;
extern float DutyCycle, Frequency;
#endif