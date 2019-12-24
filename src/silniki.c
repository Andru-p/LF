///*
// * silniki.c
// *
// *  Created on: Dec 23, 2019
// *      Author: Andrzej
// */
//
//
#include <math.h>
#include "stm32f10x.h"
#include "silniki.h"


void motorsInit()
{
 GPIO_InitTypeDef gpio;
 TIM_TimeBaseInitTypeDef tim;

 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

 GPIO_StructInit(&gpio);
 gpio.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
 gpio.GPIO_Speed = GPIO_Speed_50MHz;
 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOC, &gpio);
 GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

 TIM_TimeBaseStructInit(&tim);
 tim.TIM_CounterMode = TIM_CounterMode_Up;
 tim.TIM_Prescaler = 1;
 tim.TIM_Period = 4095*15; //7.6 to suma wag czujnikow
 TIM_TimeBaseInit(TIM3, &tim);

 TIM_Cmd(TIM3, ENABLE);
}

void motorsStart(int prawo, int lewo)
{
	 TIM_OCInitTypeDef  channel;

	 TIM_OCStructInit(&channel);
	 channel.TIM_OCMode = TIM_OCMode_PWM1;
	 channel.TIM_OutputState = TIM_OutputState_Enable;
	 channel.TIM_Pulse = prawo;
	 TIM_OC3Init(TIM3, &channel);
	 channel.TIM_Pulse = lewo;
	 TIM_OC4Init(TIM3, &channel);
}
