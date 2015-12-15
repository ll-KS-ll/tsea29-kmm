/*
* TSEA29.c
*
* Created: 2015-11-10 16:02:48
* Author : Sara Svensson, Håkan Gudmundsson
*/

#define F_CPU 15000000UL // Set clock frequency to 15 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <math.h>
#include "i2c_master.h"	// Sensor module is an i2c master.
#include "steer_cmd.h"

/* Variables used for converting frontDistance to cm */
float voltsPerUnit = 0.0049;
float mathf;

/* Variable used when reading from PORTA.
* ch changes value at the same rate as the adc-mux
* and decides which PORT0-7 we're reading from */
int ch = 0; 
/* mux is used when reading from line sensor 0-6. It
* decides which one we are getting data from */
uint8_t mux = 0;
/* id is the bus data package identification number */
uint8_t id;
/* data_out is the data that's sent on the bus */
uint16_t data_out = 0;

/* Bool variables turns true when start/autonom button is pressed */
bool start_button_down;
bool auto_button_down;

/* Arrays used for storing line sensor values and calibration values */
static unsigned int sensorBar[] = {0, 0, 0, 0, 0, 0, 0};
static unsigned int sensorBarCalibration[] = {0, 0, 0, 0, 0, 0, 0};

const DIFF = 50;	

/* Initziates the adc mux on PORTA */
void adc_init()
{
	ADMUX = (1<<REFS0); // AREF = AVcc
	// ADC Enable and prescale of 64
	// 16000000/64 = 250kHz
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
}

/* reads the adc value from the selected channel in PORTA */
uint16_t adc_read(uint8_t channel)
{
	// select the corresponding channel 0~7
	// ANDing with ’7? will always keep the value
	// of ‘ch’ between 0 and 7
	channel &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|channel; // clears the bottom 3 bits before ORing
	
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

/* Disables line sensor */
void disable_line_sensor()
{
	PORTB = 0b00000000;
}

/* Enables the chosen line sensor */
void enable_current_linesensor(uint8_t mux)
{
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
 
/* Converts input data from distance sensor to cm.
* Returns an unsigned int representing cm */
unsigned int sideIrToCm(uint16_t data) 
{
	mathf = data;
	mathf = pow((3027.4/mathf), 1.2134);
	if(mathf > 80) mathf = 80;
	if(mathf < 10) mathf = 10;
	return (unsigned int)mathf;
}


/* Updates the line sensor values */
void updateLineSensorValues()
{
	TCCR0 = (0<<CS02)|(0<<CS00);
	
	for(int mux = 0; mux < 7; mux++) {
		enable_current_linesensor(mux);
		PORTD = mux;
		sensorBar[mux] = (unsigned int)adc_read(2);
	}
	
	TCCR0 = (1<<CS02)|(1<<CS00);
}

/* Stores the line sensor values when calibration button is pressed */
void updateLineSensorCalibrationValues()
{
	TCCR0 = (0<<CS02)|(0<<CS00);
	for(int mux = 0; mux < 7; mux++) {
		enable_current_linesensor(mux);
		PORTD = mux;
		sensorBarCalibration[mux] = (unsigned int)adc_read(2);
		
	}
	TCCR0 = (1<<CS02)|(1<<CS00);
}

/* Checks whether robot is on tape or not by iterate
* through sensorBar and check if three or more line sensors
* have higher value than its calibration value. 
* Returns a bool that is true if three or more
* sensors have higher values than their calibration value */ 
bool is_tape(){
	int count = 0;
	unsigned int sensorValue;
	unsigned int calibrationValue;
	
	for(int i = 0; i < 7; i++){
		sensorValue = sensorBar[i];
		calibrationValue = sensorBarCalibration[i];
		
		if(sensorValue >= calibrationValue - DIFF) {
			count++;
		}
		
		if(count > 2) {
			return true;
		}
	}
	return false;
}

/* Iterates through the sensorBar and sets 1 on bit i in an uint16_t if
* the line sensor value is bigger than its calibration value.
* Returns an uint16_t number between 0-255 that shows which line sensors
* that is on tape */ 
uint16_t lineData(){
	uint16_t line_data = 0;
	for(int i = 0; i < 7; i++){
		if(sensorBar[i] >= sensorBarCalibration[i] - DIFF) 
			line_data |= (1<<i);
	}
	return line_data;
}

/* Gravity calculation that decides if the robot should turn left, right or
* go straight while following tape.
* Returns ?? for go forward, ?? for turn left and ?? for turn right */
unsigned int tapeRegulation() {
	unsigned int wrong = 0;
	unsigned int count = 0;
	
	unsigned int sensorValue;
	unsigned int calibrationValue;
	
	for(int i = 0; i < 7; i++){
		sensorValue = sensorBar[i];
		calibrationValue = sensorBarCalibration[i];
		
		if(sensorValue >= calibrationValue - DIFF) {
			wrong += (i+1);
			count++;
		}
	}
	if(count == 0) {
		return 6;
	}
	
	return  10 - (wrong/count);
	
}

/* Interrupt routine that runs every time TIMER0 overflows.
* Has cases that handles that get data from all the different sensors
* and buttons.
* The data is then transported to the other modules through the bus */
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
			updateLineSensorValues();
			
			if(is_tape()) {
				data_out = 1;
			} else {
				data_out = 0;
			}
			id = 18;			
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
		case 8: // Get active command from STYR.
			data_out = sty_steer_cmd();
			id = ch;
			break;
		case 9: // Get remote command from COM.
			data_out = com_steer_cmd();
			id = ch;
			if ( data_out == 8 ) {
				updateLineSensorCalibrationValues();
			}
			break;
		case 10: // Get line data.
			id = ch;
			data_out = lineData();
			break;
		case 11: //
			data_out = (unsigned int) tapeRegulation();
			id = 19;
			break;
	}

	data_package datap = {id, data_out};
	i2c_write(GENERAL_CALL_ADDRESS, datap);
	data_out = 0;
	
	ch++;
	if (ch > 11) {
		ch = 0;
	}
	TCNT0 = 227;
}


/*  Initiates TIMER0 for overflow interrupts every 2 ms.
	256 - (14 745 000 / 1024(prescaler) / 1000(frequency)) = 227
	Set TCNT to 227 and it will overflow once every 2 ms. Lower TCNT with ~15 to increase the timer by 1 ms. */
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
	DDRD = 0xFF; //PORTD0-5 as OUTPUT
	PORTB = 0x00;
	
	initTimerInteruppt();
	adc_init();
	
	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	while(true){
	}
		
}