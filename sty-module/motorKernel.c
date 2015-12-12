/************************************************************************
 *																		*
 * Author: Güntech														*
 * Purpose: Kernel for the motors										*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "motorKernel.h"
#include "variables.h"
#include "i2c_slave.h"

/*
	Port 6 = DirLeft, Port 5 = PWMLeft
	Port 3 = DirRight, Port 4 = PWMRight
*/

static bool booted = false;
static int curPwrLeft = 0;
static int curPwrRight = 0;

void initMotor() {
	/* Only initialize motor once */
	if(!booted) {
		
		// Set compare output mode to non-inverted
		TCCR1A |= (1<<COM1A1);
		TCCR1A |= (1<<COM1B1);
		// Set WGM for 10-bit Fast PWM
		TCCR1A |= (1<<WGM10);
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM12);
		// Set CS bits for 8 prescaler
		TCCR1B |= (1<<CS11);
		
		/* Set processor outputs for motor control */
		DDRD |= 0x78; // 0111_1000;
		
		booted = true; // Stop from booting again
		
	}
}


void adjust(int u) {
	/* Calculate new Power for motors */
	int nPwrL = 0, nPwrR = 0;
	if(u >= 100) {
		nPwrL = 10;
		nPwrR = 90;
	}else if(u <= -100) {
		nPwrL = 90;
		nPwrR = 10;
	} else {
		nPwrL = 50 - u;
		nPwrR = 50 + u;
		// if they try to use more then 100% power,
		// make them use less.
		if(nPwrL > 100) nPwrL = 90;
		if(nPwrL < 0) nPwrL = 10;
		if(nPwrR > 100) nPwrR = 90;
		if(nPwrR < 0) nPwrR = 10;
	}
	
	// set new power as current power
	curPwrLeft = nPwrL;
	curPwrRight = nPwrR;

	driveForward(curPwrLeft, curPwrRight);
}

void setMotorSpeed(int powerLeft, int powerRight) {
	cli();
	curPwrLeft = (TOTAL_POWER / 100) * powerLeft;
	curPwrRight = (TOTAL_POWER / 100) * powerRight;
	// Make sure the pwm signal to the motors doesnt go above 1000
	if(curPwrLeft > 1000) curPwrLeft = 1000;
	if(curPwrRight > 1000) curPwrRight = 1000;
	OCR1A = curPwrRight;
	OCR1B = curPwrLeft;
	sei();
}

void driveForward(int powerLeft, int powerRight) {
	PORTD = (0<<3)|(1<<6);
	setMotorSpeed(powerLeft, powerRight);
	cur_steer_cmd = 1;
}

void driveReverse(int powerLeft, int powerRight) {
	PORTD = (1<<3)|(0<<6);
	setMotorSpeed(powerLeft, powerRight);
	cur_steer_cmd = 3;
}

void driveRotateLeft(int powerLeft, int powerRight) {
	PORTD = (1<<3)|(1<<6);
	setMotorSpeed(powerLeft, powerRight);
	cur_steer_cmd = 7;
}

void driveRotateRight(int powerLeft, int powerRight) {
	PORTD = (0<<3)|(0<<6);
	setMotorSpeed(powerLeft, powerRight);
	cur_steer_cmd = 6;
}

void stop() {
	curPwrLeft = 0;
	curPwrRight = 0;
	setMotorSpeed(0, 0);
	cur_steer_cmd = 2;
}
