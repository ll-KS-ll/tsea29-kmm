/*
* TSEA29.c
*
* Created: 2015-11-10 16:02:48
* Author : hakan
*/
#define F_CPU 16000000UL // Set clock frequency to 1 MHz

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

void adc_init()
{
	ADMUX = (1<<REFS0); // AREF = AVcc
	// ADC Enable and prescale of 16
	// 1000000/16 = 62500
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0);
}

uint16_t adc_read(uint8_t ch)
{
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

/*
turns off the lights on the line sensor
*/
void turn_off_light(void)
{
	PORTB = 0x00;
}

/*
turns on one light on line sensor depending on which channel
is chosen from the mux
*/
void turn_on_light(uint8_t mux) 
{
	uint8_t output = 0x00;
	switch(mux)
	{
		case 0:
		output = 0b00000001;
		break;
		
		case 1:
		output = 0b00000010;
		break;
		
		case 2:
		output = 0b00000100;
		break;
		
		case 3:
		output = 0b00001000;
		break;
		
		case 4:
		output = 0b00010000;
		break;
		
		case 5:
		output = 0b00100000;
		break;
		
		case 6:
		output = 0b01000000;
		break;
		
		case 7:
		output = 0b10000000;
		break;
	}
	PORTB = output;
}

int main(void)
{
	/* Initialize sensor module as I2C master */
	//i2c_init_master();

	DDRA = 0x00; //PORTA as INPUT
	DDRB = 0xFF; // PORTB as OUTPUT
	DDRD = 0xFF; //PORTD as OUTPUT
	uint8_t ch = 0b00000101; //ch = 5 = angular rate sensor
	adc_init();
	
	float gyro_voltage = 5;
	float gyro_zero_voltage = adc_read(ch);
	float gyro_sensitivity = 0.00667;
	float rotation_threshold = 5;
	float gyro_rate;
	int d_angle = 0;
	uint16_t data_out;
	uint8_t mux = 0;
	long tick = 0;
	
	while(1)
	{
		switch(ch) 
		// case 0-4 IR sensors
		// case 5 gyro
		// case 6 line sensor
		
		{
			case 0:
			data_out = adc_read(ch);
			break;
			
			case 1:
			data_out = adc_read(ch);
			break;
			
			case 2:
			data_out = adc_read(ch);
			break;
			
			case 3:
			data_out = adc_read(ch);
			break;
			
			case 4:
			data_out = adc_read(ch);
			break;
			
			case 5: // read gyro
			gyro_rate = (adc_read(ch) - gyro_zero_voltage) * gyro_voltage / 1024;
			gyro_rate /= gyro_sensitivity;
			if(gyro_rate >= rotation_threshold || gyro_rate <= -rotation_threshold)
			{
				d_angle += gyro_rate/10;
			}
			data_out = d_angle;
			break;
			
			case 6: // not tested
			turn_on_light(mux);
			PORTD = mux;
			data_out = adc_read(ch);
			turn_off_light();
			mux++;
			if (mux == 8)
			{
				mux = 0;
			}
		}
		/*ch++;
		if (ch == 3)
		{
			ch = 1;
		}
		*/
		PORTB = data_out;
		if (tick == 5)
		/*{
			data_package datap = {'0', data_out};
			i2c_write_package(STY_ADDRESS, datap);
			tick = 0;
		}*/
			// Write an entire package to com-module.
		tick++;
	}
}

