	/************************************************************************
 *																		*
 * Author: Güntech														*
 * Purpose: Kernel for the claw											*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "clawKernel.h"

static bool booted = false;

void initClaw() {
	/* Only initialize claws once */
	if(!booted) {		
		cli();
		
		// Set compare output mode to non-inverted
		TCCR3A |= (1<<COM3A1);
		TCCR3A |= (1<<COM3B1);
		// Set WGM for 10-bit Fast PWM
		TCCR3A |= (1<<WGM30);
		//TCCR3A |= (0<<WGM31);
		TCCR3B |= (1<<WGM32);
		// Set CS bits for 8 prescaler
		TCCR3B |= (1<<CS30);
		TCCR3B |= (1<<CS31);
		
		/* Set processor outputs for motor control */
		DDRB = 0x40; // 0100 0000
		
		booted = true;
		
		sei();
	}
}

void stopClaw(){
	TCCR3B |= (0<<CS30);
	TCCR3B |= (0<<CS31);
}
void openClaw() {
	OCR3A = 200;
}

void closeClaw(){
	OCR3A = 150;
}