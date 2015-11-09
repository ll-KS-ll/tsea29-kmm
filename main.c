/*
 * com-module.c
 *
 * Created: 2015-11-06 10:54:37
 * Author : Victor
 */ 

#define F_CPU 15000000UL	// CPU clock frequency is 8MHz

#include <avr/io.h>
#include <i2c_slave.h>	// Com module is a Slave on the I2C-bus. 
#include <bluetooth.h>	// Add bluetooth compatibility.
#include <util/delay.h>



/* Simple program polling for data from bluetooth and sending it back. */
int main(void)
{	
	bt_init( F_CPU );
	i2c_init_slave(); 
	
	uint8_t data = 'a'; // Data received on bt. 
	_delay_ms( 1000 );
	bt_transmit( data );
	
	while ( 1 ) 
    {
		/* Receive data from bus */
		i2c_match_read_slave();		// Function to match the slave address and slave direction bit(read)
		i2c_read_slave();			// Function to read data
		
		bt_transmit( recv_data );	// Transmit received data on I2C-bus to the PC.
		
		/* Write data to bus */
		write_data=recv_data;		// Toggle the receive data
		i2c_match_write_slave();	//Function to match the slave address and slave direction bit(write)
		i2c_write_slave();			// Function to write data
		
		/* Bluetooth echo */
		//data = bt_receive();
		//bt_transmit( data );
		_delay_ms( 10 );
    }
}

