/*
 * io.h
 *
 * Created: 09/06/2015 22:05:29
 *  Author: Lua
 */ 


#ifndef IO_H_
#define IO_H_

#include <avr/io.h> //needed for uint8_t

enum Color {r,g,b};

//PORT A
#define MSGEQ7	0	//PA0
#define STROBE	1	//PA1
#define RESET	2	//PA2
#define BUTTON	3	//PA3
#define SW		4	//PA4
#define POT		5	//PA5
#define BLUE	6	//PA6
#define GREEN	7	//PA7

#define LED2	1	//PB0
#define LED1	0	//PB1
#define RED		2	//PB2
//PB3 not in use (is reset!)

void pwm_init(void);
void pwm_set(Color col,uint8_t val);

void adc_init(void);
void adc_setChannel(uint8_t channel);
uint8_t adc_getChannel(void);
uint16_t adc_read(void);

void button_init(void);
uint8_t button_pressed(void);

void sw_init(void);
uint8_t sw_status(void);

void led_init(void);
void led_set(uint8_t led,uint8_t val);
void led_blink(uint8_t led,uint8_t num);
void led_setMode(uint8_t mode);
uint8_t led_getMode(void);

void msgeq7_init(void);
uint16_t msgeq7_read(void);

#endif /* IO_H_ */