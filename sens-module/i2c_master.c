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
#include <util/delay.h>
#include <inttypes.h>
#include <i2c_master.h>


void i2c_init_master(void)
{
	/* Set SDA, SCL as input and activate pull-up */
	DDRC = (0<<DDC0)|(0<<DDC1);
	PORTC = (1<<DDC0)|(1<<DDC1);
	
	TWBR= 0x4E;						// Bit rate = 0x4E(=78) and Prescale = 64 => SCL = 100KHz
	TWSR=(1<<TWPS1)|(1<<TWPS0);		// Setting prescalar bits (1,1) = 64
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
	
	// TWCR=(1<<TWEN); // Enable TWI?	
}

void i2c_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
	while((TWSR & 0xF8)!= 0x08); // Check for the acknowledgment
}

void i2c_repeated_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
	while((TWSR & 0xF8)!= 0x10); // Check for the acknowledgment
}

void i2c_write_address(unsigned char data)
{
	TWDR=data;						// Address and write instruction
	TWCR=(1<<TWINT)|(1<<TWEN);		// Clear TWI interrupt flag, Enable TWI
	while (!(TWCR & (1<<TWINT)));	// Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8)!= 0x18);	// Check for the acknowledgment
}

void i2c_read_address(unsigned char data)
{
	TWDR=data;					  // Address and read instruction
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
	while((TWSR & 0xF8)!= 0x40);  // Check for the acknowledgment
}

void i2c_write_data(unsigned char data)
{
	TWDR=data;					  // Put data in TWDR
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x28); // Check for the acknowledgment
}

void i2c_read_data(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x58); // Check for the acknowledgment
	recv_data=TWDR;
}

void i2c_stop(void)
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}

