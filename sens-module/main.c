/*
* TSEA29.c
*
* Created: 2015-11-10 16:02:48
* Author : sara
*/

#define F_CPU 15000000UL // Set clock frequency to 15 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <math.h>
#include <i2c_master.h>	// Sensor module is an i2c master.

volatile uint8_t count;
volatile int angular_rate;
volatile unsigned long ms;
volatile float sec;

void adc_init()
{
	ADMUX = (1<<REFS0); // AREF = AVcc
	// ADC Enable and prescale of 64
	// 16000000/64 = 250kHz
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
}

uint16_t adc_read(uint8_t ch)
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
	
	return (ADC);
}


int ch = 3; //ch = 2 = line sensor

uint16_t data_test = 0;
uint8_t mux = 0b00000011;
uint16_t data_out = 0;



/* Timer interrupt:
	256 - (14 745 000 / 1024(prescaler) / 1000(frequency)) = 227
	Set TCNT to 227 and it will overflow once every 2 ms. */
ISR(TIMER0_OVF_vect)
{
	switch(ch)
	{	
		case 2: // line sensor
			PORTD = mux;
			data_out = adc_read(ch);
			mux++;
			if (mux == 7) mux = 0;
			break;
			
		case 3: // IR_sensor front left
			data_out = adc_read(ch);
			break;
		
		case 4: // IR-sensor back left
			data_out = adc_read(ch);
			break;
		
		case 5:
			data_out = adc_read(ch); //IR-sensor front
			break;
		
		case 6: // IR-sensor back right
			data_out = adc_read(ch);
			break;
		case 7: // IR-sensor front right
			data_out = adc_read(ch);
			break;
	}
	
	data_package datap = {ch, data_out};
		
	i2c_write(GENERAL_CALL_ADDRESS, datap);	// Write an entire package to com-module.
	
	ch++;
	if (ch == 8) {
		ch = 2;
	}
	TCNT0 = 241;
}


void initTimerInteruppt() {
	TIMSK = (1<<TOIE0);
	TCNT0 = 241;
	TCCR0 = (1<<CS02)|(1<<CS00);
}



int main(void)
{
	/* Initialize sensor module as I2C master */
	i2c_init_master();
	
	DDRA = 0x00; //PORTA as INPUT
	DDRB = 0xFF; // PORTB as OUTPUT
	DDRD = 0xFF; //PORTD as OUTPUT
	
	initTimerInteruppt();
	adc_init();
	
	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	while(true)
	{
		
	}
}