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
	DDRA |= ( 1 << PWM1) | (1 << PWM2) | (1 << PWM3);
	//****** COUNTER0 8-bit	
	// Counter 0B clear OC0B at bottom (counting up)
	TCCR0A |= ( 1 << COM0B1);
	// Set 0xFF as top, update OCRx at bottom, TOV flag on max
	TCCR0A |= ( 1 << WGM01) | ( 1 << WGM00);
	// No prescaling
	TCCR0B |= (1 << CS01); 
	
	//Both output compare already initialized as zero.
	//****** COUNTER1 16-bit
	//Counter 1A and 1B clear at bottom
	TCCR1A |= ( 1 << COM1A1) | ( 1 << COM1B1);
	TCCR1A &= ~(1 << COM1A0) | (1 << COM1B0);
	// Fast PWM 8-bit
	TCCR1A |= ( 1 << WGM10);
	TCCR1B |= ( 1 << WGM12);
	// No prescaling
	TCCR1B |= ( 1 << CS10 );
}

void pwm_setR(uint8_t val){
	OCR1BL = val;
}

void pwm_setG(uint8_t val){
	OCR1AL = val;
}

void pwm_setB(uint8_t val){
	OCR0B = val;
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
	}
}

uint16_t adc_read(){
	//Start conversion
	ADCSRA |= ( 1 << ADSC );
	while(ADCSRA & ( 1 << ADSC ));
	uint16_t adc_value = 0;
	adc_value = ADCL; // reads 8 bit value
	adc_value |= (ADCH << 8); // reads 8 bit and combines to 16bit
	//result adc_value is 10bit (0-1023)
	return adc_value;
}

void buttons_init(void){
	DDRB&=~(1 << BUTTON);
	PORTB|=(1 << BUTTON);
	//setup interupts on INT0
	MCUCR|=(1 << ISC01);
	MCUCR&=~(1<< ISC00); //Interupt on rising edge
	GIMSK |= (1 << INT0);
	//interuptrutine in disco.cpp int0_vect	
}

void led_init(void){
	DDRA |= (1 << LED1); //Output
	PORTA &=~(1 <<LED1); //Off by default
}

void led_set(int val){
	if (val==0){
		PORTA &=~(1 << LED1);
	}
	else {
		PORTA |= (1 << LED1);
	}
}

void led_blink(uint8_t num){
	if ((num==0)|(num==1)){
		PORTA|=(1 << LED1);
		_delay_ms(40);
		PORTA&=~(1 << LED1);
		_delay_ms(10);
	}
	else {
		for (int i=0;i<num;i++){
			PORTA|=(1 << LED1);
			_delay_ms(40);
			PORTA&=~(1 << LED1);
			_delay_ms(250);
		}
	}
}

void msgeq7_init(void){
		//Strobe and reset as outputs
		DDRB |= (1 << RESET) | (1 << STROBE);
		//Reset chip
		PORTB |= (1 << RESET);
		PORTB &= ~(1 << RESET);
}

void msgeq7_reset(void){
	//RESET, high -> low
	PORTB |= (1 << RESET);
	_delay_us(100);
	PORTB &= ~(1 << RESET);
	_delay_us(200);	
}

void msgeq7_strobe(void){
	PORTB |= (1 << STROBE); //high
	_delay_us(2);
	PORTB &= ~(1 << STROBE); //low
	_delay_us(40); //specs say 36 microseconds
}

uint16_t msgeq7_getVal(void){
	adc_setChannel(MSGEQ7);
	return adc_read();
}