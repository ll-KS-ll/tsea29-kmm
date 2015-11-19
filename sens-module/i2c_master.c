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
#include <avr/interrupt.h>
#include <stdbool.h>

void i2c_init_master( void )
{
	/* Set SDA, SCL as input and activate pull-up */
	DDRC = (0<<DDC0) | (0<<DDC1);
	PORTC = (1<<DDC0) | (1<<DDC1);
	
	/* Set register for clock generation */
	TWBR = 0;							// Bit rate: 62.5 kHz for F_SCL=1 MhZ
	TWSR = (0<<TWPS1) | (0<<TWPS0);		// Setting pre-scalar bits 00 = 4^0 = 0
	// SCL freq= F_CPU/(16 + 2*(TWBR)*4^TWPS)
	// 100 kHz = 16 MHz / (16 + 2 * 72 * 4^0)	TWBR=72, TWSR=0. 
	
	/* Enable TWI and Interrupts. */
	TWCR = (1<<TWEN) | (1<<TWIE);
	
	bus_busy = false;
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
		error(ERROR_REPEATED_START);	// Notify an error occurred. 
}

/* Send slave address to connect with. (SLA+R/W). */
void send_address( uint8_t address )
{
	TWDR = address;					// Load address and write instruction to send.
	clear_twint();					// Clear interrupt flag.
	while ( !(TWCR & (1<<TWINT)) );	// Wait till complete TWDR byte transmitted.
	
	if (address & 1) {	// Check for the acknowledgment
		/* Read returns NACK. */
		if ((TWSR & NO_RELEVANT_STATE_INFO) != SLAR_NACK_RECEIVED)
			error(ERROR_ADDRESS_READ);
	} else {
		/* Write returns ACK */
		if ((TWSR & NO_RELEVANT_STATE_INFO) != SLAW_ACK_TRANSMITTED)
			error(ERROR_ADDRESS_WRITE);	
	}
}

/* Send a byte of data. */
void send_data( uint8_t data )
{
	TWDR = data;					// Put data in TWDR
	clear_twint();					// Clear interrupt flag.
	while ( !(TWCR & (1<<TWINT)) );	// Wait till complete TWDR byte transmitted.
	if ( (TWSR & NO_RELEVANT_STATE_INFO) != DATA_NACK_RECEIVED ) // Check for the acknowledgment
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
	error_status = 0;	// Clear Error status.
	
	start();									// Send start condition.
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_address ( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA.
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_data( package.id );					// Write data to slave.
	if ( error_status ) return error_status;	// Check if an error occurred.
	
	repeated_start();							// Send repeated start condition.
	if ( error_status ) return error_status;	// Check if an error occurred.	
	send_address ( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA.
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_data( (package.data>>8) );				// Write data to slave.
	if ( error_status ) return error_status;	// Check if an error occurred.
	
	repeated_start();							// Send repeated start condition.
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_address ( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA.
	if ( error_status ) return error_status;	// Check if an error occurred.
	send_data( package.data );					// Write data to slave.
	if ( error_status ) return error_status;	// Check if an error occurred.
	
	stop();										// Send stop condition.	
	return 0;
}

uint16_t i2c_read_data(uint8_t id, uint8_t address)
{	
	recv_data = TWDR;
	
	start();									// Send start condition.
	send_address( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA.
	send_data(id);								// Send id of the kind of data you want from the listening slave.
	
	repeated_start();							// Send repeated start condition.
	send_address( address| I2C_READ );			// Write address and data direction bit(read) on SDA.
	read_data();								// Read the data the slave now sends you.
	uint8_t hdata = recv_data;					// Save the received data.
	 
	repeated_start();						    // Send repeated start condition.
	send_address( address| I2C_READ );			// Write address and data direction bit(read) on SDA.
	read_data();								// Read the data the slave now sends you.
	uint8_t ldata = recv_data; 					// Save the received data.
	
	stop();										// Send stop condition.
	
	uint16_t data = (hdata<<8) + ldata;			// Save the data all together.
	
	return data;								// Return the data of the id-type you requested.
}


uint8_t adr, data_package dp;
bool bus_busy;

void i2c_write(uint8_t address, data_package datap)
{
	if ( bus_busy ) // Don't start new write while still writing.
		return;
	bus_busy = true;	
	adr = address + I2C_WRITE;
	dp = datap;
	/* Send START */
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT); // Keep i2c enabled. Activate START condition. Clear interrupt flag.	
}


/* Interrupt handler for I2C interrupts. */
ISR(TWI_vect){
	uint8_t status = (TWSR & NO_RELEVANT_STATE_INFO);	// Get status code of incoming I2C interrupt.
	switch ( status ) {

		case START_COND_TRANSMITTED:		// START condition has been transmitted.
			TWDR = adr;						// Load address and write instruction to send.
			clear_twint();					// Clear interrupt flag.
			break;
		
		case RSTART_COND_TRANSMITTED:		// Repeated START condition has been transmitted.
			TWDR = adr;						// Load address and write instruction to send.
			clear_twint();					// Clear interrupt flag.
			break;

		/* ====== WRITE ====== */
		case SLAW_ACK_TRANSMITTED:			// SLA+W has been transmitted; ACK received.
			TWDR = data;					// Put data in TWDR.
			clear_twint();					// Clear interrupt flag.
			break;
			
		case SLAW_NACK_TRANSMITTED:			// SLA+W has been transmitted; NACK received.
			TWDR = data;					// Put data in TWDR.
			clear_twint();					// Clear interrupt flag.
			break;	
			
		case DATA_ACK_TRANSMITTED:			// Data byte has been transmitted; ACK received.
			// TODO: Check OP data transfer
			TWDR = data;					// Put data in TWDR.
			clear_twint();					// Clear interrupt flag.
			break;
			
		case DATA_NACK_TRANSMITTED:			// Data byte has been transmitted; NACK received.
			// TODO: Check OP data transfer
			TWDR = data;					// Put data in TWDR.
			clear_twint();					// Clear interrupt flag.	
			break;
		/* ==================== */
		
		/* ====== READ ====== */
		case SLAR_ACK_TRANSMITTED:			// SLA+R has been transmitted; ACK received.
			// Nu kommer det data! :D
			// TODO: Return ACK or NACK, that is the question.
			clear_twint();					// Data byte will be received and ACK will be returned.
			break;
		
		case SLAR_NACK_RECEIVED:			// SLA+R has been transmitted; NACK received.
			// Mer data tack! ^^
			TWCR = (1<<TWSTO | 1<<TWINT);	// Stop will be transmitted. Fuck you slave, you fucked up.
			bus_busy = false;
			break;
		
		case DATA_ACK_RECEIVED:				// Data byte has been received, ACK returned.
			// Datan är här! 
			read_data();
			// TODO: Return ACK or NACK, that is the question.
			TWCR = (1<<TWINT | 1<<TWEA);	// Data byte has been received, ACK returned.		
			break;
			
		case DATA_NACK_RECEIVED:				// Data byte has been received; NACK received.
			read_data();
			TWCR = (1<<TWSTO | 1<<TWINT);		// Data byte has been received. Slave fucked up.
			bus_busy = false;
			break;		
		/* ================== */
	}
}
			