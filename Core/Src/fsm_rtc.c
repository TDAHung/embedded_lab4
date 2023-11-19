/*
 * fsm_rtc.c
 *
 *  Created on: Nov 16, 2023
 *      Author: Mr.HUNG
 */

#include "fsm_rtc.h"

int clock[7] = {0,0,0,1,1,1,1};
int timerClock[7] = {0,0,0,1,1,1,1};

enum stateRTC currentStateClock = INIT;
enum stateSpecificationClock currentSpecificationState = SECOND;
int blink = 1;

void update_time(int second, int minute, int hour, int day, int date, int month, int year){
	ds3231_Write(ADDRESS_YEAR, year);
	ds3231_Write(ADDRESS_MONTH, month);
	ds3231_Write(ADDRESS_DATE, date);
	ds3231_Write(ADDRESS_DAY, day);
	ds3231_Write(ADDRESS_HOUR, hour);
	ds3231_Write(ADDRESS_MIN, minute);
	ds3231_Write(ADDRESS_SEC, second);
}

void displayTime(){
	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void switchTimeAtIndex(int index, int is_active){
	switch(index){
	case 0:
		if(is_active) lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
		else lcd_ShowIntNum(70, 100, ds3231_hours, 2, BLACK, BLACK, 24);
		break;
	case 1:
		if(is_active) lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
		else lcd_ShowIntNum(110, 100, ds3231_min, 2, BLACK, BLACK, 24);
		break;
	case 2:
		if(is_active) lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
		else lcd_ShowIntNum(150, 100, ds3231_sec, 2, BLACK, BLACK, 24);
		break;
	case 3:
		if(is_active) lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
		else lcd_ShowIntNum(20, 130, ds3231_day, 2, BLACK, BLACK, 24);
		break;
	case 4:
		if(is_active) lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
		else lcd_ShowIntNum(70, 130, ds3231_date, 2, BLACK, BLACK, 24);
		break;
	case 5:
		if(is_active) lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
		else lcd_ShowIntNum(110, 130, ds3231_month, 2, BLACK, BLACK, 24);
		break;
	case 6:
		if(is_active) lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
		else lcd_ShowIntNum(150, 130, ds3231_year, 2, BLACK, BLACK, 24);
		break;
	default: break;
	}
}

void convert_large_values() {
    while (clock[SECOND_INDEX] >= 60 || clock[MINUTE_INDEX] >= 60 || clock[HOUR_INDEX] >= 24 || clock[DAY_INDEX] >= 7 || clock[DATE_INDEX] > 31 || clock[MONTH_INDEX] > 12) {
        if (clock[SECOND_INDEX] >= 60) {
        	clock[SECOND_INDEX] -= 60;
        	clock[MINUTE_INDEX]++;
        }

        if (clock[MINUTE_INDEX] >= 60) {
        	clock[MINUTE_INDEX] -= 60;
        	clock[HOUR_INDEX]++;
        }

        if (clock[HOUR_INDEX] >= 24) {
        	clock[HOUR_INDEX] -= 24;
        	clock[DAY_INDEX]++;
        }

        if (clock[DAY_INDEX] >= 7) {
        	clock[DAY_INDEX] -= 7;
        }

        int days_in_month;

        switch (clock[MONTH_INDEX]) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                days_in_month = 31;
                break;
            case 4: case 6: case 9: case 11:
                days_in_month = 30;
                break;
            case 2:
                if ((clock[YEAR_INDEX] % 4 == 0 && clock[YEAR_INDEX] % 100 != 0) || (clock[YEAR_INDEX] % 400 == 0)) {
                    days_in_month = 29;
                } else {
                    days_in_month = 28;
                }
                break;
            default:
                break;
        }

        if (clock[DATE_INDEX] > days_in_month) {
        	clock[DATE_INDEX] -= days_in_month;
        	clock[MONTH_INDEX]++;

            if (clock[MONTH_INDEX] > 12) {
            	clock[MONTH_INDEX] -= 12;
            	clock[YEAR_INDEX]++;
            }
        }
    }
}

void update_clock() {
	clock[SECOND_INDEX]++;

    if (clock[SECOND_INDEX] > 59) {
    	clock[SECOND_INDEX] = 0;
    	clock[MINUTE_INDEX]++;

        if (clock[MINUTE_INDEX] > 59) {
        	clock[MINUTE_INDEX] = 0;
        	clock[HOUR_INDEX]++;

            if (clock[HOUR_INDEX] > 23) {
            	clock[HOUR_INDEX] = 0;
            	clock[DAY_INDEX]++;

                if (clock[DAY_INDEX] > 6) {
                	clock[DAY_INDEX] = 0;
                }

                clock[DATE_INDEX]++;

                int days_in_month;

                switch (clock[MONTH_INDEX]) {
                    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                        days_in_month = 31;
                        break;
                    case 4: case 6: case 9: case 11:
                        days_in_month = 30;
                        break;
                    case 2:
                        if ((clock[YEAR_INDEX] % 4 == 0 && clock[YEAR_INDEX] % 100 != 0) || (clock[YEAR_INDEX] % 400 == 0)) {
                            days_in_month = 29;
                        } else {
                            days_in_month = 28;
                        }
                        break;
                    default:
                        break;
                }

                if (clock[DATE_INDEX] > days_in_month) {
                	clock[DATE_INDEX] = 1;
                	clock[MONTH_INDEX]++;

                    if (clock[MONTH_INDEX] > 12) {
                    	clock[MONTH_INDEX] = 1;
                    	clock[YEAR_INDEX]++;
                    }
                }
            }
        }
    }
}

