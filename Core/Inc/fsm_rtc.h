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

#define SECOND_INDEX 0
#define MINUTE_INDEX 1
#define HOUR_INDEX 2
#define DAY_INDEX 3
#define DATE_INDEX 4
#define MONTH_INDEX 5
#define YEAR_INDEX 6

enum stateRTC {
	INIT,
	SHOW_CLOCK,
	MODIFY_CLOCK
};

extern int clock[7];

void updateTime(int,int,int,int,int,int,int);
void displayTime(void);
void fsm_stc(void);

#endif /* INC_FSM_RTC_H_ */
