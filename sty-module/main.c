/************************************************************************
 *																		*
 * Author: G�ntech							                            *
 * Purpose: Drive robot through labyrinth								*
 * Language: C															*
 *																		*
/************************************************************************/

#define F_CPU 15000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdbool.h>
#include "i2c_slave.h" // Is slave module
#include "autonomous.h"
#include "boot.h"
#include "sensorValues.h"
#include "variables.h"
#include "gyroController.h"

// temporary, only for testing
#include "motorKernel.h"


int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );

	/* Boot Claw-/Motor-kernel */
	boot();
	
	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	/* Chilla liiide */
	_delay_ms(1000);
	
	
	exploreLabyrinth();
	
	
}

