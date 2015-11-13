/************************************************************************
 *																		*
 * Author: Lukas Lindqvist & Jacob Johansson                            *
 * Purpose: Drive robot through labyrinth								*
 * Language: C															*
 *																		*
/************************************************************************/

#define F_CPU 15000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <i2c_slave.h> // Is slave module
#include "autonomous.h"
#include "boot.h"


//Inte klart än
void initClawPWM() {
	
	//set clock
	TCCR3A |= (1<<COM3A1)|(1<<COM3B1)|(1<<WGM30);
	TCCR3B |= (1<<WGM32)|(1<<CS31)|(1<<CS30);
	
	//set output pin
	DDRB |= 0x40; // 0100 0000
}

void openClaw() {
	OCR3A = 150;
}

void closeClaw(){
	OCR3A = 113;
}

void testClaw(){
	openClaw();
}


int main(void)
{
	
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	/* Boot Claw-/Motor-kernel */
	boot();
	
	
	/* Main loop */
	while (1)
	{
		/* The data package datap is automatically read when sens-module sends new data. */
		
		/* Example of how to get stuff from data packages. */
		uint8_t pid = datap.id;
		uint16_t pdata = datap.data;
		
		_delay_ms(2000);	// Chilla lite
	}
}

