/************************************************************************
 *																		*
 * Author: Lukas Lindqvist & Jacob Johansson                            *
 * Purpose: Basic driving functions for robot							*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include "motorKernel.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*
* Port 6 = DirLeft, Port 5 = PWMLeft
* Port 3 = DirRight, Port 4 = PWMRight
*/

void initMotor() {
	/* Set Timer control register for Fast PWM 10-bit */
	TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11)|(1<<WGM10);
	TCCR1B = (1<<WGM12)|(1<<CS11);
	
	/* Set processor outputs for motor control */
	DDRD |= 0x78; // 0111_1000;
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
	cli();
	OCR1A = leftSpeed;
	OCR1B = rightSpeed;
	sei();
}

void setForward() {
	PORTD = (1<<3)|(0<<6);
}

void setReverse() {
	PORTD = (0<<3)|(1<<6);
}

void setRotateLeft() {
	PORTD = (1<<3)|(1<<6);
}

void setRotateRight() {
	PORTD = (0<<3)|(0<<6);
}

void stop() {
	setMotorSpeed(0, 0);
}