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
#include <i2c_slave.h> // Is slave module
//#include "autonomous.h"
//#include "boot.h"
//#include "sensorValues.h"

// temporary, only for testing
//#include "motorKernel.h"

int main(void)
{
	
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	/* Boot Claw-/Motor-kernel */
	//boot();
	
	_delay_ms(2000);
	
	volatile angle = 0;
	
	//setRotateLeft();
	//setMotorSpeed(400, 400);	
	/* Main loop */
	// Testa att rotera
	while (1)
	{
		/* The data package datap is automatically read when sens-module sends new data. */
		
		/* Example of how to get stuff from data packages. */
		data_package tmp = *datap;
		volatile pid = tmp.id;
		volatile pdata = tmp.data;
		
		angle = pdata;
		
		/*
		if(angle >= 0.5) {
			stop();
			break;
		}
		*/
		
		_delay_ms(100);	// Chilla lite
	}
}

