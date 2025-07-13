/*
 * uart.c: UART Driver for STM32H753
 *
 *  Created on: Jul 12, 2025
 *      Author: hiro
 */
#include "uart.h"

void usart3_init(void);
void usart3_tx_blocking(int ch);
static uint16_t setBuadRate(uint32_t periphCLK, buad_rate_t baudRate);

int __io_putchar(int ch){
  usart3_tx_blocking(ch);
  return ch;
}

void usart3_init(void){
  RCC->AHB4ENR |= GPIODEN;
  GPIOD->MODER &= ~(PD8AFMASK | PD9AFMASK);
  GPIOD->MODER |= PD8AF | PD9AF;
  GPIOD->AFR[1] |= AFR8_AF7 | AFR9_AF7;

  RCC->APB1LENR |= USART3EN;
  USART3->BRR = setBuadRate(peripheralClock, BAUD_115200);
  USART3->CR1 |= USART_TE | USART_RE;
  USART3->CR1 |= USART_UE;
}

void usart3_tx_blocking(int ch){
  while(!(USART3->ISR & USART_ISR_TC)){}
  USART3->TDR = ch;
}

static uint16_t setBuadRate(uint32_t periphCLK, buad_rate_t baudRate){
  return (periphCLK / baudRate);
}
