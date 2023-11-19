/*
 * fsm_rtc.h
 *
 *  Created on: Nov 16, 2023
 *      Author: Mr.HUNG
 */

#ifndef INC_FSM_RTC_H_
#define INC_FSM_RTC_H_

#include "main.h"
#include "ds3231.h"
#include "software_timer.h"
#include "lcd.h"
#include "button.h"

#define SECOND_INDEX 0
#define MINUTE_INDEX 1
#define HOUR_INDEX 2
#define DAY_INDEX 3
#define DATE_INDEX 4
#define MONTH_INDEX 5
#define YEAR_INDEX 6
#define BUTTON_UP 0
#define BUTTON_CHANGE 1
#define BUTTON_NEXT 2

enum stateRTC {
	INIT,
	SHOW_CLOCK,
	MODIFY_CLOCK,
	TIMER_CLOCK
};

enum stateSpecificationClock{
	SECOND,
	MINUTE,
	HOUR,
	DAY,
	DATE,
	MONTH,
	YEAR
};

extern int clock[7];
extern int timer_clock[7];

void updateTime(int,int,int,int,int,int,int);
void displayTime(void);
void fsm_stc(void);

#endif /* INC_FSM_RTC_H_ */
