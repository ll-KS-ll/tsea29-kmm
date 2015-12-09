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
float mathf;
unsigned int math;
float voltsperunit = 0.0049;

int ch = 0; //ch = 2 = line sensor
uint16_t data_test = 0;
uint8_t mux = 0b00000000;
uint8_t id;
uint16_t data_out = 0;

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
	// ANDing with 7? will always keep the value
	// of ch between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write 1? to ADSC
	cli();
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes 0? again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	sei();
	
	return (ADC);
}


void disable_line_sensor(){
	PORTB = 0b00000000;
}

void enable_current_linesensor(uint8_t mux){
	switch(mux){
		case 0:
			PORTB = 0b01000000;
			break;
		case 1:
			PORTB = 0b00100000;
			break;
		case 2:
			PORTB = 0b00010000;
			break;
		case 3:
			PORTB = 0b00001000;
			break;
		case 4:
			PORTB = 0b00000100;
			break;
		case 5:
			PORTB = 0b00000010;
			break;
		case 6:
			PORTB = 0b00000001;
			break;
	}
}



/* Timer interrupt:
	256 - (14 745 000 / 1024(prescaler) / 1000(frequency)) = 227
	Set TCNT to 227 and it will overflow once every 2 ms. */
bool start_button_down;
bool auto_button_down;

/* Variables used for converting frontDistance to cm */
float mathf;
float voltsPerUnit = 0.0049;

unsigned int sideIrToCm(uint16_t data) {
	mathf = data;
	mathf = pow((3027.4/mathf), 1.2134);
	if(mathf > 80) mathf = 80;
	if(mathf < 10) mathf = 10;
	return (unsigned int)mathf;
}

ISR(TIMER0_OVF_vect)
{
	switch(ch)
	{	
		case 0://start button
			data_out = 0;
			if(adc_read(ch) > 1000){
				start_button_down = true;				
			} else if(start_button_down) {
				start_button_down = false;
				data_out=1;
			}
			id = ch;
			break;
		case 1: // autonom/remote
			data_out = 0;
			if(adc_read(ch) > 1000){
				auto_button_down = true;
			}
			else if(auto_button_down) {
				auto_button_down = false;
				data_out=1;
		}
		id = ch;
			break;
		case 2: // line sensor
			//enable_current_linesensor(mux);
			PORTD = mux;			
			id = mux+10; //so there is no duplicate for id.
			_delay_ms(10);
			data_out = adc_read(ch);
			//disable_line_sensor();
			mux++;
			if (mux == 7) mux = 0;
			break;
			
		case 3: // IR_sensor front left
			data_out = sideIrToCm((unsigned int)adc_read(ch));
			id = ch;
			break;
		case 4: // IR-sensor back left
			data_out = sideIrToCm((unsigned int)adc_read(ch));
			id = ch;
			break;
		case 5: //IR-sensor front
			mathf = (float)adc_read(ch) * voltsPerUnit;
			mathf = 60.495 * pow(mathf, -1.1904);
			if(mathf < 20) mathf = 20;
			if(mathf > 150) mathf = 150;
			data_out = (unsigned int)mathf;
			id = ch;
			break;
		case 6: // IR-sensor back right
			data_out = sideIrToCm((unsigned int)adc_read(ch));
			id = ch;
			break;
		case 7: // IR-sensor front right
			data_out = sideIrToCm((unsigned int)adc_read(ch));
			id = ch;
			break;
	}
	
	data_package datap = {id, data_out};
	i2c_write(GENERAL_CALL_ADDRESS, datap);	// Write an entire package to com-module.
	
	ch++;
	if (ch == 8) {
		ch = 0;
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
	DDRD = 0xFF; //PORTD0-5 as OUTPUT, PORTD6-7 as INPUT
	PORTB = 0b01111111;
	
	initTimerInteruppt();
	adc_init();
	
	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	while(true){}
}