/*
 * io.cpp
 *
 * Created: 09/06/2015 22:05:18
 *  Author: Lua
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "io.h"

void pwm_init(void){
	//OC0A -> PB2 -> RED
	//OC0B -> PA7 -> GREEN
	//OC1A -> PA6 -> BLUE

	DDRB |= (1 << RED);
	DDRA |= (1 << GREEN) | (1 << BLUE);
	
	//****** COUNTER0 8-bit	
	// Counter 0B clear OC0B at bottom (counting up)
	TCCR0A |= ( 1 << COM0B1);
	TCCR0A |= ( 1 << COM0A1);
	// Set 0xFF as top, update OCRx at bottom, TOV flag on max
	TCCR0A |= ( 1 << WGM01) | ( 1 << WGM00);
	// No prescaling
	TCCR0B |= (1 << CS01); 
	
	//Both output compare already initialized as zero.
	//****** COUNTER1 16-bit
	//Counter 1A and 1B clear at bottom
	TCCR1A |= ( 1 << COM1A1); //| ( 1 << COM1B1);
	TCCR1A &= ~(1 << COM1A0); //| (1 << COM1B0);
	// Fast PWM 8-bit
	TCCR1A |= ( 1 << WGM10);
	TCCR1B |= ( 1 << WGM12);
	// No prescaling
	TCCR1B |= ( 1 << CS10);
}

void pwm_set(Color col,uint8_t val){
	switch(col){
		case r: OCR0A=val;
		case g: OCR0B=val;
		case b: OCR1AL=val;
	}
}

void adc_init(void){
	//Set ADC reference to VCC (00)
	ADMUX &= ~( 1 << REFS0 );
	ADMUX &= ~( 1 << REFS1 );
	//Prescaler 64 -> 125kHz
	//Recommended for 10bit -> 50-200kHz
	ADCSRA |= (1 << ADPS1) | ( 1 << ADPS2);
	//Enable ADC
	ADCSRA |= ( 1 << ADEN);
}

void adc_setChannel(uint8_t channel){
	switch (channel){
		case 0:
			ADMUX &= ~((1 << MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3)|(1<<MUX4)|(1<<MUX5));
			break;
		case 1:
			ADMUX &= ~((1<<MUX1)|(1<<MUX2)|(1<<MUX3)|(1<<MUX4)|(1<<MUX5));
			ADMUX |= (1 << MUX0);
			break;
		case 2:
			ADMUX &= ~((1<<MUX0)|(1<<MUX2)|(1<<MUX3)|(1<<MUX4)|(1<<MUX5));
			ADMUX |= (1<<MUX1);
			break;
		case 3:
			ADMUX &= ~((1<<MUX2)|(1<<MUX3)|(1<<MUX4)|(1<<MUX5));
			ADMUX |= (1<<MUX0)|(1<<MUX1);
			break;
		case 4:
			ADMUX &=~((1 << MUX0)|(1<<MUX1)|(1<<MUX3)|(1<<MUX4)|(1<<MUX5));
			ADMUX |= (1<<MUX2);
		case 5:
			ADMUX &=~((1<<MUX1)|(1<<MUX3)|(1<<MUX4)|(1<<MUX5));
			ADMUX |= (1 << MUX0) | (1<<MUX2);
	}
}

uint16_t adc_read(){
	//Start conversion
	ADCSRA |= ( 1 << ADSC );
	while(ADCSRA & ( 1 << ADSC )); //wait for full buffer
	uint16_t adc_value = 0;
	adc_value = ADCL; // reads 8 bit value
	adc_value |= (ADCH << 8); // reads 8 bit and combines to 16bit
	//result adc_value is 10bit (0-1023)
	return adc_value;
}

void button_init(void){
	DDRA &= ~(1 << BUTTON);
	PORTA|=(1 << BUTTON);
}

void sw_init(void){
	DDRA &= ~(1 << SW);
	PORTA |= (1<<SW);
}

uint8_t sw_status(void){
	uint8_t val = (PINA & (1<<SW));
	return val;
}

uint8_t button_pressed(void){
	static uint8_t pushed=0;
	if (!(PINA & (1 << BUTTON)) & (!pushed)) { //PIN is low and not already pushed
		pushed =1;
		return 1;
	}
	if ((PINA & ( 1 << BUTTON)) && (pushed)){ //is pushed and goes high
		pushed=0;
	}
	return 0;
}

void led_init(void){
	DDRB |= (1 << LED1)|(1 << LED2); //Output
	PORTB &=~(1 <<LED1) | ( 1 << LED2); //Off by default
}

void led_set(uint8_t led, uint8_t val){ //led should be LED1 og LED2
	if (val==0){
		PORTB &=~(1 << led);
	}
	else {
		PORTB |= (1 << led);
	}
}

void led_blink(uint8_t led,uint8_t num){
	if ((num==0)|(num==1)){
		PORTB|=(1 << led);
		_delay_ms(40);
		PORTB&=~(1 << led);
		_delay_ms(10);
	}
	else {
		for (int i=0;i<num;i++){
			PORTB|=(1 << led);
			_delay_ms(40);
			PORTB&=~(1 << led);
			_delay_ms(250);
		}
	}
}

void msgeq7_init(void){
		//Strobe and reset as outputs
		DDRA |= (1 << RESET) | (1 << STROBE);
		DDRA &=~(1<<MSGEQ7);
		//Reset chip
		PORTA |= (1 << RESET);
		PORTA &= ~(1 << RESET);
}

void msgeq7_reset(void){
	//RESET, high -> low
	PORTA |= (1 << RESET);
	_delay_us(100);
	PORTA &= ~(1 << RESET);
	_delay_us(200);	
}

void msgeq7_strobe(void){
	PORTA |= (1 << STROBE); //high
	_delay_us(20);
	PORTA &= ~(1 << STROBE); //low
	_delay_us(40); //specs say 36 microseconds
}

uint16_t msgeq7_getVal(void){
	adc_setChannel(MSGEQ7);
	return adc_read();
}