#ifndef RTC_LIB
#define RTC_LIB
#include "stm32f10x.h"                  // Device header
//********************************************************************************************
/** 
Как конвертировать дату можно прочитать здесь:* 
https://ru.m.wikipedia.org/wiki/%D0%AE%D0%BB%D0%B8%D0%B0%D0%BD%D1%81%D0%BA%D0%B0%D1%8F_%D0%B4%D0%B0%D1%82%D0%B0
*/
#define JULIAN_DATE_BASE    2440588 
//********************************************************************************************
typedef struct{ 
	uint8_t RTC_Hours;
  uint8_t RTC_Minutes;
  uint8_t RTC_Seconds;
  uint8_t RTC_Date;
  uint8_t RTC_Wday;
  uint8_t RTC_Month;
  uint16_t RTC_Year;
}RTC_DateTimeTypeDef;
//********************************************************************************************
uint8_t my_RTC_Init(void);
void my_RTC_Init_NVIC(void);
void my_RTC_SetFirst(void);
void my_RTC_SetTime(uint8_t h10, uint8_t  h1, uint8_t m10, uint8_t m1);
//void RTC_GetDateTime(uint32_t RTC_Counter, RTC_DateTimeTypeDef* RTC_DateTimeStruct);
//void my_RTC_GetTime(uint8_t *h10, uint8_t *h1, uint8_t *m10, uint8_t *m1, uint8_t *sec);
//********************************************************************************************
#endif
