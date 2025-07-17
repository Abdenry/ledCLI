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
#include <stdbool.h>
#include "stm32h7xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "uart.h"
#include "event_groups.h"
#include "queue.h"

#define PB0MASK		(3 << 0)
#define PB0OUTPUT	(1 << 0)

/*
 * Bit 0: Command - Enable LED
 * Bit 1: Command - Disable LED
 * Bit 2: Command - Blink LED
 * Bit 3: Command - Invalid Input
 * Bit 4: Command - Clear Screen
 * Bit 5 - 9: Reserved
 * Bit 10: Status - LED Enabled
 * Bit 11: Status - LED Disabled
 * Bit 12: Status - Blinking Enabled
 * Bit 13 - 23: Reserved
 */
#define CTRL_GROUP_0			(1 <<  0)
#define CTRL_GROUP_1			(1 <<  1)
#define CTRL_GROUP_2			(1 <<  2)
#define CTRL_GROUP_3			(1 <<  3)
#define CTRL_GROUP_4			(1 <<  4)

#define CTRL_GROUP_10			(1 << 10)
#define CTRL_GROUP_11			(1 << 11)
#define CTRL_GROUP_12			(1 << 12)

#define CMD_ENABLE_LED			CTRL_GROUP_0
#define CMD_DISABLE_LED			CTRL_GROUP_1
#define CMD_BLINK_LED			CTRL_GROUP_2
#define CMD_INVALID_INPUT		CTRL_GROUP_3
#define CMD_CLEAR_SCREEN		CTRL_GROUP_4
#define STATUS_LED_ENABLED		CTRL_GROUP_10
#define STATUS_LED_DISABLED		CTRL_GROUP_11
#define STATUS_BLINKING_ENABLED	CTRL_GROUP_12

void vTaskPrint(void *pvParameters);
void vTaskLED(void *pvParameters);
void vTaskInput(void *pvParameters);

void greenLedInit(void);
void enableLed(void);
void disableLed(void);
void blinkLed(void);

void __printMainMenu(void);
void __clearScreen(void);
void updateScreen(void);
void resetBlinkState(char *loopedState);

#endif /* MAIN_H_ */
