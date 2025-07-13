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
  void vTaskPrint(void* pvParameters);

int main(void)
{
  usart3_init();

  xTaskCreate(vTaskPrint, "vTaskPrint", 1024, NULL, 5, &xPrintTask);

  vTaskStartScheduler();

    /* Loop forever */
	for(;;);
}

void vTaskPrint(void* pvParameters){
  while(1){
      usart3_tx_char('a');
      usart3_tx_char('\n');
  }
}




