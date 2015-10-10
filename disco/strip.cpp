/*
 * strip.cpp
 *
 * Created: 09/06/2015 22:07:34
 *  Author: Lua
 */ 

#include "io.h"
#include <avr/io.h>
#include <stdlib.h>
#include "strip.h"
#include <stdio.h>


//Private variables
uint8_t rgb[3]={0,0,0};
uint8_t hsv[3]={0,0,0};
uint8_t fade[3]={0,0,0};
	
uint8_t colors[4][3]= {
 	{250,255,255},
 	{100,255,255},
 	{230,255,255},
 	{45,255,255}
};
 
#define numColors 4

void strip_setChannel(Color col,uint8_t val){
	pwm_set(col,val);
	rgb[col]=val;
}

void strip_setRGB(uint8_t red, uint8_t green, uint8_t blue){
	strip_setChannel(r,red);
	strip_setChannel(g,green);
	strip_setChannel(b,blue);
}

void strip_setHSV(int hVal,int sVal, int vVal){
	uint16_t h;
	uint16_t s;
	uint16_t v;
	
	if (hVal==-1){
		h=hsv[0];	
	}
	else{
		h=hVal;
	}
	
	if (sVal==-1){
		s=hsv[1];
	}
	else {
		s=sVal;
	}
	
	if (vVal==-1){
		v=hsv[2];
	}
	else {
		v=vVal;
	}
	uint16_t p,q,t,region,remainder;
	
	if (hVal==-1){
		h=hsv[0];
	}
	if (sVal==-1){
		s=hsv[1];
	}
	
	if (s==0){
		strip_setRGB(v,v,v);
		hsv[0]=v;
		hsv[1]=v;
		hsv[2]=v;
		return;
	}
	
	region = h/43;
	remainder = (h-(region*43))*6;
	
	p=(v*(255-s)) >> 8;
	q=(v*(255-((s*remainder)>>8)))>>8;
	t=(v*(255-((s*(255-remainder))>>8)))>>8;
	
	switch(region){
		case 0:
			strip_setRGB(v,t,p);
			hsv[0]=v;
			hsv[1]=t;
			hsv[2]=p;
			break;
		case 1:
			strip_setRGB(q,v,p);
			hsv[0]=q;
			hsv[1]=v;
			hsv[2]=p;
			break;
		case 2:
			strip_setRGB(p,v,t);
			hsv[0]=p;
			hsv[1]=v;
			hsv[2]=t;
			break;
		case 3:
			strip_setRGB(p,q,v);
			hsv[0]=p;
			hsv[1]=q;
			hsv[2]=v;
			break;
		case 4:
			strip_setRGB(t,p,v);
			hsv[0]=t;
			hsv[1]=p;
			hsv[2]=v;
			break;
		default:
			strip_setRGB(v,p,q);
			hsv[0]=v;
			hsv[1]=p;
			hsv[2]=q;
			break;
	}	
	
}

void strip_setNewHSVColor(){
		uint8_t index;
		while (1){
			uint8_t newColor= rand() % numColors;
			for (uint8_t i=0;i<3;i++){
				if (colors[newColor][i]!=hsv[i]){
					index=i;
				}
			}
		}
		strip_setHSV(colors[index][0],colors[index][1],colors[index][2]);
}

void strip_setFadeColor(uint8_t h, uint8_t s, uint8_t v){
	fade[0]=h;
	fade[1]=s;
	fade[2]=v;
}

uint8_t strip_fade(uint8_t stepsize){
	//find out how many steps on different channels
	//how many iterations is needed?
	//fade a small step
	int arrDiff[3] = {0,0,0};
	for (uint8_t i = 0 ; i <3;i++){
		arrDiff[i]=fade[i]-hsv[i];
	}
	//how many steps needed with current stepsize to fit hue?
	int numSteps = arrDiff[0]/stepsize;
	if (numSteps<=1 && numSteps>=-1){
		strip_setHSV(fade[0],fade[1],fade[2]);
		uint8_t arr[3] = {0,0,0};
		strip_setFadeColor(arr[0],arr[1],arr[2]);
		return 1;
	}
	else if (numSteps>0){
		strip_setHSV(hsv[0]-stepsize,hsv[1]-arrDiff[1]/numSteps,hsv[2]-arrDiff[2]/numSteps);
		return 0;
	}
	else {
		strip_setHSV(hsv[0]+stepsize,hsv[1]+arrDiff[1]/numSteps,hsv[2]+arrDiff[2]/numSteps);
		return 0;
	}
	return 0;	
}

void strip_setNewFadeColor(){
	uint8_t index;
	while (1){
		uint8_t newColor= rand() % numColors;
		for (uint8_t i=0;i<3;i++){
			if (colors[newColor][i]!=hsv[i]){
				index=i;
			}
		}
	}
	strip_setFadeColor(colors[index][0],colors[index][1],colors[index][2]);
}


