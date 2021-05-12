/*
 * uart.c
 *
 *  Created on: Mar 24, 2021
 *      Author: jongb
 */
#include <../Inc/stm32g071xx.h>

//Configure UART2 to monitor data through STLink COM Port in PuTTY  (PA2, PA3)
void UART_Config ( void )
{
	RCC->APBENR1 |= (1 << 17);	//enable clock for USART2
	RCC->IOPENR |= (1 << 0);	//enable clock for GPIO PORT A. (Using PA2 for Transmit)

	GPIOA->OSPEEDR |= (1 << 4);	//set output speed to LOW on PA2

	GPIOA->MODER &= ~(1 << 4);  //alternate function on PA2

	GPIOA->AFR[0] |= (1 << 8); // set AF1 in AFRH register to use USART2_TX for PA2

	USART2->CR1 &= ~(1 << 0); //disable USART2 to allow changes to registers

	USART2->CR1 &= ~(1 << 28);	//program M bits, set word length to 8 bits
	USART2->CR1 &= ~(1 << 12);
	USART2->CR1 &= ~(1 << 15);	//oversampling by 16
	USART2->BRR |= 0x683;	//set the baud rate to 9600. PCLK default = 16MHz -> (16000000/9600) = 0x683. See reference manual on calculation.
	USART2->CR2 &= ~(3 << 12) ;	// program number of stop bits (1)

	USART2->CR1 |= (1 << 0);	//enable USART2
	USART2->CR1 |= (1 << 3); //Enable Transmission for USART2
}

//Send a string through UART2
void UART_SendString ( char* s )
{
	while(*s != 0)
		{
			USART2->TDR = *s; // load the data into USART2_TDR, also clears TC bit in USART2_ISR.
			while (!(USART2->ISR & (1<<6)));  // Wait until transmission complete (TC = 1)
			s++;
		}
}

void UART_SendChar (uint8_t c)
{
	USART2->TDR = c;
	while (!(USART2->ISR & (1<<6)));
}
