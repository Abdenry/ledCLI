/*
 * uart.c: UART Driver for STM32H753
 *
 *  Created on: Jul 12, 2025
 *      Author: hiro
 */
#include "uart.h"

static uint16_t
setBuadRate(uint32_t periphCLK, buad_rate_t baudRate);

int __io_putchar(int ch) {
	usart3_tx_blocking(ch);
	return ch;
}

void usart3_init_blocking(void) {
	RCC->AHB4ENR |= GPIODEN;
	GPIOD->MODER &= ~(PD8AFMASK | PD9AFMASK);
	GPIOD->MODER |= PD8AF | PD9AF;
	GPIOD->AFR[1] |= AFR8_AF7 | AFR9_AF7;

	RCC->APB1LENR |= RCC_APB1LENR_USART3EN;
	USART3->BRR = setBuadRate(peripheralClock, BAUD_115200);
	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USART3->CR1 |= USART_CR1_UE;
}
;

void usart3_init_interrupt(void) {
	RCC->AHB4ENR |= GPIODEN;
	GPIOD->MODER &= ~(PD8AFMASK | PD9AFMASK);
	GPIOD->MODER |= PD8AF | PD9AF;
	GPIOD->AFR[1] |= AFR8_AF7 | AFR9_AF7;

	RCC->APB1LENR |= RCC_APB1LENR_USART3EN;
	USART3->BRR = setBuadRate(peripheralClock, BAUD_115200);
	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USART3->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
	NVIC_EnableIRQ(USART3_IRQn);
	USART3->CR1 |= USART_CR1_UE;
}

void usart3_tx_blocking(int ch) {
	while (!(USART3->ISR & USART_ISR_TC)) {
	}
	USART3->TDR = ch;
}

int usart3_rx_blocking(void) {
	while (!(USART3->ISR & USART_ISR_RXNE_RXFNE)) {
	}
	return USART3->RDR;
}

int usart3_rx(void){
	return USART3->RDR;
}

static uint16_t setBuadRate(uint32_t periphCLK, buad_rate_t baudRate) {
	return (periphCLK / baudRate);
}
