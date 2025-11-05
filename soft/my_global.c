#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "string.h"
#include "my_global.h"
//#include "my_tim4_delay.h"
//***********************************************
volatile uint32_t v_systick;
//***********************************************
//заглушка скидання на всяк випадок
//reset plug just in case
void HardFault_Handler(void){
	NVIC_SystemReset();
}
//********************************************************************************************
//системний таймер кожну мілісекунду
//system timer every millisecond
void SysTick_Handler(void){
	//1ms
	v_systick++;
}
//********************************************************************************************



