/*
 * gyroController.c
 *
 * Created: 2015-11-30 18:46:24
 *  Author: Lukas
 */ 

#include "gyroController.h"
#include "variables.h"
#include <avr/interrupt.h>
#include <avr/io.h>

void initAdc()
{
	ADMUX = (1<<REFS0)|(1<<ADLAR); // AREF = AVcc
	// ADC Enable and prescale of 64
	// 16000000/64 = 250kHz
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
}

volatile uint8_t gv = 0;

uint8_t adcReadGyro(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with ’7? will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1? to ADSC
	cli();
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0? again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	sei();
	return (ADCH);
}


/* Timer interrupt:
	256 - (14 745 000 / 1024(prescaler) / 100(frequency)) = 144
	Set TCNT to 144 and it will overflow once every 10 ms. */
ISR(TIMER0_OVF_vect) {
	updateRegisters(17, adcReadGyro(GYRO) - 2); // reduce by 2 because gyro noice
	TCNT0 = 144;
}

void initTimerInteruppt() {
	TIMSK0 = (1<<TOIE0);
	TCNT0 = 144;
	TCCR0B = (0<<CS02)|(0<<CS00);
}

void initGyro() {
	DDRA = 0x00; // Port A input
	initAdc();
	initTimerInteruppt();
}

void stopGyroInterrupts() {
	cli();
	TCCR0B = (0<<CS02)|(0<<CS00);
	sei();
}

void startGyroInterrupts() {
	cli();
	TCNT0 = 144;
	TCCR0B = (1<<CS02)|(1<<CS00);
	sei();
}