#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "czujniki.h"
#include "silniki.h"
#include "przyciski.h"
#define PREDKOSC_MIN 6000
#define PREDKOSC_MAX 15000

void send_char(char c)
{
	 while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	 USART_SendData(USART2, c);
}

int __io_putchar(int c)
{
	 if (c=='\n')
	 send_char('\r');
	 send_char(c);
	 return c;
}

void EXTI15_10_IRQHandler()
{
 if (EXTI_GetITStatus(EXTI_Line13))
 {

	 if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1 && GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2) == 0)
	 {
			 GPIO_SetBits(GPIOD, GPIO_Pin_2);
	 }
	 else if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1 && GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2) == 1)
	 {
			 GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	 }


 EXTI_ClearITPendingBit(EXTI_Line13);
 }
}

int main(void)
{
	 GPIO_InitTypeDef gpio;
	 USART_InitTypeDef uart;

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	 //UART 2 konfiguracja

	 GPIO_StructInit(&gpio);
	 gpio.GPIO_Pin = GPIO_Pin_2;
	 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_Init(GPIOA, &gpio);

	 gpio.GPIO_Pin = GPIO_Pin_3;
	 gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA, &gpio);

	 USART_StructInit(&uart);
	 uart.USART_BaudRate = 9600;
	 USART_Init(USART2, &uart);
	 USART_Cmd(USART2, ENABLE);





	 adcInit();
	 motorsInit();
	 buttonInit();
	 bluetoothInit();

	 int prawo;
	 int lewo;

	 while (1)
	 {
		 if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE)) // zdalne zalaczanie mostka przez bluetooth wersja podstawowa do zmodyfikowania
		 {
			 char c = USART_ReceiveData(USART3);
			 if (c != 0)
			 {
				 GPIO_SetBits(GPIOD, GPIO_Pin_2);
				 printf("START \r\n");
			 }
			 else
			 {
				 GPIO_ResetBits(GPIOD, GPIO_Pin_2);
				 printf("STOP \r\n");
			 }


		 }
		 adcCalculateValue(&prawo, &lewo);
		 //if(lewo == 11 && prawo == 11)
		 if(lewo == 5 && prawo == 5)
		 {
			 GPIO_ResetBits(GPIOD, GPIO_Pin_2);
			 printf("STOP poza linia \r\n");
		 }

		 lewo *= 2244;
		 //lewo *= 1020;
		 prawo *= 2200;
		 //prawo *= 1000;
		 //printf("Sterowanie Lewo = %d    Sterowanie Prawo = %d     ", lewo, prawo);

		 motorsStart(prawo, lewo);
	 }
}
