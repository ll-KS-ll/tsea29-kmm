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
#include <avr/delay.h>
#include "clawKernel.h"

static bool booted = false;

/* One tick should be 20 ms 
	PWM_freq = fclk/prescaler*256(clock bit length)
	 */
void initClaw() {
	/* Only initialize claws once */
	if(!booted) {		
		cli();
		
		// Set compare output mode to non-inverted
		TCCR3A |= (1<<COM3A1);
		TCCR3A |= (1<<COM3B1);
		// Set WGM for 10-bit Fast PWM
		TCCR3A |= (1<<WGM31);
		TCCR3B |= (1<<WGM32);
		TCCR3B |= (1<<WGM33);
		// Set CS bits for 8 prescaler
		TCCR3B |= (1<<CS30);
		TCCR3B |= (1<<CS32);
		ICR3 = 286;
		
		/* Set processor outputs for motor control */
		DDRB = 0xC0; // 1100 0000
		
		booted = true;
		
		sei();
	}
	openClaw();
	raiseClaw();
}

void stopClaw(){
	cli();
	TCCR3B = (0<<CS30);
	TCCR3B = (0<<CS32);
	sei();
}
void openClaw() {
	OCR3A = 24;
}

void closeClaw(){
	OCR3A = 10;
}

void raiseClaw() {
	OCR3B = 23;
}

void lowerClaw() {
	OCR3B = 17;
}