/*
 * com-module.c
 *
 * Created: 2015-11-06 10:54:37
 * Author : Victor
 */ 

#define F_CPU 8000000UL	// CPU clock frequency is 8MHz

#include <avr/io.h>
#include <i2c_slave.h>	// Com module is a Slave on the I2C-bus. 
#include <bluetooth.h>	// Add bluetooth compatibility.
#include <util/delay.h>



/* Simple program polling for data from bluetooth and sending it back. */
int main(void)
{	
	bt_init( F_CPU );
	uint8_t data;
	
	while ( 1 ) 
    {
		data = bt_receive();
		bt_transmit( data );
		_delay_ms( 10 );
    }
}

