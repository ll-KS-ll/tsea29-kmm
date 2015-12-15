/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Handle communication between modules.						*
 * Language: C															*
 *																		*
/************************************************************************/

#define F_CPU 15000000UL	// CPU clock frequency is 15MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c_slave.h"	// Com module is a Slave on the I2C-bus. 
#include "bluetooth.h"	// Add bluetooth compatibility.
#include "sensorValues.h" // Storage of sensor values received over I2C.
#include "sound.h"

/* Simple program waiting for data on the i2c bus. */
int main(void)
{	
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( COM_ADDRESS );
	/* Initialize bluetooth, enables global interrupts for handling interrupt when receiving data. */
	bt_init( F_CPU );	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	/* Main loop */
	while ( 1 ) 
    {		
		if(getSteerCmd() == 9) {
			playSong();
		}
		if (write_data == 10){
			playSong();
		}
    }
	
}

