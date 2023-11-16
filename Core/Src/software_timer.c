/*
 * software_timer.c
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#include "software_timer.h"

#define TIMER_CYCLE_2 1


uint16_t flag_timer2 = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_MUL = 0;

uint16_t flag_timer[NUMS_TIMER] = {0,0,0};
uint16_t timer_counter[NUMS_TIMER] = {0,0,0};
uint16_t timer_MUL[NUMS_TIMER] = {0,0,0};

void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
}

void setTimer2(uint16_t duration){
	timer2_MUL = duration/TIMER_CYCLE_2;
	timer2_counter = timer2_MUL;
	flag_timer2 = 0;
}

void setTimer(uint16_t duration, int index){
	timer_MUL[index] = duration/TIMER_CYCLE_2;
	timer_counter[index] = timer_MUL[index];
	flag_timer[index] = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		if(timer2_counter > 0){
			timer2_counter--;
			if(timer2_counter == 0) {
				flag_timer2 = 1;
				timer2_counter = timer2_MUL;
			}
		}
		led7_Scan();
		for(int i = 0; i < NUMS_TIMER; i++){
			timer_counter[i]--;
			if(timer_counter[i] == 0) {
				flag_timer[i] = 1;
				timer_counter[i] = timer_MUL[i];
			}
		}
	}
}

