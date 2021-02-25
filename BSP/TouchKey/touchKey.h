#ifndef __TOUCHKEY_H
#define __TOUCHKEY_H

#include "stm32f4xx_hal.h"
#include <stdio.h>


#define TPAD_ARR_MAX_VAL 0XFFFF
#define TPAD_GATE_VAL 100

uint8_t TPAD_Init(void);
uint8_t TPAD_Scan();



#endif