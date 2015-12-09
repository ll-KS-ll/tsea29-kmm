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
#include <util/delay.h>
#include "motorKernel.h"
#include "variables.h"
#include "gyroController.h"

/*
	Port 6 = DirLeft, Port 5 = PWMLeft
	Port 3 = DirRight, Port 4 = PWMRight
*/

static bool booted = false;
static int curPwrLeft = 0;
static int curPwrRight = 0;

/*
PWM freq = 2000 that is 2kHz;


*/

void initMotor() {
	/* Only initialize motor once */
	if(!booted) {
		
		// Set compare output mode to non-inverted
		TCCR1A |= (1<<COM1A1);
		TCCR1A |= (1<<COM1B1);
		// Set WGM for 10-bit Fast PWM
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM12);
		TCCR1B |= (1<<WGM13);
		// Set CS bits for 8 prescaler
		TCCR1B |= (1<<CS11);
		ICR1 = 920;
		
		/* Set processor outputs for motor control */
		DDRD |= 0x78; // 0111_1000;
		
		booted = true; // Stop from booting again
		
	}
}


void adjust(int u) {
	/* Calculate new Power for motors */
	int nPwrL = 0, nPwrR = 0;
	if(u >= 100) {
		nPwrL = 35;
		nPwrR = 65;
	}else if(u <= -100) {
		nPwrL = 65;
		nPwrR = 35;
	} else {
		nPwrL = 50 - u;
		nPwrR = 50 + u;
		// if they try to use more then 100% power,
		// make them use less.
		if(nPwrL > 65) nPwrL = 65;
		if(nPwrL < 35) nPwrL = 35;
		if(nPwrR > 65) nPwrR = 65;
		if(nPwrR < 35) nPwrR = 35;
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
	if(curPwrLeft > 900) curPwrLeft = 900;
	if(curPwrRight > 900) curPwrRight = 900;
	OCR1A = curPwrRight;
	OCR1B = curPwrLeft;
	sei();
}

void driveForward(int powerLeft, int powerRight) {
	PORTD = (0<<3)|(1<<6);
	setMotorSpeed(powerLeft, powerRight);
}

void driveReverse(int powerLeft, int powerRight) {
	PORTD = (1<<3)|(0<<6);
	setMotorSpeed(powerLeft, powerRight);
}

void driveRotateLeft(int powerLeft, int powerRight) {
	PORTD = (1<<3)|(1<<6);
	setMotorSpeed(powerLeft, powerRight);
}

void driveRotateRight(int powerLeft, int powerRight) {
	PORTD = (0<<3)|(0<<6);
	setMotorSpeed(powerLeft, powerRight);
}

void stop() {
	curPwrLeft = 0;
	curPwrRight = 0;
	setMotorSpeed(0, 0);
}

/* 90-degree left turn */
void turnLeft() {
	startGyroInterrupts();
	_delay_ms(500);
	float targetAngle = (int)getCurrentAngle() + LEFT_TURN;
	while(true) {
		driveRotateLeft(50, 50);
		if((int)getCurrentAngle() >= targetAngle) {
			stop();
			stopGyroInterrupts();
			_delay_ms(500);
			return;
		}
		
	}
}

/* 90-degree right turn */
void turnRight() {
	startGyroInterrupts();
	_delay_ms(500);
	int targetAngle = (int) getCurrentAngle() - RIGHT_TURN;
	while(true) {
		driveRotateRight(50, 50);
		if((int)getCurrentAngle() <= targetAngle) {
			stop();
			stopGyroInterrupts();
			_delay_ms(500);
			return;
		}
		
	}
}
