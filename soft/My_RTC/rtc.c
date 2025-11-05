#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_rtc.h"              // Keil::Device:StdPeriph Drivers:RTC
#include "stm32f10x_bkp.h"              // Keil::Device:StdPeriph Drivers:BKP
#include "stm32f10x_pwr.h"              // Keil::Device:StdPeriph Drivers:PWR

#include "..\My_TIM4\tim4_delay.h"
#include "rtc.h"
//********************************************************************************************
uint8_t h10 = 0;
uint8_t h1 = 0;
uint8_t m10 = 0;
uint8_t m1 = 0;
uint8_t sec = 0;
uint8_t dot = 0x00;
//********************************************************************************************
uint8_t my_RTC_Init(void){
//
	//тактування живлення і резервної області
	//power and reserve area timing
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	//вивести на тампер (РС13) секунди
	//he 1 second output signal happened so quickly (32µs every 1 second)
//	BKP_TamperPinCmd(DISABLE);
	BKP_RTCOutputConfig(BKP_RTCOutputSource_None);
	PWR_BackupAccessCmd(ENABLE);
  //якщо часи вимкнено - ініціалізувати
//if times are off - initialize	
	if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN){
		//скинути бекап-область
		//reset the backup area
		RCC_BackupResetCmd(ENABLE);
		RCC_BackupResetCmd(DISABLE);
		//тактування кварц 32768
		//quartz clock 32768
		RCC_LSEConfig(RCC_LSE_ON);
		//чекаємо стабілізації клока
		//waiting for the tide to stabilize
		while ((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY) {
		}        
		//тактуємо
		//we are timing
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		//дільник для рахування секунд
		//divider for counting seconds
		RTC_SetPrescaler(0x7FFF);
		//вмикаємо часи
		RCC_RTCCLKCmd(ENABLE);
		//чекаємо на синхро часів з шиною
		//waiting for time synchronization with the bus
		RTC_WaitForSynchro();
		return 1;
	}	
	return 0;
}
//********************************************************************************************
void my_RTC_Init_NVIC(void){
	NVIC_InitTypeDef nvic;
	//0-1
	nvic.NVIC_IRQChannel = RTC_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 3;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	//rtc
	RTC_EnterConfigMode();
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	RTC_WaitForLastTask();
	RTC_ExitConfigMode();
	RTC_WaitForLastTask();
}
//********************************************************************************************
// Convert Date to Counter повна версія з датою
uint32_t RTC_GetRTC_Counter(RTC_DateTimeTypeDef* RTC_DateTimeStruct){
	uint8_t a;
	uint16_t y;
	uint8_t m;
	uint32_t JDN;
	a=(14-RTC_DateTimeStruct->RTC_Month)/12;
	y=RTC_DateTimeStruct->RTC_Year+4800-a;
	m=RTC_DateTimeStruct->RTC_Month+(12*a)-3;
	JDN=RTC_DateTimeStruct->RTC_Date;
	JDN+=(153*m+2)/5;
	JDN+=365*y;
	JDN+=y/4;
	JDN+=-y/100;
	JDN+=y/400;
  JDN = JDN -32045;
  JDN = JDN - JULIAN_DATE_BASE;
	JDN*=86400;
	JDN+=(RTC_DateTimeStruct->RTC_Hours*3600);
	JDN+=(RTC_DateTimeStruct->RTC_Minutes*60);
	JDN+=(RTC_DateTimeStruct->RTC_Seconds);
	return JDN;
}
//********************************************************************************************
//Get current date  повна версія з датою
void RTC_GetDateTime(uint32_t RTC_Counter, RTC_DateTimeTypeDef* RTC_DateTimeStruct){
	unsigned long time;
	unsigned long t1, a, b, c, d, e, m;
	int year = 0;
	int mon = 0;
	int wday = 0;
	int mday = 0;
	int hour = 0;
	int min = 0;
	int sec = 0;
	uint64_t jd = 0;
	uint64_t jdn = 0;
	jd = ((RTC_Counter+43200)/(86400>>1)) + (2440587<<1) + 1;
	jdn = jd>>1;
	time = RTC_Counter;
	t1 = time/60;
	sec = time - t1*60;
	time = t1;
	t1 = time/60;
	min = time - t1*60;
	time = t1;
	t1 = time/24;
	hour = time - t1*24;
	wday = jdn%7;
	a = jdn + 32044;
	b = (4*a+3)/146097;
	c = a - (146097*b)/4;
	d = (4*c+3)/1461;
	e = c - (1461*d)/4;
	m = (5*e+2)/153;
	mday = e - (153*m+2)/5 + 1;
	mon = m + 3 - 12*(m/10);
	year = 100*b + d - 4800 + (m/10);
	RTC_DateTimeStruct->RTC_Year = year;
	RTC_DateTimeStruct->RTC_Month = mon;
	RTC_DateTimeStruct->RTC_Date = mday;
	RTC_DateTimeStruct->RTC_Hours = hour;
	RTC_DateTimeStruct->RTC_Minutes = min;
	RTC_DateTimeStruct->RTC_Seconds = sec;
	RTC_DateTimeStruct->RTC_Wday = wday;
}
//********************************************************************************************
// повна версія з датою
void my_RTC_SetFirst(void){
	RTC_DateTimeTypeDef RTC_DateTime;
	RTC_DateTime.RTC_Date = 8;
	RTC_DateTime.RTC_Month = 3;
	RTC_DateTime.RTC_Year = 2023;
	RTC_DateTime.RTC_Hours = 15;
	RTC_DateTime.RTC_Minutes = 33;
	RTC_DateTime.RTC_Seconds = 00;
	//delay for first set - ???
	//delay_TIM4_ms(500);
	RTC_WaitForLastTask();
	RTC_SetCounter(RTC_GetRTC_Counter(&RTC_DateTime)); 
	RTC_WaitForLastTask();
}
//********************************************************************************************
//перетворення для переводу часу в індекси масива цифр для виводу
void my_RTC_GetTime(uint8_t *h10, uint8_t  *h1, uint8_t *m10, uint8_t *m1, uint8_t *sec){
	RTC_DateTimeTypeDef RTC_DateTime;
	uint32_t RTC_Counter;
	RTC_Counter = RTC_GetCounter();
	RTC_GetDateTime(RTC_Counter, &RTC_DateTime);
	//minute
	*m1 = RTC_DateTime.RTC_Minutes;
	if (*m1 > 9){
		*m10 = *m1 / 10;
		*m1 = RTC_DateTime.RTC_Minutes - *m10 * 10;
	}
	else{
		*m10 = 0;
	}
	//hour
	*h1 = RTC_DateTime.RTC_Hours;
	if (*h1 > 9){
		*h10 = *h1 / 10;
		*h1 = RTC_DateTime.RTC_Hours - *h10 * 10;
	}
	//ноль спереду гасимо
	else{
		*h10 = 10;
	}
	//секунди просто так забираю
	*sec = RTC_DateTime.RTC_Seconds;
}
//********************************************************************************************
void my_RTC_SetTime(uint8_t h10, uint8_t  h1, uint8_t m10, uint8_t m1){
	RTC_DateTimeTypeDef RTC_DateTime;
	uint32_t RTC_Counter;
	RTC_Counter = RTC_GetCounter();
	RTC_GetDateTime(RTC_Counter, &RTC_DateTime);
	//
	RTC_DateTime.RTC_Hours = h1 + h10 * 10;
	RTC_DateTime.RTC_Minutes = m1 + m10 * 10;
	RTC_DateTime.RTC_Seconds = 00;
	//
	RTC_WaitForLastTask();
	RTC_SetCounter(RTC_GetRTC_Counter(&RTC_DateTime)); 
	RTC_WaitForLastTask();
	
}
//********************************************************************************************
//********************************************************************************************
//скорочені варіанти для роботи тільки з часами - без повної дати
//abbreviated options for working with times only - without the full date
//********************************************************************************************
//конвертація часу в лічильник RTC
//time conversion to RTC counter
uint32_t my_TimeToRtc(RTC_DateTimeTypeDef *time )
{
  uint32_t result;
  result  = (uint32_t)time->RTC_Hours * 3600;    //часи
  result += (uint32_t)time->RTC_Minutes * 60;    //хвилини
  result +=           time->RTC_Seconds;         //секунди
 
  return result;
}
//********************************************************************************************
//конвертація лічильника в структуру часу
//converting a counter to a time structure
void my_RtcToTime( uint32_t cnt, RTC_DateTimeTypeDef *time )
 {
   time->RTC_Seconds = cnt % 60;       
   cnt /= 60;                          
   time->RTC_Minutes = cnt % 60;       
   cnt /= 60;                          
   time->RTC_Hours = cnt % 24;         
 }//********************************************************************************************
