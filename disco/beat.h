/*
 * beat.h
 *
 * Created: 12/06/2015 17:44:15
 *  Author: Lua
 */ 


#ifndef BEAT_H_
#define BEAT_H_

#define N_FILTER 3

//uses the lowest bandpass-filters on MSGEQ7 up to "filter" and analyses them for beat.
//this function should be run very often (whith timer maybe?)
uint8_t isBeat(void);

//uses all the filters on MSGEQ7 and tries to make sense of them.
//should let through up to 7 filters. if less filters is choosen, it will
//use some kind of average values of all the filters divided in equally parts
//returns pointer to int array. [length, value1,value2,...,value7]
//int * getEQ(int length);



#endif /* BEAT_H_ */