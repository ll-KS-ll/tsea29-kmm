/*
 * com-module.c
 *
 * Created: 2015-11-06 10:54:37
 * Author : Victor
 */ 

#define F_CPU 15000000UL	// CPU clock frequency is 15MHz

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
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( COM_ADDRESS ); 
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	write_data = 0xBEEF;	// Set data to write. In future this will be a command from PC.
	
	/* Main loop */
	while ( 1 ) 
    {
		_delay_ms(100);
		
		data_package tmp = *datap; // Store entire package in case a new is read.
		bt_transmit( '<' );
		bt_transmit( tmp.id );
		bt_transmit( ',' );
		bt_transmit( (tmp.data >> 8) );
		bt_transmit( tmp.data );
		bt_transmit( '>' );
		bt_transmit( ' ' );
    }
}

