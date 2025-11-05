#include "..\my_global.h"
#include "..\My_TIM4\tim4_delay.h"
#include "..\My_LED\led.h"
#include "..\My_Keyboard\keyboard.h"
#include "rtc.h"
#include "set_time.h"
//******************************************************************************
extern uint8_t h10;
extern uint8_t h1;
extern uint8_t m10;
extern uint8_t m1;
extern volatile uint32_t v_systick;
extern volatile uint8_t dig7seg[11];
//******************************************************************************
#define pause 200;
//******************************************************************************
void my_SetTime(void){
	uint8_t h10s = h10;
	uint8_t h1s = h1;
	uint8_t m10s = m10;
	uint8_t m1s = m1;
	uint8_t key;
	uint8_t	value;
	uint32_t v_tick;
	//встановлення часу перебором по 1-й цифрі
	//setting the time by searching through the 1st digit
	Dp_H;
	//встановлюємо десятки годин ---
	//we set dozens of hours ---
	value = h10s;
	//інтервал бликання розряду
	//discharge flashing interval
	v_tick = v_systick + pause;
	do{
		//глянемо клаву що нажато
		//Let's see what key is pressed.
		key = my_KeyKeyboard();
		//виводиться цифра
		//a number is displayed
		if (value < 10){
			//інтервал виводу ще не закінчився
			//the output interval has not ended yet
			if (v_tick > v_systick){
			}
			//інтервал виводу закінчився
			//output interval has expired
			else{
				//пусто
				//empty
				value = 10;
				LedDisplayShow(value, h1s, m10s, m1s);
				//задаєм інтервал виводу
				//set the output interval
				v_tick = v_systick + pause;
			}
		}
		//виводиться пусто
		//show empty
		else{
			//інтервал виводу ще не закінчився
			//the output interval has not ended yet
			if (v_tick > v_systick){
			}
			//інтервал виводу закінчився
			//output interval has expired
			else{
				//поточна цифра
				//current digit
				value = h10s;
				LedDisplayShow(value, h1s, m10s, m1s);
				//задаєм інтервал виводу
				//set the output interval
				v_tick = v_systick + pause;
			}
		}
		//тут йде обробка клавіш вверх та вниз
		//here is the processing of the up and down keys
		//клавіша збільшити +
		//zoom in + key
		if (key == key_u){
			//не після 20-ї
			//not after the 20th
			if (h10s < 2){
				h10s++;
				v_tick = v_systick;
				//після 20-ї
				//after the 20th
				if (h10s == 2){
					//години не можуть бути більше за 23
					//hours cannot be more than 23
					if (h1s > 3){
						h1s = 0;
					}
				}
			}
		}
		//клавіша зменшити -
		//decrease key -
		if (key == key_d){
			if (h10s != 0){
				h10s--;
				v_tick = v_systick;
				//у будь якому випадку повернемо поточну цифру одиниць бо час менше 20-ї
				//in any case, we will return the current units digit because the time is less than 20
				h1s = h1;
			}
		}
	}while(key != key_sl);
	//встановлюємо десятки годин --- кінець
	//set dozens of hours --- end
	


	
	//встановлюємо одиниці годин ---
	value = h1s;
	//інтервал бликання розряду
	v_tick = v_systick + pause;
	do{
		//глянемо клаву що нажато
		key = my_KeyKeyboard();
		//виводиться цифра
		if (value < 10){
			//інтервал виводу ще не закінчився
			if (v_tick > v_systick){
			}
			//інтервал виводу закінчився
			else{
				//пусто
				value = 10;
				LedDisplayShow(h10s, value, m10s, m1s);
				//задаєм інтервал виводу
				v_tick = v_systick + pause;
			}
		}
		//виводиться пусто
		else{
			//інтервал виводу ще не закінчився
			if (v_tick > v_systick){
			}
			//інтервал виводу закінчився
			else{
				//поточна цифра
				value = h1s;
				LedDisplayShow(h10s, value, m10s, m1s);
				//задаєм інтервал виводу
				v_tick = v_systick + pause;
			}
		}
		//тут йде обробка клавіш вверх та вниз
		//клавіша збільшити +
		if (key == key_u){
			//до 20-ї
			if (h10s < 2){
				//може бути будь-яка цифра
				if (h1s < 9){
					h1s++;
				}
			}
			//після 20-ї
			else{
				//може бути тільки від 0 до 3
				if (h1s < 3){
					h1s++;
				}
			}
		}
		//клавіша зменшити -
		if (key == key_d){
			if (h1s != 0){
				h1s--;
				v_tick = v_systick;
			}
		}
	}while(key != key_sl);
	//встановлюємо одиниці годин --- кінець
	
	
	//встановлюємо десятки хвилин ---
	value = m10s;
	//інтервал бликання розряду
	v_tick = v_systick + pause;
	do{
		//глянемо клаву що нажато
		key = my_KeyKeyboard();
		//виводиться цифра
		if (value < 10){
			//інтервал виводу ще не закінчився
			if (v_tick > v_systick){
			}
			//інтервал виводу закінчився
			else{
				//пусто
				value = 10;
				LedDisplayShow(h10s, h1s, value, m1s);
				//задаєм інтервал виводу
				v_tick = v_systick + pause;
			}
		}
		//виводиться пусто
		else{
			//інтервал виводу ще не закінчився
			if (v_tick > v_systick){
			}
			//інтервал виводу закінчився
			else{
				//поточна цифра
				value = m10s;
				LedDisplayShow(h10s, h1s, value, m1s);
				//задаєм інтервал виводу
				v_tick = v_systick + pause;
			}
		}
		//тут йде обробка клавіш вверх та вниз
		//клавіша збільшити +
		if (key == key_u){
			if (m10s < 5){
				m10s++;
			}
		}
		//клавіша зменшити -
		if (key == key_d){
			if (m10s != 0){
				m10s--;
				v_tick = v_systick;
			}
		}
	}while(key != key_sl);
	//встановлюємо десятки хвилин --- кінець
	
	
	//встановлюємо одиниці хвилин ---
	value = m1s;
	//інтервал бликання розряду
	v_tick = v_systick + pause;
	do{
		//глянемо клаву що нажато
		key = my_KeyKeyboard();
		//виводиться цифра
		if (value < 10){
			//інтервал виводу ще не закінчився
			if (v_tick > v_systick){
			}
			//інтервал виводу закінчився
			else{
				//пусто
				value = 10;
				LedDisplayShow(h10s, h1s, m10s, value);
				//задаєм інтервал виводу
				v_tick = v_systick + pause;
			}
		}
		//виводиться пусто
		else{
			//інтервал виводу ще не закінчився
			if (v_tick > v_systick){
			}
			//інтервал виводу закінчився
			else{
				//поточна цифра
				value = m1s;
				LedDisplayShow(h10s, h1s, m10s, value);
				//задаєм інтервал виводу
				v_tick = v_systick + pause;
			}
		}
		//тут йде обробка клавіш вверх та вниз
		//клавіша збільшити +
		if (key == key_u){
			if (m1s < 9){
				m1s++;
			}
		}
		//клавіша зменшити -
		if (key == key_d){
			if (m1s != 0){
				m1s--;
				v_tick = v_systick;
			}
		}
	}while(key != key_sl);
	//встановлюємо одиниці хвилин --- кінець
	
	
	my_RTC_SetTime(h10s, h1s, m10s, m1s);
	
	
	
	
}
//******************************************************************************