void showTime(int rtc[7]){
	update_time(rtc[0],rtc[MINUTE_INDEX],rtc[HOUR_INDEX],rtc[DAY_INDEX],rtc[DATE_INDEX],rtc[MONTH_INDEX],rtc[YEAR_INDEX]);
	displayTime();
}

void blinky(int index){
	if(flag_timer[1]){
		switchTimeAtIndex(index,blink);
		if(blink) blink = 0;
		else blink = 1;
		setTimer(200,1);
	}
}

int check_timer_clock(int rtc[7], int timer_clock[7]){
	if(rtc[SECOND_INDEX] == timer_clock[SECOND_INDEX] &&
			rtc[MINUTE_INDEX] == timer_clock[MINUTE_INDEX] &&
			rtc[HOUR_INDEX] == timer_clock[HOUR_INDEX] &&
			rtc[DAY_INDEX] == timer_clock[DAY_INDEX] &&
			rtc[DATE_INDEX] == timer_clock[DATE_INDEX] &&
			rtc[MONTH_INDEX] == timer_clock[MONTH_INDEX] &&
			rtc[YEAR_INDEX] == timer_clock[YEAR_INDEX] ) return 1;
	return 0;
}

void fsm_rtc_button_processing(int rtc[7]){
	switch(currentSpecificationState){
	case SECOND:
		blinky(SECOND);
		if(button_count[BUTTON_UP]) {
			if(rtc[SECOND_INDEX] == 59) rtc[SECOND_INDEX] = 0;
			rtc[SECOND_INDEX]++;
		}
		if(button_count[BUTTON_NEXT]) {
			switchTimeAtIndex(SECOND, 1);
			currentSpecificationState = MINUTE;
		}
		break;
	case MINUTE:
		blinky(MINUTE);
		if(button_count[BUTTON_UP]) {
			if(rtc[MINUTE_INDEX] == 59) rtc[MINUTE_INDEX] = 0;
			rtc[MINUTE_INDEX]++;
		}
		if(button_count[BUTTON_NEXT]) {
			switchTimeAtIndex(MINUTE, 1);
			currentSpecificationState = HOUR;
		}
		break;
	case HOUR:
		blinky(HOUR);
		if(button_count[BUTTON_UP]) {
			if(rtc[HOUR_INDEX] == 23) rtc[HOUR_INDEX] = 0;
			rtc[HOUR_INDEX]++;
		}
		if(button_count[BUTTON_NEXT]) {
			switchTimeAtIndex(HOUR, 1);
			currentSpecificationState = DAY;
		}
		break;
	case DAY:
		blinky(DAY);
		if(button_count[BUTTON_UP]) {
			if(rtc[DAY_INDEX] == 7) rtc[DAY_INDEX] = 1;
			rtc[DAY_INDEX]++;
		}
		if(button_count[BUTTON_NEXT]) {
			switchTimeAtIndex(DAY, 1);
			currentSpecificationState = DATE;
		}
		break;
	case DATE:
		blinky(DATE);
		if(button_count[BUTTON_UP]) {
	        int days_in_month;
	        switch (rtc[MONTH_INDEX]) {
	            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
	                days_in_month = 31;
	                break;
	            case 4: case 6: case 9: case 11:
	                days_in_month = 30;
	                break;
	            case 2:
	                if ((rtc[YEAR_INDEX] % 4 == 0 && rtc[YEAR_INDEX] % 100 != 0) || (rtc[YEAR_INDEX] % 400 == 0)) {
	                    days_in_month = 29;
	                } else {
	                    days_in_month = 28;
	                }
	                break;
	            default:
	                break;
	        }
			if(rtc[DATE_INDEX] == days_in_month) rtc[DATE_INDEX] = 1;
			rtc[DATE_INDEX]++;
		}
		if(button_count[BUTTON_NEXT]) {
			switchTimeAtIndex(DATE, 1);
			currentSpecificationState = MONTH;
		}
		break;
	case MONTH:
		blinky(MONTH);
		if(button_count[BUTTON_UP]) {
			if(rtc[MONTH_INDEX] == 12) rtc[MONTH_INDEX] = 1;
			rtc[MONTH_INDEX]++;
		}
		if(button_count[BUTTON_NEXT]) {
			switchTimeAtIndex(MONTH, 1);
			currentSpecificationState = YEAR;
		}
		break;
	case YEAR:
		blinky(YEAR);
		if(button_count[BUTTON_UP]) {
			rtc[YEAR_INDEX]++;
		}
		if(button_count[BUTTON_NEXT]) {
			switchTimeAtIndex(YEAR, 1);
			currentSpecificationState = SECOND;
		}
		break;
	default: break;
	}
}

void fsm_rtc(){
	switch(currentStateClock){
	case INIT:
		convert_large_values();
		currentStateClock = SHOW_CLOCK;
		break;
	case SHOW_CLOCK:
		if(flag_timer[0]){
			update_clock();
			setTimer(1000, 0);
		}
		if(check_timer_clock(clock,timerClock)){
			//lcd display reach the timer
		}
		showTime(clock);
		if(button_count[BUTTON_CHANGE]) {
			currentSpecificationState = SECOND;
			currentStateClock = MODIFY_CLOCK;
		}
		break;
	case MODIFY_CLOCK:
		fsm_rtc_button_processing(clock);
		if(button_count[BUTTON_CHANGE]) currentStateClock = TIMER_CLOCK;
		break;
	case TIMER_CLOCK:
		showTime(timerClock);
		fsm_rtc_button_processing(timerClock);
		if(button_count[BUTTON_CHANGE]) {
			currentSpecificationState = SECOND;
			currentStateClock = SHOW_CLOCK;
		}
		break;
	default: break;
	}
}

