#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "czujniki.h"
#include "silniki.h"
#include "przyciski.h"


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
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

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
	 int prawo;
	 int lewo;
	 while (1)
	 {
		 adcCalculateValue(&prawo, &lewo);
		 printf("Sterowanie Lewo = %d    Sterowanie Prawo = %d\r\n", lewo, prawo);
		 motorsStart(prawo, lewo);
	 }
}
