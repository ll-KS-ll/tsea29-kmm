/*
 * slave.c
 * Program for Slave Mode
 * Created: 2015-11-05 14:47:54
 *  Author: Viktor & Victor
 */ 
#include <avr/io.h>
#include <i2c_slave.h>


/* Initialize slave */
void i2c_init_slave(void)
{
	TWAR=COM_ADDRESS; // Set the address of the slave.
}


void i2c_send_ack ( void )
{
	// Get acknowledgment, Enable TWI, Clear TWI interrupt flag
	TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
}

/* Write data to bus */
void i2c_write_slave(void)
{
	TWDR= write_data;				// Fill TWDR register with the data to be sent.
	TWCR= (1<<TWEN)|(1<<TWINT);		// Enable TWI, Clear TWI interrupt flag.
	while((TWSR & 0xF8) != 0xC0);	// Wait for the acknowledgment.
}

/* Wait for write connection */
void i2c_match_write_slave(void)
{
	/* Match the slave address and slave direction bit(write)  */
	while((TWSR & 0xF8)!= 0xA8) // Loop till correct acknowledgment have been received
		i2c_send_ack();
}

/* Read data from bus */
void i2c_read_slave(void)
{
	/* Clear TWI interrupt flag,Get acknowledgment, Enable TWI. */ 
	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));	// Wait for TWINT flag
	while((TWSR & 0xF8)!=0x80);		// Wait for acknowledgment
	recv_data=TWDR;					// Get value from TWDR
}

/* Wait for read connection */
void i2c_match_read_slave(void) 
{
	/* Match the slave address and slave direction bit(read) */
	while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknowledgment have been received
		i2c_send_ack();
}

void i2c_read ( void )
{
	data_package package;
	
	i2c_match_read_slave();
	i2c_read_slave();
	package.id = recv_data;
	
	i2c_send_ack();
	i2c_read_slave();
	uint8_t hdata = recv_data;
	
	i2c_send_ack();
	i2c_read_slave();
	uint8_t ldata = recv_data;
	
	package.data = (hdata<<8)|ldata;
}