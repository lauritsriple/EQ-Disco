/*
 * strip.h
 *
 * Created: 09/06/2015 22:07:22
 *  Author: Lua
 */ 


#ifndef STRIP_H_
#define STRIP_H_

#include <avr/io.h>
#include "io.h"
#include <stdlib.h>

//Public functions
void strip_setChannel(Color col,uint8_t val);
void strip_setRGB(uint8_t r, uint8_t g, uint8_t b);
void strip_setHSV(uint16_t hVal,uint16_t sVal, uint16_t vVal);
void strip_setNewHSVColor();//returns new HSV color from colors 2x array
//All fade-funcions uses HSV values and colors 2x array
uint8_t strip_fade(uint16_t stepcount); //returns true if faded to fadecolor
void strip_setFadeColor(uint16_t h, uint16_t s, uint16_t v); //changes fade array
void strip_setNewFadeColor();

#endif /* STRIP_H_ */