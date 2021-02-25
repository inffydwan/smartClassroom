#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx_hal.h"

void keyInit(void);
void keyScan(void);

enum {
	NONE,
	KEY1_SHORT,
	KEY2_SHORT,
	TOUCH_KEY
};

extern uint8_t keyState;

extern uint8_t delay;
extern uint8_t delay_cnt;


#endif
