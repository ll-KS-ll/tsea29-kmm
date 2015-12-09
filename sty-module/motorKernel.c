/************************************************************************
 *																		*
 * Author: G�ntech														*
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
#include "gyroController.h"

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
	if(curPwrLeft > 1000) curPwrLeft = 1000;
	if(curPwrRight > 1000) curPwrRight = 1000;
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
	float targetAngle = (int)getCurrentAngle() + 32;
	while(true) {
		driveRotateLeft(50, 50);
		if((int)getCurrentAngle() >= targetAngle) {
			stop();
			stopGyroInterrupts();
			return;
		}
		
	}
}

/* 90-degree right turn */
void turnRight() {
	startGyroInterrupts();
	int targetAngle = (int) getCurrentAngle() - 34;
	while(true) {
		driveRotateRight(50, 50);
		if((int)getCurrentAngle() <= targetAngle) {
			stop();
			stopGyroInterrupts();
			return;
		}
		
	}
}
