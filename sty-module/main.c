/************************************************************************
 *																		*
 * Author: Güntech - Lukas Lindqvist 		                            *
 * Purpose: Drive robot through labyrinth								*
 * Language: C															*
 * Version: 1.0															*
 *																		*
/************************************************************************/

#define F_CPU 14575000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "i2c_slave.h" // Is slave module
#include "autonomous.h"
#include "boot.h"
#include "sensorValues.h"
#include "remoteControl.h"


int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );

	/* Boot Claw-/Motor-/Gyro-kernel */
	boot();
	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	_delay_ms(1000);
	
	/* Main loop */
	while (1)
	{
		if(getAutonom()) {
			if(getStart()) {
				/* reset start so it only runs the labyrinth once */
				updateRegisters(0, 1);
				initMap();
				solveLabyrinth();
				
			}
		} else if(!getAutonom()) {
			/* Do shit that the PC says to do */
			executeCurrentCommand();
		}
	}
}

