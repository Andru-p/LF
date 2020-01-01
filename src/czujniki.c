/*
 * czujniki.c
 *
 *  Created on: Dec 23, 2019
 *      Author: Andrzej
 */
#include <stdio.h>
#include "stm32f10x.h"
#include "czujniki.h"


int adcRead(int channel)
{
	 ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_13Cycles5);
	 ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	 while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);

	 return ADC_GetConversionValue(ADC1);
}

void adcDisplayValues()
{
	uint16_t adc;

	uint8_t piny[8] = { ADC_Channel_8 , ADC_Channel_9 , ADC_Channel_10,
							ADC_Channel_11, ADC_Channel_12, ADC_Channel_13,
							ADC_Channel_14, ADC_Channel_15 };

	for(unsigned char i=0;i<8;i++)
	{
		adc = adcRead(piny[i]);
		printf("Adc%d = %d    ", i + 1, adc);
	}
	printf("\n");
}

void adcCalculateValue(int *prawo, int *lewo)
{
	int sredniaPrawo = 0;
	int sredniaLewo = 0;
	int adcValues[8];   //adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8;

	uint8_t piny[8] = { ADC_Channel_8 , ADC_Channel_9 , ADC_Channel_10,
								ADC_Channel_11, ADC_Channel_12, ADC_Channel_13,
								ADC_Channel_14, ADC_Channel_15 };

	uint8_t wagi[8] = {4, 3, 2, 1, 1, 2, 3 ,4};

	for(unsigned char i=0;i<4;i++)
	{
		adcValues[i] = 4095 - adcRead(piny[i]);
		//printf("Adc%d = %d    ", i + 1, adcValues[i]);
		sredniaPrawo += adcValues[i] * wagi[i];
	}
	for(unsigned char i = 4; i < 8; i++)
	{
		adcValues[i] = 4095 - adcRead(piny[i]);
		//printf("Adc%d = %d    ", i + 1, adcValues[i]);
		sredniaLewo += adcValues[i] * wagi[i];
	}
	//printf("\r\n");

	//int sredniaPrawo = adcValues[3] + 1.6*adcValues[2] + 2*adcValues[1] + 3*adcValues[0]; //1+1.6 +  2 + 3 = 7.6
	//int sredniaLewo = adcValues[4] + 1.6*adcValues[5] + 2*adcValues[6] + 3*adcValues[7];

	*prawo = sredniaPrawo;
	*lewo = sredniaLewo;
}


void adcInit()
{
	 GPIO_InitTypeDef gpio;
	 ADC_InitTypeDef adc;

	 RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	 GPIO_StructInit(&gpio);
	 gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	 gpio.GPIO_Mode = GPIO_Mode_AIN;
	 GPIO_Init(GPIOB, &gpio);

	 GPIO_StructInit(&gpio);
	 gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	 gpio.GPIO_Mode = GPIO_Mode_AIN;
	 GPIO_Init(GPIOC, &gpio);

	 ADC_StructInit(&adc);
	 adc.ADC_ContinuousConvMode = DISABLE;
	 adc.ADC_NbrOfChannel = 1;
	 adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	 ADC_Init(ADC1, &adc);
	 ADC_Cmd(ADC1, ENABLE);

	 ADC_ResetCalibration(ADC1);
	 while (ADC_GetResetCalibrationStatus(ADC1));

	 ADC_StartCalibration(ADC1);
	 while (ADC_GetCalibrationStatus(ADC1));
}
