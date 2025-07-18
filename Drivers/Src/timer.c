/*
 * timer.c
 *
 *  Created on: Jul 18, 2025
 *      Author: hiro
 */

#include "timer.h"


void tim2_interrupt_init(bool usingFreeRTOS){
	RCC->APB1LENR |= RCC_APB1LENR_TIM2EN;
	TIM2->PSC = (peripheralClockHz / desiredFrequencyHz) - 1;
	TIM2->ARR = ((desiredFrequencyHz - 1) << 16); // Configured 1 second period
	TIM2->CNT = 0;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	if(usingFreeRTOS){
		NVIC_SetPriority(TIM2_IRQn, 5);
	}
	TIM2->CR1 |= TIM_CR1_UDIS;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void tim2_enable_update(void){
	TIM2->CR1 &= ~(TIM_CR1_UDIS);
}

void tim2_disable_update(void){
	TIM2->CR1 |= TIM_CR1_UDIS;
}
