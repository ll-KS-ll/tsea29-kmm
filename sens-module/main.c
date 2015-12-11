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

int ch = 0;
uint16_t data_test = 0;
uint8_t id;
uint16_t data_out = 0;
const int DIFF = 50;
static uint16_t calibrated_sensor_bar[] = {783, 389, 460, 393, 364, 330, 353};
static uint16_t current_sensor_values[] = {0, 0, 0, 0, 0, 0, 0};
bool on_tape = false;
bool cal_sb = true;

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

unsigned int sideIrToCm(uint16_t data) {
	volatile unsigned int math = 0;
	math = data;
	math = (6050 / math);
	
	if(math >= 2) {
		math -= 2; // fix linear error (-2)
	}
	if(math < 10) {
		math = 10; // min limit at 10cm
	}
	if(math > 80) {
		math = 80; // max limit at 80cm
	}
	return math;
}

/* Timer interrupt:
	256 - (14 745 000 / 1024(prescaler) / 500(frequency)) = 227
	Set TCNT to 227 and it will overflow once every 2 ms. */
bool start_button_down;
bool auto_button_down;

void calibrate_sensor_bar()
{
	cal_sb = true;
}

uint16_t tape_regulator(){
	uint16_t sensor_value;
	int numerator=0;
	int denominator=0;
	//bool arrived = true;
	volatile float fault=0;
	
	volatile int sara=0;
	volatile int svensson=0;
	
	for(int i=0; i<7; i++){
		sensor_value = current_sensor_values[i];
		/*if((sensor_value < calibrated_sensor_bar[i]+DIFF) && arrived)
		{
			arrived = false;
		}*/
		numerator += sensor_value*(i+1);
		sara=sensor_value*(i+1);
		sara++;
		denominator += sensor_value;
		svensson=sensor_value;
		svensson++;
	}
	fault = (numerator/denominator);
	fault++;
	//if(arrived) return 1;
	if(fault<4){
		return 2; //2 = left
	}
	else if(fault>4.2){
		return 3; //3 = right
	}
	else{
		return 4; //4 = forwárd
	}
}

ISR(TIMER0_OVF_vect)
{
	TCCR0 = (0<<CS02)|(0<<CS00); // stop clock as to not generete new interrupt inside interrupt
	uint8_t id;
	
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
			id = ch;
			volatile uint16_t test=0;
			volatile uint16_t hej=0;
			//PORTB=0xFF;
		/*	if(cal_sb)
			{
				cal_sb=false;
				for (int i=0; i<7; i++)
				{
					PORTD = i;
					calibrated_sensor_bar[i] = adc_read(ch);
					test = calibrated_sensor_bar[i];
					test++;
				}
			}
			else{*/
				for (uint8_t mux=0; mux<7; mux++)
				{
					enable_current_linesensor(mux);
					PORTD = mux;
					current_sensor_values[mux] = adc_read(ch);
					hej=current_sensor_values[mux];
					hej++;
					//if(current_sensor_values[mux] > calibrated_sensor_bar[mux]+DIFF) on_tape = true; //can't be here since this wont be true before it's calibrated
				}
			//}
			/*if (cal_sb)
			{
				strncpy(calibrated_sensor_bar,current_sensor_values,7);
				cal_sb = false;
			}*/
			
			//if(on_tape)
			//{
				//on_tape = false;
				data_out = tape_regulator();
			//}
			//else data_out = 0; 
			disable_line_sensor();
			break;
			
		case 3: // IR_sensor front left
			data_out = sideIrToCm(adc_read(ch));
			id = ch;
			break;
		
		case 4: // IR-sensor back left
			data_out = sideIrToCm(adc_read(ch));
			id = ch;
			break;
		
		case 5:
			mathf = 0;
			mathf = (float)adc_read(ch) * voltsperunit; //IR-sensor front
			mathf = 60.495 * pow(mathf, -1.1904);
			if(mathf >= 2) math += 2; // fix linear error (+2)
			if(mathf < 20) math = 20; // min limit at 10cm
			if(mathf > 150) math = 150; // max limit at 80cm
			data_out = (unsigned int)mathf;
			id = ch;
			break;
		
		case 6: // IR-sensor back right
			data_out = sideIrToCm(adc_read(ch));
			id = ch;
			break;
		case 7: // IR-sensor front right
			data_out = sideIrToCm(adc_read(ch));
			id = ch;
			break;
	}
	
	data_package datap = {id, data_out};
		
	i2c_write(STY_ADDRESS, datap);	// Write an entire package to com-module.
	ch++;
	if (ch == 8) ch = 0;
	TCNT0 = 227;
	TCCR0 = (1<<CS02)|(1<<CS00);
}

void initTimerInteruppt() {
	TIMSK = (1<<TOIE0);
	TCNT0 = 227;
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
	
	_delay_ms(500);
	volatile uint8_t test;
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	while(true){}
}