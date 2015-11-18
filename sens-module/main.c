/*
* TSEA29.c
*
* Created: 2015-11-10 16:02:48
* Author : hakan
*/
#define F_CPU 1000000UL // Set clock frequency to 1 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <math.h>
#include <i2c_master.h>	// Sensor module is a i2c master.

volatile uint8_t count;
volatile int angular_rate;
volatile unsigned long ms;
volatile float sec;

void adc_init(){
	ADMUX = (1<<REFS0); // AREF = AVcc
	// ADC Enable and prescale of 16
	// 1000000/16 = 62500
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0);
}

uint16_t adc_read(uint8_t ch){
	// select the corresponding channel 0~7
	// ANDing with ’7? will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	//char cSREG; <-- disable interrupt
	//cSREG=SREG;
	
	
	// start single convertion
	// write ’1? to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0? again
	// till then, run loop continuously
	// interrupt forbidden??
	//cli();
	while(ADCSRA & (1<<ADSC));
	//sei();
	//SREG=cSREG; <---disable interrupt
	
	return (ADC);
}

int main(void)
{
	/* Initialize sensor module as I2C master */
	i2c_init_master();

	DDRA = 0x00; //PORTA as INPUT
	DDRB = 0xFF; // PORTB as OUTPUT
	DDRD = 0xFF; //PORTD as OUTPUT
	uint16_t angle;
	uint8_t ch = 0b00000101; //ch = 5 = angular rate sensor
	adc_init();
	
	float gyro_voltage = 5;
	float gyro_zero_voltage = adc_read(ch);
	float gyro_sensitivity = 0.00667;
	float rotation_threshold = 5;
	float gyro_rate;
	float d_angle = 0;
	uint16_t data_out;
	uint8_t distance;
	
	while(1)
	{
		switch(ch) 
		// case 0-3 distance sensors
		// case 5 gyro
		
		{
			case 0:
			data_out = adc_read(ch);
			data_out = (data_out >> 2); // shifts out the 2 lsb so we have an 8bit representation
			//_delay_ms(40);
			break;
			case 1:
			data_out = adc_read(ch);
			data_out = (data_out >> 2);
			//data_out &= 0xF8;
			_delay_ms(40);
			break;
			case 2:
			data_out = adc_read(ch);
			data_out = (data_out >> 2);
			//data_out &= 0xF8;
			_delay_ms(40);
			break;
			case 3:
			data_out = adc_read(ch);
			data_out = (data_out >> 2);
			//data_out &= 0xF8;
			_delay_ms(40);
			break;
			case 4:
			case 5:
			gyro_rate = (adc_read(ch) - gyro_zero_voltage) * gyro_voltage / 1024;
			gyro_rate /= gyro_sensitivity;
			if(gyro_rate >= rotation_threshold || gyro_rate <= -rotation_threshold)
			{
				d_angle += gyro_rate/1000;
			}
			data_out = (uint8_t)d_angle;
			break;
		}
		//ch++;
		if (ch == 3)
		{
			ch = 1;
		}
		
		PORTB = data_out;
		data_package datap = {'0', data_out};
		i2c_write_package(STY_ADDRESS, datap);	// Write an entire package to com-module.
		_delay_ms(500);
	}
}

