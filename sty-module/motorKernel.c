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

/*
	Port 6 = DirLeft, Port 5 = PWMLeft
	Port 3 = DirRight, Port 4 = PWMRight
*/

bool booted = false;
static int currentLeftSpeed = DEFAULT_SPEED;
static int currentRightSpeed = DEFAULT_SPEED;

void initMotor() {
	/* Only initialize motor once */
	if(!booted) {
		cli(); // disable global interrupts
		
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
		
		sei(); // enable global interrupts
	}
}

void goForwardWithCurrentSpeed() {
	driveForward(currentLeftSpeed, currentRightSpeed);
}

void adjustRight() {
	if (currentLeftSpeed < MAX_SPEED && currentLeftSpeed > MIN_SPEED)
	{
		currentLeftSpeed += 2;
		currentRightSpeed -= 2;
		setMotorSpeed(currentLeftSpeed, currentRightSpeed);
	}
}

void adjustLeft() {
	if (currentLeftSpeed < MAX_SPEED && currentLeftSpeed > MIN_SPEED)
	{
		currentLeftSpeed -= 2;
		currentRightSpeed += 2;
		setMotorSpeed(currentLeftSpeed, currentRightSpeed);
	}
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
	cli();
	OCR1A = leftSpeed;
	OCR1B = rightSpeed;
	currentLeftSpeed = leftSpeed;
	currentRightSpeed = rightSpeed;
	sei();
}

void driveForward(int leftSpeed, int rightSpeed) {
	PORTD = (0<<3)|(1<<6);
	setMotorSpeed(leftSpeed, rightSpeed);
}

void driveReverse(int leftSpeed, int rightSpeed) {
	PORTD = (1<<3)|(0<<6);
	setMotorSpeed(leftSpeed, rightSpeed);
}

void driveRotateLeft(int leftSpeed, int rightSpeed) {
	PORTD = (1<<3)|(1<<6);
	setMotorSpeed(leftSpeed, rightSpeed);
}

void driveRotateRight(int leftSpeed, int rightSpeed) {
	PORTD = (0<<3)|(0<<6);
	setMotorSpeed(leftSpeed, rightSpeed);
}

void stop() {
	setMotorSpeed(0, 0);
	currentLeftSpeed = DEFAULT_SPEED;
	currentRightSpeed = DEFAULT_SPEED;
}
