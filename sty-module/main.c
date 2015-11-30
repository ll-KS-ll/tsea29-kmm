/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Drive robot through labyrinth								*
 * Language: C															*
 *																		*
/************************************************************************/

#define F_CPU 15000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "i2c_slave.h" // Is slave module
#include "autonomous.h"
#include "boot.h"
#include "sensorValues.h"
#include "variables.h"

// temporary, only for testing

int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	/* Boot Claw-/Motor-kernel */
	boot();
	
	_delay_ms(2000);
	
	//driveForward(50, 50);
	//exploreLabyrinth();
	
	
	volatile f = 0;
	volatile fr = 0;
	volatile fl = 0;
	volatile br = 0;
	volatile bl = 0;
	/* Main loop */
	while (1)
	{
		
		f = getFrontDistance();
		fr = getFrontRightDistance();
		fl = getFrontLeftDistance();
		br = getBackRightDistance();
		bl = getBackLeftDistance();
		
		
	}
}

