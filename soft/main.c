#include "stm32f10x.h"                  // Device header
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "My_GPIO\gpio.h"
#include "My_TIM4\tim4_delay.h"
#include "My_TIM3\tim3_pwm.h"
#include "My_RTC\rtc.h"
#include "My_LED\led.h"
#include "My_ADC\adc.h"
#include "My_Keyboard\keyboard.h"
#include "My_RTC\set_time.h"
//********************************************************************************************
//  ========================================================================================
 //   NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority 
 // ========================================================================================
 //  NVIC_PriorityGroup_2  |                0-3                |            0-3             
 // =========================================================================================
//******************************************************************************
extern volatile uint32_t v_systick;
//static volatile uint32_t sysTickDelay;
extern uint8_t h10;
extern uint8_t h1;
extern uint8_t m10;
extern uint8_t m1;
extern uint8_t dot;
//******************************************************************************
//інтервал виводу на дисплей в мілісекундах
//display interval in milliseconds
#define led_update_interval	200   //systick count
//інтервал дихання крапочки через таймер
//breathing interval dots through timer
#define dot_update_interval 4		//systick count
//******************************************************************************
int main(){
	uint8_t key = key_no;
	//інтервал через який переписується дисплей
	//interval at which the display is overwritten
	uint32_t led_upddate = 0;
	uint32_t dot_update = 0;
	dot = 0;
	uint8_t dot_bright;
	//remap PA15 and PB3 from JTAG to normal pins
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
	//
	//set systick to 1ms
  //72 000 000 / 1000 = 72 000
  //від 0 до 72000 рахує і спрацьовує  інтерапт
	//from 0 to 72000 counts and triggers an interrupt
	SysTick_Config((SystemCoreClock / 1000) - 1);
	//NVIC 0-3 / 0-3
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//init GPIOB port pins
	my_GPIOB_init();
	//TIM4 usin for delays mks and ms
	delay_TIM4_init();
	//налаштування порта дисплея
	//display port settings
	LedDisplayInit();
	//налаштування таймера щоб крапка плавно гаснула
	//setting a timer so that the dot fades out smoothly
	Init_Bright_Dp();
	//налаштування ацп для клавіатури - робота по 1-му проводу
	//ACP settings for the keyboard - work on the 1st wire
	my_ADC_init();
	//якщо часи ще не запускались то ставимо час від фонаря
	//if the times have not yet started, then set the time from the flashlight
	if (my_RTC_Init() == 1){
		my_RTC_SetFirst();
	}
	//налаштувати переривання від часів
	//set timeout
	my_RTC_Init_NVIC();

	
	//infinite loop
	while(1){
		//led update
		if (led_upddate < (v_systick + led_update_interval) ){
			LedDisplayShow(h10, h1, m10, m1);
			led_upddate = v_systick + led_update_interval;
		}
		//зменшуємо яскравіть крапки в циклі через таймер в режимі ШИМа
		//we reduce the brightness of the dot in a cycle through a timer in PWM mode
		//dot update
		if (dot != 0){
			dot_update = v_systick + dot_update_interval;
			dot = 0;
			dot_bright = 100;
			Set_Bright_Dp(dot_bright);
		}
		if (dot_update < (v_systick)){
			dot_update = v_systick + dot_update_interval;
			if (dot_bright != 0){
				dot_bright -= 1;
			}
			Set_Bright_Dp(dot_bright);
		}
		
		//клавіатура для налаштування часів
		//keyboard for setting times
		key = my_KeyKeyboard();
		if (key != key_no){
			if (key == key_sl){
				//в режимі налаштування крапка світить постійно
				//in setup mode the dot is constantly lit
				Set_Bright_Dp(100);
				my_SetTime();
			}
		}
	}

}
