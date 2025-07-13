/*
 * main.h
 *
 *  Created on: Jul 11, 2025
 *      Author: hiro
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stdio.h>
#include "stm32h7xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "uart.h"

#define PB0MASK		(3 << 0)
#define PB0OUTPUT	(1 << 0)

void vTaskPrint(void* pvParameters);
void vTaskBlink(void* pvParameters);
void vTaskEcho(void* pvParameters);
void greenLedInit(void);

#endif /* MAIN_H_ */
