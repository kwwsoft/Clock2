#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "led.h"
#include "..\My_TIM4\tim4_delay.h"
//********************************************************************************************
/*
	GPIO_Pin_15		hH		клок десятки годин 	(по фронту) 	clock tens of hours (on the front)
	GPIO_Pin_14		hL		клок одиниці годин	(по фронту)		clock unit of time (on the front)
	GPIO_Pin_13		mH		клок десятки хвилин	(по фронту)		clock tens of minutes (on the front)
	GPIO_Pin_12		mL		клок одиниці хвилин	(по фронту)		clock unit minutes (on the front)
	//8 data bits (parallel for all digits) MSB first
	GPIO_Pin_11		Data	8 біт даних (паралельно для всіх розрядів)	MSB first	
	//resetting all bits to 0 (active 0)	
	GPIO_Pin_10		R			скидання всіх розрядів в 0 (активний 0)
*/
//********************************************************************************************
//                                0     1     2     3     4     5     6     7     8     9   pusto
volatile uint8_t dig7seg[11] = {0x6F, 0x06, 0x5B, 0x1F, 0x36, 0x3D, 0x7D, 0x07, 0x7F, 0xBF, 0x00};

//********************************************************************************************
//********************************************************************************************
void LedDisplayInit(void){
	R_L;
	//затримка просто так - без неї теж працює
	//the delay is just that - it works without it too
	delay_TIM4_us(100);
	Dp_L;
	Data_L;
	hH_L;
	hL_L;
	mH_L;
	mL_L;
	//затримка просто так - без неї теж працює
	//the delay is just that - it works without it too
	delay_TIM4_us(100);
	R_H;
}
//********************************************************************************************
void LedDisplayShow(uint8_t h10, uint8_t h1, uint8_t m10, uint8_t m1){
	uint8_t data, i;
	//
	R_L;
	hH_L;
	hL_L;
	mH_L;
	mL_L;
	R_H;
	
	//hH -------------------- десятки годин
	//вивід в регістр на платі індикатора послідовно бітів цифри - старший першим
	//output to the register on the indicator board in sequence of digit bits - highest first
	data = dig7seg[h10];		i = 8;
	do{
		if ( data & (1 << --i) )
			Data_H;
		else
			Data_L;
		hH_H;
		hH_L;
	}while(i != 0);

	//hL -------------------- одиниці годин
	//вивід в регістр на платі індикатора послідовно бітів цифри - старший першим
	//output to the register on the indicator board in sequence of digit bits - highest first
	data = dig7seg[h1];			i = 8;
	do{
		if ( data & (1 << --i) )
			Data_H;
		else
			Data_L;
		hL_H;
		hL_L;
	}while(i != 0);

	//mH -------------------- десятки хвилин
	//вивід в регістр на платі індикатора послідовно бітів цифри - старший першим
	//output to the register on the indicator board in sequence of digit bits - highest first
	data = dig7seg[m10];		i = 8;
	do{
		if ( data & (1 << --i) )
			Data_H;
		else
			Data_L;
		mH_H;
		mH_L;
	}while(i != 0);
	
	//mL -------------------- одиниці хвилин
	//вивід в регістр на платі індикатора послідовно бітів цифри - старший першим
	//output to the register on the indicator board in sequence of digit bits - highest first
	data = dig7seg[m1];			i = 8;
	do{
		if ( data & (1 << --i) )
			Data_H;
		else
			Data_L;
		mL_H;
		mL_L;
	}while(i != 0);
}
//********************************************************************************************



