/*
 * strip.h
 *
 * Created: 09/06/2015 22:07:22
 *  Author: Lua
 */ 


#ifndef STRIP_H_
#define STRIP_H_

#include <avr/io.h>
//Public functions
void strip_setR(uint8_t val);
void strip_setG(uint8_t val);
void strip_setB(uint8_t val);
void strip_setRGB(uint8_t r, uint8_t g, uint8_t b);
void strip_setHSV(int hVal,int sVal, int vVal);
void strip_setNewHSVColor();//returns new HSV color from colors 2x array
//All fade-funcions uses HSV values and colors 2x array
uint8_t strip_fade(uint8_t stepsize); //returns true if faded to fadecolor
void strip_setFadeColor(uint8_t h, uint8_t s, uint8_t v); //changes fade array
void strip_setNewFadeColor();

#endif /* STRIP_H_ */