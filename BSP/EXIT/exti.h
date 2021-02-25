#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

#define key1_irqHandler                   EXTI0_IRQHandler
#define key2_irqHandler                   EXTI15_10_IRQHandler

void extiInit();

#endif