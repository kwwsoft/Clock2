#include "tim3_pwm.h"
//***********************************************
void Init_Bright_Dp(void){
//bright dot pwm3/2
	GPIO_InitTypeDef port;
	TIM_TimeBaseInitTypeDef timer;
	TIM_OCInitTypeDef pwm;
	//порт в альтернативний режим
	//port in alternate mode
	GPIO_StructInit(&port);
	port.GPIO_Mode = GPIO_Mode_AF_PP;
	port.GPIO_Pin = GPIO_Pin_5;
	port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &port);
	//ремап вивода таймера на РВ5
	//remap timer output on РВ5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
  //
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	
	//налаштування частоти шима
	//shim frequency setting
	TIM_TimeBaseStructInit(&timer);
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Prescaler = 72 - 1; //1 mgz
	timer.TIM_Period = 10000 - 1;  //100 gz
	timer.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timer);
	//шим на 2-му каналі
	//PWM on channel 2
	TIM_OCStructInit(&pwm);
	pwm.TIM_OCMode = TIM_OCMode_PWM1;
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_Pulse = 50;   //50%
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &pwm);
	//дозволити 2-й канал
	//enable 2nd channel
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//
	TIM_Cmd(TIM3, ENABLE);
}
//***********************************************
void Set_Bright_Dp(uint8_t percent){
	//залежно від заповнення ШИМ-сигналу буде різна яскравість крапки
	//depending on the filling of the PWM signal, the brightness of the dot will be different
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;
  uint32_t ccr = (uint32_t)((percent / 100.0f) * (TIM3->ARR + 1));
  TIM3->CCR2 = ccr;
	TIM_Cmd(TIM3, ENABLE);
}
//**********************************************************************
