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
  TaskHandle_t xBlinkLed;
  TaskHandle_t xEchoInput;

int main(void)
{
  usart3_init();
  greenLedInit();

  xTaskCreate(vTaskPrint, "vTaskPrint", 1024, NULL, 5, &xPrintTask);
  xTaskCreate(vTaskBlink, "vTaskBlink", 1024, NULL, 4, &xBlinkLed);
  xTaskCreate(vTaskEcho, "vTaskEcho", 1024, NULL, 3, &xEchoInput);

  vTaskStartScheduler();

    /* Loop forever */
	for(;;);
}

void vTaskPrint(void* pvParameters){
  int secondsPast = 0;
  const TickType_t xDelay = 10000;
  while(1){
      vTaskDelay(xDelay);
      secondsPast+=10;
      printf("%d seconds has past\n\r", secondsPast);
  }
}

void vTaskBlink(void* pvParameters){
  const TickType_t delay = 100;
  while(1){
      vTaskDelay(delay);
      GPIOB->ODR ^= PB0OUTPUT;
  }
}

void vTaskEcho(void* pvParameters){
  int input = 0;
  while(1){
      input = usart3_rx_blocking();
      printf("Echo: %c\n\r", input);
  }
}

void greenLedInit(void){
  RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
  GPIOB->MODER &= ~(PB0MASK);
  GPIOB->MODER |= PB0OUTPUT;
}






