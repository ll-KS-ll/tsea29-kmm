/*
* TSEA29.c
*
* Created: 2015-11-10 16:02:48
* Author : sara
*/

#define F_CPU 15000000UL // Set clock frequency to 15 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
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

ISR(TIMER0_OVF_vect)
{
	// 14 745 000 Hz / 1000 = 14 745 Hz
	// 14 745 / 8 / 256 = 7,1997070313
	// TIMER0 will overflow ca 7 times per ms
	count++;
	if (count == 7)
	{
		ms++;
		count = 0;
	}
}

unsigned long millis()
{
	return ms;
}

void timer_init()
{
	TIMSK = (1<<TOIE0);
	TCNT0 = 0x00;
	TCCR0 = (1<<CS01);
}

void clear_ms_counter()
{
	ms = 0;
}


int main(void)
{
	/* Initialize sensor module as I2C master */
	i2c_init_master();
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	//_delay_ms(2000); // Chilla lite
	
	DDRA = 0x00; //PORTA as INPUT
	DDRB = 0xFF; // PORTB as OUTPUT
	DDRD = 0xFF; //PORTD as OUTPUT
	//PORTB = 0xFF; // turns on lights on line sensor
	timer_init();
	adc_init();
	
	uint8_t ch = 1; //ch = 2 = line sensor
	float gyro_voltage = 5;
	unsigned char gyro_zero_voltage = adc_read(1) >> 2;
	float gyro_sensitivity = 26.67;
	float rotation_threshold = 5;
	unsigned char gyro_value;
	float gyro_rate;
	int d_angle_value = 2000;
	float buffer = 0;
	uint16_t data_test = 0;
	uint8_t mux = 0b00000011;
	uint16_t data_out = 0;	
	
	_delay_ms(2000);
	
	while(1)
	{
		switch(ch) 
		{
			case 1: // gyro
				gyro_value = adc_read(ch) >> 2;
				gyro_rate = (gyro_value - 125);
				buffer += gyro_rate / 100;
				if (buffer >= 1 || buffer <= -1)
				{
					d_angle_value += buffer;
					buffer = 0;
				}
				data_out = d_angle_value;
				break;
			
			case 2: // line sensor
				PORTD = mux;
				data_out = adc_read(ch);
				mux++;
				if (mux == 7) mux = 0;
				break;
				
			case 3:
				data_out = adc_read(ch); //IR-sensor front left
				break;
			
			case 4:
				data_out = adc_read(ch); //IR-sensor back left
				break;
			
			case 5:
				data_out = adc_read(ch); //IR-sensor front
				break;
			
			case 6:
				data_out = adc_read(ch); //IR-sensor back right
				break;
		}
		data_package datap = {ch, data_out};
		i2c_write(GENERAL_CALL_ADDRESS, datap);	// Write an entire package to com-module.
		/*if (ch == 1 && (data_out > 2700 || data_out < 1000))
		{
			d_angle = 2000;
		}*/
		ch++;
		if (ch == 7) ch = 1;
	}
}

