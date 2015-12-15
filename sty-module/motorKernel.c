/************************************************************************
 *																		*
 * Author: Güntech - Lukas Lindqvist/Jacob Johansson					*
 * Purpose: Kernel for the motors										*
 * Language: C															*
 * File type: Source													*
 * Version: 1.0															*
 *																		*
/************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include "motorKernel.h"
#include "variables.h"
#include "gyroController.h"
#include "i2c_slave.h"

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
		TCCR1A |= (1<<WGM11);
		TCCR1B |= (1<<WGM12);
		TCCR1B |= (1<<WGM13);
		// Set CS bits for 8 prescaler
		TCCR1B |= (1<<CS11);
		ICR1 = 921;
		
		/* Set processor outputs for motor control */
		DDRD |= 0x78; // 0111_1000;
		
		booted = true; // Stop from booting again
		
	}
}


void adjust(int u) {
	/* Calculate new Power for motors */
	int nPwrL = 0, nPwrR = 0;
	if(u >= 100) {
		nPwrL = MIN_SPEED;
		nPwrR = MAX_SPEED;
	}else if(u <= -100) {
		nPwrL = MAX_SPEED;
		nPwrR = MIN_SPEED;
	} else {
		nPwrL = 50 - u;
		nPwrR = 50 + u;
		// if they try to use more then 100% power,
		// make them use less.
		if(nPwrL > MAX_SPEED) nPwrL = MAX_SPEED;
		if(nPwrL < MIN_SPEED) nPwrL = MIN_SPEED;
		if(nPwrR > MAX_SPEED) nPwrR = MAX_SPEED;
		if(nPwrR < MIN_SPEED) nPwrR = MIN_SPEED;
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
	if(curPwrLeft > MAX_POWER) curPwrLeft = MAX_POWER;
	if(curPwrRight > MAX_POWER) curPwrRight = MAX_POWER;
	OCR1A = curPwrRight;
	OCR1B = curPwrLeft;
	sei();
}

void driveForward(int powerLeft, int powerRight) {
	PORTD = (0<<3)|(1<<6);
	setMotorSpeed(powerLeft, powerRight);
	if (powerLeft == powerRight)
		cur_steer_cmd = 1;
	else if (powerRight < powerLeft)
		cur_steer_cmd = 4;
	else
		cur_steer_cmd = 5;
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
	curPwrLeft = ZERO_SPEED;
	curPwrRight = ZERO_SPEED;
	setMotorSpeed(ZERO_SPEED, ZERO_SPEED);
	cur_steer_cmd = 2;
}

/* 90-degree left turn */
void turnLeft(int turns) {
	startGyroInterrupts();
	float targetAngle = (int)getCurrentAngle() + (LEFT_TURN * turns);
	while(true) {
		driveRotateLeft(TURN_SPEED, TURN_SPEED);
		if((int)getCurrentAngle() >= targetAngle) {
			stop();
			stopGyroInterrupts();
			return;
		}
		
	}
}

/* 90-degree right turn */
void turnRight(int turns) {
	startGyroInterrupts();
	int targetAngle = (int) getCurrentAngle() - (RIGHT_TURN * turns);
	while(true) {
		driveRotateRight(TURN_SPEED, TURN_SPEED);
		if((int)getCurrentAngle() <= targetAngle) {
			stop();
			stopGyroInterrupts();
			return;
		}
		
	}
}
