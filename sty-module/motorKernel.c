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
static int curPwrLeft = 0;
static int curPwrRight = 0;

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

/*

GÖR SÅ ATT ISTÄLLET FÖR ATT ÄNDRA HASTIGHETEN SÅ ÄNDRAS FARTEN
TILL EN PROCENT AV EN TOTAL HASTIGHET. HJULEN FÅR TILLSAMMANS ALDRIG GÅ ÖVER 700
MEN SKA ALLTID VARA 700 NÄR MAN ADDERAR DE TVÅ



*/

// Skriv om, mycket skärp-kod
void adjust(int u) {
	int temp = 0;
	if(u > 100) {
		curPwrLeft = 10;
		curPwrRight = 90;
	}else if(u < -100) {
		curPwrLeft = 90;
		curPwrRight = 10;
	} else {
		int nPwrL = 50 - u;
		int nPwrR = 50 + u;
		if(nPwrL > 100) nPwrL = 90;
		if(nPwrL < 0) nPwrL = 10;
		if(nPwrR > 100) nPwrR = 90;
		if(nPwrR < 0 ) nPwrR = 10;
		curPwrLeft = nPwrL;
		curPwrRight = nPwrR;
	}
	driveForward(curPwrLeft, curPwrRight);
}

void setMotorSpeed(int powerLeft, int powerRight) {
	cli();
	curPwrLeft = (TOTAL_POWER / 100) * powerLeft;
	curPwrRight = (TOTAL_POWER / 100) * powerRight;
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
