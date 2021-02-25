#ifndef __ADC_H
#define __ADC_H

#include "stm32f4xx_hal.h"

#define RHEOSTAT_ADC_DR_ADDR  ((uint32_t)ADC1+0x4c)



void Rheostat_Init(void);

extern __IO uint16_t ADC_ConvertedValue[];

#endif