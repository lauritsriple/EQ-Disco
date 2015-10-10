/*
 * disco.cpp
 *
 * Created: 12/06/2015 16:56:04
 *  Author: Lua
 */ 

#define F_CPU 8000000UL //8mhz

#include <avr/io.h>
#include "strip.h"
#include "io.h"
#include <util/delay.h>
#include "beat.h"
#include <avr/interrupt.h>
#include <stdlib.h>

static volatile uint8_t mode=1;
static uint8_t musicMode=1;

int main() {
	pwm_init();
	adc_init();
	msgeq7_init();
	led_init();
	button_init();
	sw_init();
	
	static uint8_t repeat=0;
	static uint8_t count=0;
	static uint16_t noBeatCount=0;
	
	while(1){
		//led_blink(LED1,1);
		//led_blink(LED2,1);
		_delay_ms(100);
		
		
		
		if (!(sw_status)){
			led_blink(LED1,1);
		}
		/*
		
		if (button_pressed()){
			led_blink(LED1,1);
			if (mode>=3){
				mode=1;
			}
			else {
				mode++;
			}
		}
		switch (mode){
			case 1: //const
				uint8_t hsv[3];
				adc_setChannel(POT1);
				hsv[0]=adc_read()>>2;
				adc_setChannel(POT2);
				hsv[1]=adc_read()>>2;
				adc_setChannel(POT3);
				hsv[2]=adc_read()>>2;
				strip_setHSV(hsv[0],hsv[1],hsv[2]);
				break;
			case 2: //fade
				uint8_t speed;
				uint8_t val;
				adc_setChannel(POT2);
				speed=adc_read()>>2;
				adc_setChannel(POT3);
				val=adc_read()>>2;
				strip_setHSV(-1,-1,val);
				if (strip_fade(speed)){
					strip_setNewFadeColor();
				}
				break;
			case 3: //music
				if (count==repeat){
					repeat=(rand()%5)*4;
					count=0;
					musicMode=(rand()%2)+1;
				}
			
				switch (musicMode){
					case 1: //blink
						if (isBeat()){
							strip_setNewHSVColor();
							_delay_ms(200);
							strip_setHSV(0,0,0);
						}
						else{
							noBeatCount++;
						}
						break;
					case 2: //change
						if (isBeat()){
							strip_setNewHSVColor();
						}
						else {
							noBeatCount++;
						}
						break;
				}
				if (noBeatCount>2048){
					if (strip_fade(10)){
						strip_setNewFadeColor();
					}
				}
				count++;
				break;
			default:
				break;
		}*/
	}
}
