/*
 * przyciski.c
 *
 *  Created on: Dec 28, 2019
 *      Author: Andrzej
 */


#include "stm32f10x.h"

void buttonInit()
{
	 GPIO_InitTypeDef gpio;
	 EXTI_InitTypeDef exti;
	 NVIC_InitTypeDef nvic;

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	 GPIO_StructInit(&gpio);
	 gpio.GPIO_Pin = GPIO_Pin_2;
	 gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_Init(GPIOD, &gpio);

	 gpio.GPIO_Pin = GPIO_Pin_13;
	 gpio.GPIO_Mode = GPIO_Mode_IPU;
	 GPIO_Init(GPIOC, &gpio);

	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);

	 EXTI_StructInit(&exti);
	 exti.EXTI_Line = EXTI_Line13;
	 exti.EXTI_Mode = EXTI_Mode_Interrupt;
	 exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	 exti.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&exti);

	 nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	 nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	 nvic.NVIC_IRQChannelSubPriority = 0x00;
	 nvic.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&nvic);
}
