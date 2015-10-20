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

uint8_t isBeat(void){
	uint16_t values[N_ITER];
	static uint8_t beat=0; //1 if beat, 0 if not beat
	static uint16_t indexBeat=0;
	static uint16_t beatVal=50;
	static uint16_t noBeatCount=0;
	static uint16_t offset=0;
	static uint16_t minVal=400;
	
	
	PORTA |= (1 << RESET);
	_delay_us(10);
	PORTA &= ~(1 << RESET);
	_delay_us(80); //72us min
	for (uint8_t i=0;i<N_ITER;i++){
		PORTA &= ~(1 << STROBE); //low
		_delay_us(36);
		values[i]=msgeq7_read();
		PORTA |= (1 << STROBE); //high
		_delay_us(45);
	}
	
	//find beat, iterate through output list
	
	// beat should be zero when 
	if (beat==0){
		for (uint8_t i=0; i<N_FILTER;i++){
			if (values[i]>beatVal-offset){ // high value and not already beat
				beatVal=values[i];
				noBeatCount=0;
				beat=1;
				indexBeat=i;
				return 1;
			}
		}
		if(beatVal>minVal){
			beatVal--;
		}
	}
	
	else {//beat==1
		//4 is nice! changed to 5
		if ((values[indexBeat]*4<beatVal) && (noBeatCount >= 400)){ //if value on filter is X times lower than it was when the beat was detected, count as no beat.
			beat=0;
			noBeatCount=0;
			return 0;
		}
		
		else {
			noBeatCount++;
		}
		
		if (noBeatCount>=700){
			for (uint8_t i=0; i<indexBeat;i++){ //check filters lower than beatindex
				if(values[i]>beatVal){
					beatVal=values[i];
					noBeatCount=0;
					beat=1;
					indexBeat=i;
					return 1;
				}
			}
		}
		if (noBeatCount>4096){ //no beat for very long time, increse indexbeat to so that next iteration will check all filters 
			beat=0;
		}
	}
	return 0;
	
}


