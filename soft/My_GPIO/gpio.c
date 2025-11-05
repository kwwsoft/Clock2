#include "stm32f10x.h"                  // Device header
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "gpio.h"

//**********************************************************************
void my_GPIOB_init(void){
	GPIO_InitTypeDef port;
	GPIO_StructInit(&port);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*
	GPIO_Pin_15		hH		клок десятки годин		clock tens of hours
	GPIO_Pin_14		hL		клок одиниці годин		clock unit of time
	GPIO_Pin_13		mH		клок десятки хвилин		clock tens of minutes
	GPIO_Pin_12		mL		клок одиниці хвилин		clock unit of minutes
	GPIO_Pin_11		Data	8 біт даних (паралельно для всіх розрядів)	8 data bits (parallel for all bits)
	GPIO_Pin_10		R			скидання всіх розрядів в 0		resetting all digits to 0
	GPIO_Pin_5		Dp		dot
	*/
	//всі піни порта на вихід крім крапки - вона окремо
	//all pins of the port are output except for the dot - it is separate
	port.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	port.GPIO_Speed = GPIO_Speed_50MHz;
	port.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &port);
}
//**********************************************************************
