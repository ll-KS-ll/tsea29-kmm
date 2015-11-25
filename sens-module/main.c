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
	cli();
	ADMUX = (1<<REFS0); // AREF = AVcc
	// ADC Enable and prescale of 16
	// 1000000/16 = 62500
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
	sei();
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
uint8_t turn_on_light(uint8_t mux) 
{
	uint8_t output = 0x00;
	switch(mux)
	{
		case 0:
		output = 0b00000001; //PB0 (line sensor 8)
		break;
		
		case 1:
		output = 0b00000010; //PB1 (line sensor 10)
		break;
		
		case 2:
		output = 0b00000100; //PB2 (line sensor 12)
		break;
		
		case 3:
		output = 0b00001000; //PB3 (line sensor 14)
		break;
		
		case 4:
		output = 0b00010000; //PB4 (line sensor 16)
		break;
		
		case 5:
		output = 0b00100000; //PB5 (line sensor 18)
		break;
		
		case 6:
		output = 0b01000000; //PB6 (line sensor 20)
		break;
	}
	return output;
}

int main(void)
{
	/* Initialize sensor module as I2C master */
	i2c_init_master();
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	_delay_ms(2000); // Chilla lite
	
	
	DDRA = 0x00; //PORTA as INPUT
	DDRB = 0xFF; // PORTB as OUTPUT
	DDRD = 0xFF; //PORTD as OUTPUT
	uint8_t ch = 4; //ch = 2 = line sensor
	adc_init();
	
	float gyro_voltage = 5;
	float gyro_zero_voltage = adc_read(ch);
	float gyro_sensitivity = 0.00667;
	float rotation_threshold = 5;
	float gyro_rate;
	volatile d_angle = 0;
	uint16_t data_out;
	uint8_t distance;
	uint16_t data = 0;
	uint8_t mux = 0x00;
	long tick = 0;
	
	int dist;
	while(1)
	{
		
		switch(ch) 
		{
			case 1: // gyro
				gyro_rate = (adc_read(ch) - gyro_zero_voltage) * gyro_voltage / 1024;
				gyro_rate /= gyro_sensitivity;
				if(gyro_rate >= rotation_threshold || gyro_rate <= -rotation_threshold)
				{
					d_angle += gyro_rate/10;
				}
				data_out = d_angle;
				break;
			case 2: // line sensor, not tested
				PORTB = turn_on_light(mux);
				PORTD = mux;
				data_out = adc_read(ch);
				turn_off_light();
				mux++;
				if (mux == 7)
				{
					mux = 0;
				}
				break;
				
			case 3:
				data_out = adc_read(ch); //IR-sensor v.fram
				break;
			
			case 4:
				data_out = adc_read(ch); //IR-sensor v.bak
				break;
			
			case 5:
				data_out = adc_read(ch); //IR-sensor fram;
				break;
			
			case 6:
				data_out = adc_read(ch); //IR-sensor h.bak
				break;
			
			case 7:
				data_out = adc_read(ch); //IR-sensor h.fram
				break;
		}
		
		//PORTB = data_out;
		data_package datap = {ch, data_out};
		i2c_write(GENERAL_CALL_ADDRESS, datap);	// Write an entire package to com-module.
		
		ch++;
		if (ch == 8)
		{
			ch = 3;
		}
		
		//data++;
		//_delay_ms(2000);
		
		//PORTB = data_out;
		//if (tick == 5)
		/*{
			data_package datap = {'0', data_out};
			i2c_write_package(STY_ADDRESS, datap);
			tick = 0;
		}*/
			// Write an entire package to com-module.
		//tick++;
	}
}

