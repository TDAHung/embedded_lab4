/*
 * fsm_rtc.c
 *
 *  Created on: Nov 16, 2023
 *      Author: Mr.HUNG
 */

#include "fsm_rtc.h"

int clock[7] = {0,0,0,0,0,0,0};

enum stateRTC currentStateClock = INIT;

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


void fsm_rtc(){
	switch(currentStateClock){
	case INIT:
		currentStateClock = SHOW_CLOCK;
		break;
	case SHOW_CLOCK:
		if(flag_timer[0]){
			update_clock();
			setTimer(1000, 0);
		}
		update_time(clock[SECOND_INDEX],clock[MINUTE_INDEX],clock[HOUR_INDEX],clock[DAY_INDEX],clock[DATE_INDEX],clock[MONTH_INDEX],clock[YEAR_INDEX]);
		break;
	default: break;
	}
}

