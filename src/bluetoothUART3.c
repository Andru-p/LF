/*
 * bluetoothUART3.c
 *
 *  Created on: Jan 1, 2020
 *      Author: Andrzej
 */
#include "stm32f10x.h"

void bluetoothInit()
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &gpio);
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);

	USART_StructInit(&uart);
	uart.USART_BaudRate = 9600;
	uart.USART_StopBits = USART_StopBits_1;
	uart.USART_WordLength = USART_WordLength_8b;
	uart.USART_Parity = USART_Parity_No;
	USART_Init(USART3, &uart);
	USART_Cmd(USART3, ENABLE);
}
