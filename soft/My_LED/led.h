#ifndef LED_LIB
#define LED_LIB
#include "stm32f10x.h"                  // Device header
//******************************************************************************
#define hH_L		(GPIOB->BRR = GPIO_Pin_15)
#define hH_H		(GPIOB->BSRR = GPIO_Pin_15)

#define hL_L		(GPIOB->BRR = GPIO_Pin_14)
#define hL_H		(GPIOB->BSRR = GPIO_Pin_14)

#define mH_L		(GPIOB->BRR = GPIO_Pin_13)
#define mH_H		(GPIOB->BSRR = GPIO_Pin_13)

#define mL_L		(GPIOB->BRR = GPIO_Pin_12)
#define mL_H		(GPIOB->BSRR = GPIO_Pin_12)

#define Data_L	(GPIOB->BRR = GPIO_Pin_11)
#define Data_H	(GPIOB->BSRR = GPIO_Pin_11)

#define R_L			(GPIOB->BRR = GPIO_Pin_10)
#define R_H			(GPIOB->BSRR = GPIO_Pin_10)

#define Dp_L		(GPIOB->BRR = GPIO_Pin_5)
#define Dp_H		(GPIOB->BSRR = GPIO_Pin_5)

//******************************************************************************
void LedDisplayInit(void);
void LedDisplayShow(uint8_t h10, uint8_t h1, uint8_t m10, uint8_t m1);
//******************************************************************************
#endif
