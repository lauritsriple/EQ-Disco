/*
 * beat.cpp
 *
 * Created: 12/06/2015 17:44:06
 *  Author: Lua
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include "beat.h"
#include <util/delay.h>
#include "io.h"
#include "strip.h"


//int eqFilters=3;
//int * filterVal[8];
//enum Modus {strobe,change,eq,fade};
//strobe: turns on lights on beat (blinking)
//change: change color on beat
//eq: if used with 3 different lights, just like an equalizer with 3 channels.
//fade: fades through colors

uint8_t isBeat(void){
	uint16_t values[N_FILTER];
	static uint8_t beat=0; //1 if beat, 0 if not beat
	static uint8_t index=0;
	static uint16_t indexBeat=0;
	static uint16_t beatVal=50;
	static uint16_t noBeatCount=0;
	static uint16_t offset=20;
	
	//RESET, high -> low
	msgeq7_reset();
	_delay_ms(10);
	//READ FROM CHIP UP TO "FILTERS"
	for (int i = 0 ; i<N_FILTER;i++){
		values[i]=msgeq7_getVal();
		msgeq7_strobe();
	}
	
	strip_setRGB(values[0]>>2,values[1]>>2,values[2]>>2);
	
	//find beat, iterate through output list
	if (beat==0){
		for (uint8_t i=0; i<N_FILTER;i++){
			if (values[i]>beatVal-offset){ // high value and not already beat
				beatVal=values[i];
				noBeatCount=0;
				beat=1;
				index=i;
				indexBeat=i;
				return 1;
			}
		}
		if(beatVal>(150)){
			beatVal--;
		}
	}
	else {//beat==1
		if (values[index]*5<beatVal){
			beat=0;
			return 0;
		}
		else {
			noBeatCount++;
		}
		
		if (noBeatCount>=4){
			for (uint8_t i=0; i<indexBeat;i++){ //check lower filters! more important to get bass than wait for low
				if(values[i]>beatVal){
					beatVal=values[i];
					noBeatCount=0;
					beat=1;
					index=i;
					indexBeat=i;
					return 1;
				}
			}
		}
		if (noBeatCount>256){
			if (indexBeat<=1){ 
				indexBeat=2;
			}
			else if (noBeatCount>512){
				if (beatVal>180){
					beatVal--;
				}
				if (noBeatCount>1024){
					indexBeat=N_FILTER;
				}
			}
		}
	}
	return 0;
	
}


