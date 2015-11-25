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

/*
	Port 6 = DirLeft, Port 5 = PWMLeft
	Port 3 = DirRight, Port 4 = PWMRight
*/
static int currentLeftSpeed;
static int currentRightSpeed;
static bool booted = false;

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

void adjustRight() {
	currentLeftSpeed += 2;
	currentRightSpeed -= 2;
	setMotorSpeed(currentLeftSpeed, currentRightSpeed);
}

void adjustLeft() {
	currentLeftSpeed -= 2;
	currentRightSpeed += 2;
	setMotorSpeed(currentLeftSpeed, currentRightSpeed);
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
}
