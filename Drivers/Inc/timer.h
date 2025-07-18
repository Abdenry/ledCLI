/*
 * timer.h
 *
 *  Created on: Jul 18, 2025
 *      Author: hiro
 */

#ifndef DRIVERS_INC_TIMER_H_
#define DRIVERS_INC_TIMER_H_

#include <stdbool.h>
#include "stm32h7xx.h"

#define peripheralClockHz 64000000
#define desiredFrequencyHz 1000

void tim2_interrupt_init(bool usingFreeRTOS);
void tim2_enable_update(void);
void tim2_disable_update(void);

#endif /* DRIVERS_INC_TIMER_H_ */
