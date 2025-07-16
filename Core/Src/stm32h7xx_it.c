/*
 * stm32h7xx_it.c
 *
 *  Created on: Jul 11, 2025
 *      Author: hiro
 */

#include "main.h"
#include "stm32h7xx_it.h"
#include "FreeRTOS.h"
#include "task.h"

void SysTick_Handler(void) {
	xPortSysTickHandler();  // FreeRTOS tick handler
}
