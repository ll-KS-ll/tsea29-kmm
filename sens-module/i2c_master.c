/*
 * TSEA29.c
 * Program for Master Mode
 * Created: 2015-11-05 13:27:23
 * Author : Viktor & Victor
 * SDA = Serial Data
 * SCL = Serial CLock
 */ 
#define F_CPU 1000000UL

#include <avr/io.h>
#include <i2c_master.h>


void i2c_init_master( void )
{
	/* Set SDA, SCL as input and activate pull-up */
	DDRC = (0<<DDC0) | (0<<DDC1);
	PORTC = (1<<DDC0) | (1<<DDC1);
	
	/* Set register for clock generation */
	TWBR = 0x4E;						// Bit rate = 0x4E(=78) and Prescale = 64 => SCL = 100kHz for f_cpu 1MHz ...but it's really 100Hz 
	TWSR = (1<<TWPS1) | (1<<TWPS0);		// Setting prescalar bits (1,1) = 64
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
	
	/* Enable TWI */
	TWCR = (1<<TWEN);
}

/* Clear the i2c interrupt flag. */
void clear_twint( void )
{
	TWCR = (1<<TWEN) | (1<<TWINT);	// Keep i2c enabled. Clear interrupt flag. 
}

/* Send START condition. */
void start( void )
{
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT); // Keep i2c enabled. Activate START condition. Clear interrupt flag. 
	while( !(TWCR & (1<<TWINT)) );				// Wait till start condition is transmitted
	while( (TWSR & NO_RELEVANT_STATE_INFO) != START_COND_TRANSMITTED ); // Check for the acknowledgment
}

/* Send repeated START condition. */
void repeated_start( void )
{
	TWCR = TWCR | (1<<TWSTA);		// Activate START condition.
	clear_twint();					// Clear interrupt flag.
	while( !(TWCR & (1<<TWINT)) );  // Wait till restart condition is transmitted.
	while( (TWSR & NO_RELEVANT_STATE_INFO) != RSTART_COND_TRANSMITTED ); // Check for the acknowledgment
}

/* Send slave address to connect with. (SLA+R/W). */
void send_address( uint8_t address )
{
	TWDR = address;					// Load address and write instruction to send.
	clear_twint();					// Clear interrupt flag.
	while ( !(TWCR & (1<<TWINT)) );	// Wait till complete TWDR byte transmitted.
	while( (TWSR & NO_RELEVANT_STATE_INFO) != SLAW_ACK_RECEIVED );	// Check for the acknowledgment
}

/* Send a byte of data. */
void send_data( uint8_t data )
{
	TWDR = data;					// Put data in TWDR
	clear_twint();					// Clear interrupt flag.
	while ( !(TWCR & (1<<TWINT)) );	// Wait till complete TWDR byte transmitted.
	while( (TWSR & NO_RELEVANT_STATE_INFO) != DATA_ACK_RECEIVED ); // Check for the acknowledgment
}

/* Read a byte of data. */
void read_data( void )
{
	clear_twint();					// Clear interrupt flag.
	while ( !(TWCR & (1<<TWINT)) ); // Wait till complete TWDR byte transmitted.
	while( (TWSR & NO_RELEVANT_STATE_INFO) != DATA_NACK_RECEIVED ); // Check for the acknowledgment. 
	recv_data = TWDR;				// Load incoming data to recv_data.
}

/* Send STOP condition. */
void stop( void )
{
	TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT); // Keep i2c enabled. Activate STOP condition. Clear interrupt flag.
	while( !(TWCR & (1<<TWSTO)) );				// Wait till stop condition is transmitted.
}

void i2c_write_byte( uint8_t address, uint8_t byte )
{
	start();							// Send start condition.
	send_address ( address|I2C_WRITE );	// Write address and data direction bit(write) on SDA.
	send_data( byte );					// Write data to slave.
	stop();								// Send stop condition.
}

void i2c_read_byte( uint8_t address )
{
	start();							// Send start condition.
	send_address ( address|I2C_READ );	// Write address and data direction bit(write) on SDA.
	read_data();						// Read data from bus to recv_data. 
	stop();								// Send stop condition.
}

void i2c_write( uint8_t address, data_package data )
{
	start();									// Send start condition.
	send_address ( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA.
	send_data( data.id );						// Write data to slave.
	repeated_start();							// Send repeated start condition.
	send_data( (data.data<<8) );				// Write data to slave.
	repeated_start();							// Send repeated start condition.
	send_data( data.data );						// Write data to slave.
	stop();										// Send stop condition.	
}