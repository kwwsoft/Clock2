#ifndef TIM4DELAY_LIB
#define TIM4DELAY_LIB

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"



void Init_Bright_Dp(void);
void Set_Bright_Dp(uint8_t percent);

#endif
