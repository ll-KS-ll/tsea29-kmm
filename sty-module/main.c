/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Drive robot through labyrinth								*
 * Language: C															*
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
#include "variables.h"
#include "gyroController.h"
#include "clawKernel.h"

// temporary, only for testing
#include "motorKernel.h"
#include "clawKernel.h"


void follow_tape(uint16_t regulate_value){
	switch(regulate_value){
		case 1:
			stop();
			break;
		case 2:
			driveRotateLeft(25,25);
			break;
		case 3:
			driveRotateRight(25,25);
			break;
		case 4:
			driveForward(20,20);
	}
}

int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );

	/* Boot Claw-/Motor-/Gyro-kernel */
	boot();
	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	_delay_ms(1000);
	
	initMap();

	
	
	/* Main loop */
	while (1)
	{
		
			
		if(getAutonom()) {
			if(getStart()) {
				/* reset start so it only runs the labyrinth once */
				//updateRegisters(0, 1);
				//exploreLabyrinth();
				//driveForward(30,30);
				if(getRegulateValue()>0){
					//follow_tape(getRegulateValue());
					stop();
				}
			} 
		}
		else {
		/* Do shit that the PC says to do */
			if(getStart()) {
			}
		}
	}
}

