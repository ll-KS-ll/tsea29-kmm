/*
 * com-module.c
 *
 * Created: 2015-11-06 10:54:37
 * Author : Victor
 */ 

#define F_CPU 15000000UL	// CPU clock frequency is 15MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c_slave.h"	// Com module is a Slave on the I2C-bus. 
#include "bluetooth.h"	// Add bluetooth compatibility.
#include "sensorValues.h" // Storage of sensor values received over I2C.
#include <util/delay.h>

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
		/*
		_delay_ms(10);

		if ((flag_array & (1<<0)) == (1<<0)) {
			bt_transmit(START);
			bt_transmit(getStart());
			flag_array &= ~(1<<0);
		}
		if ((flag_array & (1<<1)) == (1<<1)) {
			bt_transmit(AUTONOM);
			bt_transmit(getAutonom());
			flag_array &= ~(1<<1);
		}
		if ((flag_array & (1<<2)) == (1<<2)) {
			// bt_transmit(FRONT_LEFT);
			// bt_transmit(getFrontLeftDistance());
			flag_array &= ~(1<<2);
		}
		if ((flag_array & (1<<3)) == (1<<3)) {
			bt_transmit(BACK_LEFT);
			bt_transmit(getBackLeftDistance());
			flag_array &= ~(1<<3);
		}		
		if ((flag_array & (1<<4)) == (1<<4)) {
			//bt_transmit(FRONT);
			//bt_transmit(getFrontDistance());
			flag_array &= ~(1<<4);
		}
		if ((flag_array & (1<<5)) == (1<<5)) {
			// bt_transmit(BACK_RIGHT);
			// bt_transmit(getBackRightDistance());
			flag_array &= ~(1<<5);
		}
		if ((flag_array & (1<<6)) == (1<<6)) {
			// bt_transmit(FRONT_RIGHT);
			// bt_transmit(getFrontRightDistance());
			flag_array &= ~(1<<6);
		}	
		*/
		/*
		bt_transmit(LINESENSOR_0)
		bt_transmit(*(getSensorBar()));
		
		bt_transmit(LINESENSOR_1)
		bt_transmit(*(getSensorBar() + 1));
		
		bt_transmit(LINESENSOR_2)
		bt_transmit(*(getSensorBar() + 2));
		
		bt_transmit(LINESENSOR_3)
		bt_transmit(*(getSensorBar() + 3));
		
		bt_transmit(LINESENSOR_4)
		bt_transmit(*(getSensorBar() + 4));
		
		bt_transmit(LINESENSOR_5)
		bt_transmit(*(getSensorBar() + 5));
		
		bt_transmit(LINESENSOR_6)
		bt_transmit(*(getSensorBar() + 6));
		*/
    }
	
}

