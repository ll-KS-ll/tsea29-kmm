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

static bool booted = false;

void initMotor() {
	/* Only initialize motor once */
	if(!booted) {
		cli(); // disable global interrupts
		
		// Set compare output mode to non-inverted
		TCCR1A |= (1<<COM1A1);
		TCCR1A |= (1<<COM1B1);
		// Set WGM for 10-bit Fast PWM
		TCCR1A |= (1<<WGM11);
		TCCR1A |= (1<<WGM10);
		TCCR1B |= (1<<WGM12);
		// Set CS bits for 8 prescaler
		TCCR1B |=(1<<CS11);
		
		/* Set processor outputs for motor control */
		DDRD |= 0x78; // 0111_1000;
		
		booted = true; // Stop from booting again
		
		sei(); // enable global interrupts
	}
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

void driveForward() {
	setForward();
	setMotorSpeed(250, 250);
}