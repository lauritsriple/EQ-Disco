/*
 * io.h
 *
 * Created: 09/06/2015 22:05:29
 *  Author: Lua
 */ 


#ifndef IO_H_
#define IO_H_

#include <avr/io.h> //needed for uint8_t

//PORT A
#define MSGEQ7	0	//PA0
#define POT1	1	//PA1
#define POT2	2	//PA2
#define POT3	3	//PA3
#define LED1	4	//PA4
#define PWM1	5	//PA5
#define PWM2	6	//PA6
#define PWM3	7	//PA7

//PORT B
#define RESET	0	//PB0
#define STROBE	1	//PB1
#define BUTTON	2	//PB2
//#define LED2	3	//PB3 Not in use

void pwm_init(void);
void pwm_setR(uint8_t val);
void pwm_setG(uint8_t val);
void pwm_setB(uint8_t val);
void adc_init(void);
void adc_setChannel(uint8_t channel);
uint16_t adc_read();
void buttons_init(void);
void led_init(void);
void led_set(int val);
void led_blink(uint8_t num);
void msgeq7_init(void);
void msgeq7_reset(void);
void msgeq7_strobe(void);
uint16_t msgeq7_getVal(void);

#endif /* IO_H_ */