//перетворення для переводу часу в індекси масива цифр для виводу
 //conversion to convert time into indices of an array of numbers for output
void my_RTC_GetOnlyTime(uint8_t *h10, uint8_t  *h1, uint8_t *m10, uint8_t *m1, uint8_t *sec){
	RTC_DateTimeTypeDef RTC_DateTime;
	uint32_t RTC_Counter;
	RTC_Counter = RTC_GetCounter();
	my_RtcToTime(RTC_Counter, &RTC_DateTime);
	//minute
	*m1 = RTC_DateTime.RTC_Minutes;
	if (*m1 > 9){
		*m10 = *m1 / 10;
		*m1 = RTC_DateTime.RTC_Minutes - *m10 * 10;
	}
	else{
		*m10 = 0;
	}
	//hour
	*h1 = RTC_DateTime.RTC_Hours;
	if (*h1 > 9){
		*h10 = *h1 / 10;
		*h1 = RTC_DateTime.RTC_Hours - *h10 * 10;
	}
	//ноль спереду гасимо
	//we delete the leading zero
	else{
		*h10 = 10;
	}
	//секунди просто так забираю
	//I'm taking seconds just like that
	*sec = RTC_DateTime.RTC_Seconds;
}
//********************************************************************************************
void RTC_IRQHandler(){
	//преривання кожну секунду
	//interrupt every second
	if (RTC_GetITStatus(RTC_IT_SEC) == SET){
		RTC_ClearITPendingBit(RTC_IT_SEC);
		//забрати час і дату з часів
		//my_RTC_GetTime(&h10, &h1, &m10, &m1, &sec);
		//забрати тільки час
		//just take time
		my_RTC_GetOnlyTime(&h10, &h1, &m10, &m1, &sec);
		//прапорець для крапки що пройшла секунда
		//flag for the dot that has passed a second
		dot = 1;  
	}
}
//********************************************************************************************
