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
uint16_t rgb[3]={0,0,0};
uint16_t hsv[3]={0,0,0};
uint16_t fade[3]={0,0,0};
	
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

void strip_setHSV(uint16_t hVal,uint16_t sVal, uint16_t vVal){
	uint16_t h;
	uint16_t s;
	uint16_t v;
	
	if (hVal==65535){
		h=hsv[0];	
	}
	else{
		h=hVal;
	}
	
	if (sVal==65535){
		s=hsv[1];
	}
	else {
		s=sVal;
	}
	
	if (vVal==65535){
		v=hsv[2];
	}
	else {
		v=vVal;
	}
	
	uint16_t p,q,t,region,remainder;
	
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
			rgb[0]=v;
			rgb[1]=t;
			rgb[2]=p;
			break;
		case 1:
			strip_setRGB(q,v,p);
			rgb[0]=q;
			rgb[1]=v;
			rgb[2]=p;
			break;
		case 2:
			strip_setRGB(p,v,t);
			rgb[0]=p;
			rgb[1]=v;
			rgb[2]=t;
			break;
		case 3:
			strip_setRGB(p,q,v);
			rgb[0]=p;
			rgb[1]=q;
			rgb[2]=v;
			break;
		case 4:
			strip_setRGB(t,p,v);
			rgb[0]=t;
			rgb[1]=p;
			rgb[2]=v;
			break;
		default:
			strip_setRGB(v,p,q);
			rgb[0]=v;
			rgb[1]=p;
			rgb[2]=q;
			break;
	}
	hsv[0]=h;
	hsv[1]=s;
	hsv[2]=v;	
	
}

void strip_setFadeColor(uint8_t h, uint8_t s, uint8_t v){
	fade[0]=h;
	fade[1]=s;
	fade[2]=v;
}

void strip_setNewHSVColor(){
		uint8_t found=0;
		uint8_t newColor=0;
		while (found<3){
			found=0;
			uint8_t newColor= rand() % numColors;
			for (uint8_t i=0;i<numColors-1;i++){
				if (colors[newColor][i]==hsv[i]){
					found++;
				}
			}
		}
		strip_setHSV(colors[newColor][0],colors[newColor][1],colors[newColor][2]);
}

void strip_setNewFadeColor(){
		uint8_t found=0;
		uint8_t newColor=0;
		while (found<3){
			found=0;
			uint8_t newColor= rand() % numColors;
			for (uint8_t i=0;i<numColors-1;i++){
				if (colors[newColor][i]==hsv[i]){
					found++;
				}
			}
		}
		strip_setFadeColor(colors[newColor][0],colors[newColor][1],colors[newColor][2]);
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
		return 1;
	}
	else if (numSteps>2){
		strip_setHSV(hsv[0]-stepsize,hsv[1]-arrDiff[1]/numSteps,hsv[2]-arrDiff[2]/numSteps);
		return 0;
	}
	else {
		strip_setHSV(hsv[0]+stepsize,hsv[1]+arrDiff[1]/numSteps,hsv[2]+arrDiff[2]/numSteps);
		return 0;
	}
	return 0;	
}




