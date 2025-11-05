#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "string.h"
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "adc.h"
//********************************************************************************************
//********************************************************************************************
//налаштування АЦП для опитування клавіатури по одному дроту  - рівень напруги
//ADC settings for polling the keyboard via one wire - voltage level
void my_ADC_init(){
	ADC_InitTypeDef adc;
	//
	GPIO_InitTypeDef port;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//adc 0
	GPIO_StructInit(&port);
	port.GPIO_Mode = GPIO_Mode_AIN;
	port.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &port);
	//
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO, ENABLE);
	//
	adc.ADC_Mode = ADC_Mode_Independent;
	adc.ADC_ScanConvMode = DISABLE;
	adc.ADC_ContinuousConvMode = ENABLE;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfChannel = 1;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles5);
	ADC_Init(ADC1, &adc);
	ADC_Cmd(ADC1, ENABLE);
	//
	ADC_ResetCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	//
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
//********************************************************************************************
//********************************************************************************************

