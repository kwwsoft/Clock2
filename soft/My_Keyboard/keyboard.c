#include "..\my_global.h"
#include "..\My_TIM4\tim4_delay.h"
#include "keyboard.h"
//********************************************************************************************
//********************************************************************************************
uint8_t my_KeyKeyboard(){
	uint32_t tmp = 0;
	uint32_t count, i, k;
	//якщо нічого не нажато
	//if nothing is pressed
	if (ADC1->DR > 3800){
		return key_no;
	}
	//
	count = 0;
	k = 1000;
	tmp = ADC1->DR;
	//крутимо трохи часу макс 3,1сек
	//we twist a little time max 3.1sec
	for (i = 0; i < 500000; i++){
		//якщо нажато - то лічильник в плюс
		//if pressed, then the counter is incremented
		if ( (tmp - ADC1->DR) < 150){
			count++;
			//ліквідувати переповнення при довгому нажатті
			//eliminate overflow on long press
			if (count == UINT32_MAX){
				count --; 
			}
		}
		//вже не нажато - скидаємо лічильник
		//no longer pressed - reset the counter
		else{
			k--;
		}
		//лічильник ненажато фсе - брейк по циклу
		//counter unpressed fse - break in cycle
		if (k == 0){
			//захист від дребезгу
			//anti-bounce protection
			if (count < 100){
				return key_no;
			}
			else{
				break;
			}
		}
	  delay_TIM4_us(50);
	}
	//почекаємо віджатого стану
	//let's wait for the squeezed state
	while (ADC1->DR < 3800){
	}
	//коротке нажимання
	//short press
	if (count < 15000){
		if (tmp < 500){
			return key_sl;
		}
		else if (tmp < 2000){
			return key_u;
		}
		else{
			return key_d;
		}
	}
	//довге нажаття
	//long press
	else{
		if (tmp < 500){
			return key_sl_l;
		}
		else if (tmp < 2000){
			return key_u_l;
		}
		else{
			return key_d_l;
		}
	}
}
//********************************************************************************************
