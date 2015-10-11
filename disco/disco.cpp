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

uint8_t controlMode=1;
uint8_t mode=1;
static uint8_t musicMode=1;


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
	uint8_t val=0;
	strip_setRGB(0,0,0);
	adc_setChannel(5);
	strip_setHSV(150,250,120);
	while(1){
		if (sw_status()){
			mode=2;
			led_setMode(0);
		}
		else{
			mode=1;
			led_setMode(0);
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
				switch (controlMode){
					if (adc_getChannel()!=5){
						adc_setChannel(5);
					}
					_delay_ms(100);
					case 1: //Control hue
						led_setMode(1);
						strip_setHSV((adc_read()>>2),65535,65535);
						break;	
					case 2: //control saturation
						led_setMode(2);
						strip_setHSV(65535,(adc_read()>>2),65535);
						break;
					case 3: //control brightness
						led_setMode(3);
						strip_setHSV(65535,65535,(adc_read()>>2));
						break;
				}
				break;
			case 2: //music
				led_blink(LED1,1);
				if (count==repeat){
					repeat=(rand()%5)*4;
					count=0;
					musicMode=(rand()%2)+1;
				}
			
				switch (musicMode){
					case 1: //blink
						/*if (isBeat()){
							//strip_setNewHSVColor();
							_delay_ms(200);
							//strip_setHSV(0,0,0);
						}
						else{
							noBeatCount++;
						}*/
						break;
					case 2: //change
						/*if (isBeat()){
							//strip_setNewHSVColor();
						}
						else {
							noBeatCount++;
						}*/
						break;
				}
				
				if (noBeatCount>10){
					/*if (strip_fade(1)){
						//strip_setNewFadeColor();
						led_blink(LED2,1);
					}*/
					led_blink(LED2,1);
				}
				count++;
				break;
		}
	}
}
