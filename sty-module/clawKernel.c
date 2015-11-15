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
		/* Set Timer control register for Fast PWM 10-bit */
		TCCR3A = (1<<COM3A1)|(1<<COM3B1)|(1<<WGM31)|(1<<WGM30);
		TCCR3B = (1<<WGM32)|(1<<CS31);
		
		/* Set processor outputs for motor control */
		DDRB = 0x40; // 0100 0000
		
		booted = true;
	}
}

void openClaw() {
	OCR3A = 150;
}

void closeClaw(){
	OCR3A = 113;
}
