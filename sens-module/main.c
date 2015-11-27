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
	
	//char cSREG; <-- disable interrupt
	//cSREG=SREG;
	
	
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
	// Clock = 14,745 Mhz
	// 14,745 Mhz / 1024 = 14,3994140625 Khz
	// 14,3994140625 Khz / 256 = 56,2477111816 Hz
	
	/* Initialize sensor module as I2C master */
	i2c_init_master();
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	//_delay_ms(2000); // Chilla lite
	
	DDRA = 0x00; //PORTA as INPUT
	DDRB = 0xFF; // PORTB as OUTPUT
	DDRD = 0xFF; //PORTD as OUTPUT
	PORTB = 0xFF; // turns on lights on line sensor
	adc_init();
	timer_init();
	
	uint8_t ch = 1; //ch = 1 = angular rate sensor
	float gyro_voltage = 5;
	float gyro_zero_voltage = adc_read(1);
	float gyro_sensitivity = 26.67;
	float rotation_threshold = 5;
	float gyro_rate;
	int d_angle = 0;
	uint16_t data_out = 0;
	uint8_t mux = 0x00;
	uint16_t time_s = 0;
	
	_delay_ms(2000);
	while(1)
	{
		switch(ch) 
		{
			case 1: // gyro
				clear_ms_counter();
				gyro_rate = ((adc_read(ch) - gyro_zero_voltage) * gyro_voltage / 1024) * 1000;
				gyro_rate /= gyro_sensitivity;
				time_s = millis();
				time_s = time_s+1;
				PORTB = time_s;
				//gyro_rate *= 100;
				if(gyro_rate >= rotation_threshold || gyro_rate <= -rotation_threshold)
				{
					d_angle += gyro_rate;
				}
				data_out = d_angle;
				//_delay_ms(100);
				//PORTB = data_out;
				break;
			
			case 2: // line sensor
				PORTD = mux;
				data_out = adc_read(ch);
				mux++;
				if (mux == 7)
				{
					mux = 0;
				}
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
		
		//ch++;
		if (ch == 8)
		{
			ch = 1;
		}		
	}
}

