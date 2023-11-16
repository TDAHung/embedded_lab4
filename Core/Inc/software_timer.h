/*
 * software_timer.h
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "tim.h"

#define NUMS_TIMER 3

extern uint16_t flag_timer2;

extern uint16_t flag_timer[NUMS_TIMER];

void timer_init();
void setTimer2(uint16_t duration);
void setTimer(uint16_t, int);

#endif /* INC_SOFTWARE_TIMER_H_ */
