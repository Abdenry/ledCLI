/*
 * uart.h: UART Driver for STM32H753
 *
 *  Created on: Jul 12, 2025
 *      Author: hiro
 */

#ifndef DRIVERS_INC_UART_H_
#define DRIVERS_INC_UART_H_

#include <stdint.h>
#include <stdlib.h>
#include "stm32h7xx.h"

#define peripheralClock 64000000

#define GPIODEN		(1 <<  3)
#define PD8AF		(2 << 16)
#define PD8AFMASK	(3 << 16)
#define PD9AF		(2 << 18)
#define PD9AFMASK	(3 << 18)
#define	AFR8_AF7	(7 <<  0)
#define AFR9_AF7	(7 <<  4)

typedef enum {
	BAUD_9600 = 9600, BAUD_115200 = 115200
} buad_rate_t;

void usart3_init_blocking(void);
void usart3_init_interrupt(void);
void usart3_tx_blocking(int ch);
int usart3_rx_blocking(void);
int usart3_rx(void);

#endif /* DRIVERS_INC_UART_H_ */
