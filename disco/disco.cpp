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
#include <stdlib.h>

uint8_t controlMode=1;
uint8_t mode=1;
uint8_t musicMode=1;
uint8_t on=0;

int main() {
	pwm_init();
	adc_init();
	msgeq7_init();
	led_init();
	button_init();
	sw_init();
	led_setMode(0);
	static uint8_t repeat=0;
	static uint8_t count=0;
	static uint16_t noBeatCount=0;
	DDRA&=~(1<<MSGEQ7);
	PORTA&=~(1<<MSGEQ7);
	
	adc_setChannel(0);
	while(1){
		if (sw_status()){
			mode=2;
			led_setMode(0);
		}
		else{
			mode=1;
			//led_setMode(0);
		}
		
		if (button_pressed()){
			if (controlMode>=3){
				controlMode=1;
			}
			else {
				controlMode++;
			}
		}
		
		switch (mode){
			case 1: //const
				if (adc_getChannel()!=5){
					adc_setChannel(5);
				}
				switch (controlMode){
					case 1: //Control hue
						led_setMode(1);
						strip_setHSV((adc_read()>>2),55555,55555);
						break;
					case 2: //control saturation
						led_setMode(2);
						strip_setHSV(55555,(adc_read()>>2),55555);
						break;
					case 3: //control brightness
						led_setMode(3);
						strip_setHSV(55555,55555,(adc_read()>>2));
						break;
				}
				break;
			case 2: //music
				if (count==repeat){
					repeat=(rand()%3+2)*4;
					count=0;
					musicMode=(rand()%3)+1;
				}
				
				switch (musicMode){
					case 1: //change
						if (isBeat()){
							count++;
							strip_setNewHSVColor();
							noBeatCount=0;
						}
						else{
							noBeatCount++;
						}
						break;
					case 2: //blink
						if (isBeat()){
							count++;
							noBeatCount=0;
							strip_setNewHSVColor();
							_delay_ms(30);
							strip_setHSV(55555,55555,0);
							
						}
						else {
							noBeatCount++;
						}
						break;
					case 3: //inverted blink
						if (isBeat()){
							count++;
							noBeatCount=0;
							if (on){
								strip_setHSV(55555,55555,0);
								on=0;
							}
							else{
								on=1;
								strip_setNewHSVColor();
							}
						}
						else{
							noBeatCount++;
						}
						break;
				}
				
				if (noBeatCount>=2048){
					noBeatCount=2048;
					if (strip_fade(32)){
						strip_setNewFadeColor();
					}
				}
				break;
		}
		
	}
}
