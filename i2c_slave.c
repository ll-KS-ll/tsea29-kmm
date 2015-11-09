/*
 * slave.c
 * Program for Slave Mode
 * Created: 2015-11-05 14:47:54
 *  Author: Viktor & Victor
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <i2c_slave.h>


int i2c_slave_main(void)
{
	DDRB=0xff; // Set all B ports to output.
	i2c_init_slave(); // Function to initialize slave
	while(1)
	{
		i2c_match_read_slave(); //Function to match the slave address and slave direction bit(read)
		i2c_read_slave(); // Function to read data
		
		write_data=~recv_data; // Toggle the receive data
		
		i2c_match_write_slave(); //Function to match the slave address and slave direction bit(write)
		i2c_write_slave();       // Function to write data
	}
}

void i2c_init_slave(void) // Function to initialize slave
{
	TWAR=0x20; // Fill slave address to TWAR
}

void i2c_write_slave(void) // Function to write data
{
	TWDR= write_data;           // Fill TWDR register with the data to be sent
	TWCR= (1<<TWEN)|(1<<TWINT);   // Enable TWI, Clear TWI interrupt flag
	while((TWSR & 0xF8) != 0xC0); // Wait for the acknowledgement
}

void i2c_match_write_slave(void) //Function to match the slave address and slave direction bit(write)
{
	while((TWSR & 0xF8)!= 0xA8) // Loop till correct acknowledgement have been received
	{
		// Get acknowledgement, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
	}
}

void i2c_read_slave(void)
{
	// Clear TWI interrupt flag,Get acknowledgement, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
	while((TWSR & 0xF8)!=0x80); // Wait for acknowledgement
	recv_data=TWDR; // Get value from TWDR
	PORTB=recv_data; // send the received value on PORTB
}

void i2c_match_read_slave(void) //Function to match the slave address and slave direction bit(read)
{
	while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknowledgement have been received
	{
		// Get acknowledgement, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
	}
}