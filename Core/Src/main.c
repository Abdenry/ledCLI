/**
 ******************************************************************************
 * @file           : main.c
 * @author         : hiro
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include "main.h"

TaskHandle_t xPrintTask;
TaskHandle_t xControlLedTask;
TaskHandle_t xProcessInputTask;
EventGroupHandle_t xControlGroup;
QueueHandle_t xUSARTQueue;
SemaphoreHandle_t xBlinkLedSemaphore;

int main(void) {
	usart3_init_interrupt(true);
	tim2_interrupt_init(true);

	greenLedInit();

	xControlGroup = xEventGroupCreate();
	xTaskCreate(vTaskPrint, "vTaskPrint", 1024, NULL, 1, &xPrintTask);
	xTaskCreate(vTaskLED, "vTaskLED", 1024, NULL, 1, &xControlLedTask);
	xTaskCreate(vTaskInput, "vTaskInput", 1024, NULL, 1, &xProcessInputTask);
	xUSARTQueue = xQueueCreate(10,sizeof(uint32_t));
	xBlinkLedSemaphore = xSemaphoreCreateBinary();

	vTaskStartScheduler();

	/* Loop forever */
	for (;;)
		;
}

void vTaskPrint(void *pvParameters) {
	EventBits_t uxBits;
	char looped = 0;
	updateScreen();
	while (1) {
		uxBits = xEventGroupWaitBits(xControlGroup, (CMD_INVALID_INPUT | CMD_CLEAR_SCREEN | STATUS_LED_ENABLED | STATUS_LED_DISABLED | STATUS_BLINKING_ENABLED), pdFALSE, pdFALSE,
		portMAX_DELAY);
		if ((uxBits & STATUS_LED_ENABLED) != 0) {
			updateScreen();
			printf("Led has been enabled!\n");
			xEventGroupClearBits(xControlGroup, STATUS_LED_ENABLED);
			resetBlinkState(&looped);
		} else if ((uxBits & STATUS_LED_DISABLED) != 0) {
			updateScreen();
			printf("Led has been disabled!\n");
			xEventGroupClearBits(xControlGroup, STATUS_LED_DISABLED);
			resetBlinkState(&looped);
		} else if (((uxBits & STATUS_BLINKING_ENABLED) != 0) && looped == 0) {
			updateScreen();
			printf("Led is blinking!\n");
			looped = 1;
		} else if ((uxBits & CMD_CLEAR_SCREEN) != 0) {
			updateScreen();
			xEventGroupClearBits(xControlGroup, CMD_CLEAR_SCREEN);
		} else if ((uxBits & CMD_INVALID_INPUT) != 0) {
			updateScreen();
			printf("Input was not an option please pick select 1, 2 or 3\n");
			xEventGroupClearBits(xControlGroup, CMD_INVALID_INPUT);
		}
	}
}

/*
 * TODO:	Find a alternative method for blinking that doesn't induce a UI freeze
 * 		when switching from blinking state to enable/disable state.
 */
void vTaskLED(void *pvParameters) {
	EventBits_t uxBits;
	while (1) {
		uxBits = xEventGroupWaitBits(xControlGroup, (CMD_ENABLE_LED | CMD_DISABLE_LED | CMD_BLINK_LED), pdFALSE, pdFALSE, portMAX_DELAY);
		if ((uxBits & CMD_ENABLE_LED) != 0) {
			enableLed();
			xEventGroupClearBits(xControlGroup, (CMD_ENABLE_LED | CMD_BLINK_LED));
			xEventGroupSetBits(xControlGroup, STATUS_LED_ENABLED);
		} else if ((uxBits & CMD_DISABLE_LED) != 0) {
			disableLed();
			xEventGroupClearBits(xControlGroup, (CMD_DISABLE_LED | CMD_BLINK_LED));
			xEventGroupSetBits(xControlGroup, STATUS_LED_DISABLED);
		} else if ((uxBits & CMD_BLINK_LED) != 0) {
			if(xSemaphoreTake(xBlinkLedSemaphore, portMAX_DELAY) == pdTRUE){
				blinkLed();
				if ((uxBits & STATUS_BLINKING_ENABLED) == 0) {
					xEventGroupSetBits(xControlGroup, STATUS_BLINKING_ENABLED);
				}
			}
		}
	}
}


void vTaskInput(void *pvParameters) {
	uint32_t input;
	while (1) {
		if(xQueueReceive(xUSARTQueue, &input, portMAX_DELAY) == pdTRUE){
			switch (input) {
			case 49:
				xEventGroupSetBits(xControlGroup, CMD_ENABLE_LED);
				break;
			case 50:
				xEventGroupSetBits(xControlGroup, CMD_DISABLE_LED);
				break;
			case 51:
				xEventGroupSetBits(xControlGroup, CMD_BLINK_LED);
				tim2_enable_update();
				break;
			case 67:
				xEventGroupSetBits(xControlGroup, CMD_CLEAR_SCREEN);
				break;
			case 99:
				xEventGroupSetBits(xControlGroup, CMD_CLEAR_SCREEN);
				break;
			default:
				xEventGroupSetBits(xControlGroup, CMD_INVALID_INPUT);
				break;
			}
		}
	}
}

void greenLedInit(void) {
	RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
	GPIOB->MODER &= ~(PB0MASK);
	GPIOB->MODER |= PB0OUTPUT;
}

void enableLed(void) {
	GPIOB->ODR |= PB0OUTPUT;
}

void disableLed(void) {
	GPIOB->ODR &= ~(PB0OUTPUT);
}

void blinkLed(void) {
	GPIOB->ODR ^= PB0OUTPUT;
}

void __printMainMenu(void) {
	printf("Welcome to the LED CLI! Please select an option below:\n"
			"1.Enable LED\n"
			"2.Disable LED\n"
			"3.Blink LED\n\n"
			"Press C to clear the screen!\n");
}

void __clearScreen(void) {
	// ANSI and VT102 escape code "Esc[2J" clears terminal screens once received.
	printf("%c%c%c%c\n", 0x1B, 0x5B, 0x32, 0x4A);
}

void updateScreen(void) {
	__clearScreen();
	__printMainMenu();
}

void resetBlinkState(char *loopedState) {
	tim2_disable_update();
	xEventGroupClearBits(xControlGroup, STATUS_BLINKING_ENABLED);
	*loopedState = 0;
}

void USART3_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(USART3->ISR & USART_ISR_RXNE_RXFNE){
		uint32_t UARTRX = usart3_rx();
		xQueueSendFromISR(xUSARTQueue, &UARTRX, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

void TIM2_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	TIM2->SR &= ~TIM_SR_UIF; // Interrupt must be cleared manually
	xSemaphoreGiveFromISR(xBlinkLedSemaphore, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

