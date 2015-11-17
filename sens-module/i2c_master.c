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
#include <util/delay.h>

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
	
	recv_data = 0;
	error_status = 0;
}

void error( uint8_t status )
{
	error_status = status;
	/* Reset I2C Unit. */
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTO);
	TWCR = 0; 
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
	if( (TWSR & NO_RELEVANT_STATE_INFO) != START_COND_TRANSMITTED )	// Check for correct status.
		error(ERROR_START);	// Notify an error occurred. 
}

/* Send repeated START condition. */
void repeated_start( void )
{
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT); // Keep i2c enabled. Activate START condition. Clear interrupt flag.
	while( !(TWCR & (1<<TWINT)) );  // Wait till restart condition is transmitted.
	if( (TWSR & NO_RELEVANT_STATE_INFO) != RSTART_COND_TRANSMITTED ) // Check for the acknowledgment
		error(ERROR_REPAETED_START);	// Notify an error occurred. 
}

/* Send slave address to connect with. (SLA+R/W). */
void send_address( uint8_t address )
{
	TWDR = address;					// Load address and write instruction to send.
	clear_twint();					// Clear interrupt flag.
	while ( !(TWCR & (1<<TWINT)) );	// Wait till complete TWDR byte transmitted.
	
	/* W returns ACK but R returns NACK, hence removal of check for ACK. */
	//while( (TWSR & NO_RELEVANT_STATE_INFO) != SLAW_ACK_RECEIVED );	// Check for the acknowledgment
}

/* Send a byte of data. */
void send_data( uint8_t data )
{
	TWDR = data;					// Put data in TWDR
	clear_twint();					// Clear interrupt flag.
	while ( !(TWCR & (1<<TWINT)) );	// Wait till complete TWDR byte transmitted.
	if ( (TWSR & NO_RELEVANT_STATE_INFO) != DATA_ACK_RECEIVED ) // Check for the acknowledgment
		error(ERROR_SEND_DATA);	// Notify an error occurred. 
}

/* Read a byte of data. */
void read_data( void )
{
	clear_twint();					// Clear interrupt flag.
	while ( !(TWCR & (1<<TWINT)) ); // Wait till complete TWDR byte transmitted.
	if ( (TWSR & NO_RELEVANT_STATE_INFO) != DATA_NACK_RECEIVED ) // Check for the acknowledgment. 
		error(ERROR_READ_DATA);	// Notify an error occurred. 
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

uint8_t i2c_write_package( uint8_t address, data_package package )
{
	/* Writes data package but without repeated start. */
	error_status = 0;	// Clear Error status.
	
	start();									// Send start condition.
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_address ( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA.
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_data( package.id );					// Write data to slave.
	if ( error_status ) return error_status;	// Check if an error occurred.
	/* ======================= */
	stop();
	_delay_ms(10);
	start();
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_address ( address|I2C_WRITE );
	if ( error_status ) return error_status;	// Check if an error occurred.
	//repeated_start();							// Send repeated start condition.
	/* ======================= */
	send_data( (package.data>>8) );				// Write data to slave.
	if ( error_status ) return error_status;	// Check if an error occurred.
	/* ======================= */
	stop();
	_delay_ms(10);
	start();
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_address ( address|I2C_WRITE );
	if ( error_status ) return error_status;	// Check if an error occurred.
	//repeated_start();							// Send repeated start condition.
	/* ======================= */
	send_data( package.data );					// Write data to slave.
	if ( error_status ) return error_status;	// Check if an error occurred.
	stop();										// Send stop condition.	
	return 0;
}