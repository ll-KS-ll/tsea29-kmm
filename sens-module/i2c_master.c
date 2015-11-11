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
	DDRC = (0<<DDC0)|(0<<DDC1);
	PORTC = (1<<DDC0)|(1<<DDC1);
	
	/* Set register for clock generation */
	TWBR= 0x4E;						// Bit rate = 0x4E(=78) and Prescale = 64 => SCL = 100kHz for f_cpu 1MHz ...but it's really 100Hz 
	TWSR=(1<<TWPS1)|(1<<TWPS0);		// Setting prescalar bits (1,1) = 64
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
	
	// TWCR=(1<<TWEN); // Enable TWI?	
}

void i2c_start( void )
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
	while((TWSR & NO_RELEVANT_STATE_INFO)!= START_COND_TRANSMITTED); // Check for the acknowledgment
	//if ((TWSR & NO_RELEVANT_STATE_INFO) != START_COND_TRANSMITTED)
		
}

void i2c_repeated_start( void )
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
	while((TWSR & NO_RELEVANT_STATE_INFO)!= RSTART_COND_TRANSMITTED); // Check for the acknowledgment
}

void i2c_send_address( uint8_t address )
{
	TWDR = address;						// Address and write instruction
	TWCR = (1<<TWINT)|(1<<TWEN);		// Clear TWI interrupt flag, Enable TWI
	while (!(TWCR & (1<<TWINT)));		// Wait till complete TWDR byte transmitted
	while( (TWSR & NO_RELEVANT_STATE_INFO) != SLAW_ACK_RECEIVED) ;	// Check for the acknowledgment
}

void i2c_send_data( uint8_t data )
{
	TWDR = data;						// Put data in TWDR
	TWCR = (1<<TWINT)|(1<<TWEN);		// Clear TWI interrupt flag,Enable TWI
	while ( !(TWCR & (1<<TWINT)) );		// Wait till complete TWDR byte transmitted
	while( (TWSR & NO_RELEVANT_STATE_INFO) != DATA_ACK_RECEIVED ); // Check for the acknowledgment
}

void i2c_read_data( void )
{
	TWCR = (1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while ( !(TWCR & (1<<TWINT)) ); // Wait till complete TWDR byte transmitted
	while( (TWSR & NO_RELEVANT_STATE_INFO) != DATA_NACK_RECEIVED ); // Check for the acknowledgment. 
	recv_data=TWDR;
}

void i2c_stop( void )
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while( !(TWCR & (1<<TWSTO)) );  // Wait till stop condition is transmitted
}

void i2c_write_byte( uint8_t address, uint8_t byte )
{
	i2c_start();									// Send start condition
	i2c_send_address ( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA
	i2c_send_data( byte );							// Write data to slave
	i2c_stop();										// Send stop condition
}

void i2c_read_byte( uint8_t address )
{
	i2c_start();									// Send start condition
	i2c_send_address ( address|I2C_READ );			// Write address and data direction bit(write) on SDA
	i2c_read_data();
	i2c_stop();
}

void i2c_write_bytes( uint8_t address )
{
	i2c_start();									// Send start condition
	i2c_send_address ( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA
	i2c_read_data();
	i2c_stop();
}

void i2c_write( uint8_t address, data_package data )
{
	i2c_start();									// Send start condition
	i2c_send_address ( address|I2C_WRITE );			// Write address and data direction bit(write) on SDA
	i2c_send_data( data.id );						// Write data to slave
	i2c_repeated_start();							// Send repeated start condition
	i2c_send_data( (data.data<<8) );				// Write data to slave
	i2c_repeated_start();							// Send repeated start condition
	i2c_send_data( data.data );						// Write data to slave
	i2c_stop();										// Send stop condition	
}