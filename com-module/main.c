/*
 * com-module.c
 *
 * Created: 2015-11-06 10:54:37
 * Author : Victor
 */ 

#define F_CPU 15000000UL	// CPU clock frequency is 8MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <i2c_slave.h>	// Com module is a Slave on the I2C-bus. 
#include <bluetooth.h>	// Add bluetooth compatibility.
#include <util/delay.h>


/* Simple program waiting for data on the i2c bus. */
int main(void)
{	
	/* Initialize bluetooth, enables global interrupts for handling interrupt when receiving data. */
	bt_init( F_CPU );
	/* Initialize as a slave on I2C-bus. */
	i2c_init_slave(); 
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	
	/* Main loop */
	while ( 1 ) 
    {
		/* Receive data from bus */
		//i2c_match_read_slave();		// Wait for connection from master to read data.
		//i2c_read_slave();			// Read incoming data.
		
		_delay_ms(500);
		bt_transmit( recv_data );	// Transmit received data on I2C-bus to the PC.
		
		/* Write data to bus */
		//write_data=recv_data;		// Echo the receive data.
		//i2c_match_write_slave();	// Request to connect to master to write data.
		//i2c_write_slave();			// Write data to master.
		
    }
}

