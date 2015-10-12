/*
 * disco.cpp
 *
 * Created: 12/06/2015 16:56:04
 *  Author: Lua
 */ 

#define F_CPU 8000000UL //8mhz

#include <avr/io.h>
//#include "strip.h"
#include "io.h"
#include <util/delay.h>
//#include "beat.h"
//#include <stdlib.h>

/*uint8_t controlMode=1;
uint8_t mode=1;
static uint8_t musicMode=1;*/


int main() {
	//pwm_init();
	//adc_init();
	//msgeq7_init();
	//led_init();
	//button_init();
	//sw_init();
	//led_setMode(0);
	
	DDRA |= (1 << RESET) | (1 << STROBE);
	
	/*static uint8_t repeat=0;
	static uint8_t count=0;
	static uint16_t noBeatCount=0;
	uint8_t val=0;*/
	while(1){
		DDRA&=~(1<<MSGEQ7);
		PORTA&=~(1<<MSGEQ7);
		/*if (sw_status()){
			mode=2;
			led_setMode(0);
		}
		else{
			mode=1;
			//led_setMode(0);
		}*/
		
		//RESET, high -> low
		PORTA |= (1 << RESET);
		_delay_us(10);
		PORTA &= ~(1 << RESET);
		PORTB |= (1<<STROBE);
		_delay_us(100);
		//_delay_us(160);
		uint16_t value=0;
		for (uint8_t i=0;i<7;i++){
			PORTA &= ~(1 << STROBE); //low
			_delay_us(100);
			//value=adc_read();
			//if (value>=1023){
				//led_blink(LED2,1);
			//}
			PORTA |= (1 << STROBE); //high
			_delay_us(100);
			
		}
		PORTA &=~(1<<STROBE);
		//_delay_us(600);
		//led_blink(LED1,1);
		
		/*if (button_pressed()){
			if (controlMode>=3){
				controlMode=1;
			}
			else {
				controlMode++;
			}
		}*/
		/*adc_setChannel(0);
		switch (mode){
			case 1: //const
				if (adc_getChannel()!=5){
					adc_setChannel(5);
				}
				switch (controlMode){
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
				if (count==repeat){
					repeat=(rand()%5)*4;
					count=0;
					musicMode=(rand()%2)+1;
				}
				
				
				
				musicMode=1;
			
				switch (musicMode){
					case 1: //blink
						if (isBeat()){
							//strip_setRGB(250,0,0);
							//strip_setNewHSVColor();
							//strip_setHSV(250,250,250);
							//_delay_ms(200);
							//strip_setHSV(250,250,80);
							//strip_setHSV(0,0,0);
							//strip_setRGB(50,0,0);
						}
						else{
							noBeatCount++;
						}
						break;
					case 2: //change
						if (isBeat()){
							//strip_setNewHSVColor();
						}
						else {
							noBeatCount++;
						}
						break;
				}
				
				if (noBeatCount>10){
					_delay_ms(10);
					if (strip_fade(1)){
						led_blink(LED2,1);
						strip_setNewFadeColor();
						
					}
				}
				count++;
				break;
		}*/
	}
}
