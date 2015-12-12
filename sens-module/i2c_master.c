/*
 * TSEA29.c
 * Program for Master Mode
 * Created: 2015-11-05 13:27:23
 * Author : Viktor & Victor
 * SDA = Serial Data
 * SCL = Serial CLock
 */ 
#define F_CPU 14745000UL

#include <avr/io.h>
#include <i2c_master.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

bool rw_mode;				// Boolean to keep track of what mode the master is currently operating in.
uint8_t adr;				// Address of slave to write/read to/from.
data_package dp;			// Data package to write.
bool bus_busy;				// Boolean for when bus is busy doing a transaction.
uint8_t transaction_state;	// State variables, keeping track of where in the transaction we are.
uint16_t recv_data;			// Received data.

void i2c_init_master( void )
{
	/* Set SDA, SCL as input and activate pull-up */
	DDRC = (0<<DDC0) | (0<<DDC1);
	PORTC = (1<<DDC0) | (1<<DDC1);
	
	/* Set register for clock generation */
	TWBR = 65;							// Bit rate: 100 kHz for F_SCL=14.745 MhZ
	//TWBR = 16;
	TWSR = (0<<TWPS1) | (0<<TWPS0);		// Setting prescalar bits 00 = 4^0 = 0
	// SCL freq= F_CPU/(16 + 2*(TWBR)*4^TWPS)
	
	/* Enable TWI and Interrupts. */
	TWCR = (1<<TWEN) | (1<<TWIE);
}

/* Clear the i2c interrupt flag. */
void clear_twint( void )
{
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWINT);	// Keep i2c enabled. Clear interrupt flag. 
}

/* Send START condition. */
void start ( void )
{
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWSTA) | (1<<TWINT); // Keep i2c enabled. Activate START condition. Clear interrupt flag.
}

/* Send STOP condition. */
void stop( void ) 
{
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWSTO) | (1<<TWINT);	// Keep i2c enabled, keep interrupt enabled, send stop, clear interrupt flag;
	bus_busy = false;
}

/* Write an entire package. Used in write mode. */
void write_package( void ) 
{
	if ( transaction_state == 1 ) {
		TWDR = dp.data>>8;		// Send hdata.
		transaction_state++;
		clear_twint();			// Clear interrupt flag.
	} else if ( transaction_state == 2 ) {
		TWDR = dp.data;			// Send ldata.
		transaction_state++;
		clear_twint();			// Clear interrupt flag.
	} else {
		stop();	// Data package sent, end transmission.
	}
}

/* Write only id and change to SLA+R. Used in read mode. */
void switch_read_mode( void )
{
	adr = adr + I2C_READ;	// Change to SLA+R.
	start();				// Send RESTART.
}

/* Write data package datap to the bus. */
void i2c_write(uint8_t address, data_package datap)
{
	// TODO: Add a timeout to transactions, in case the bus is frozen.
	if ( bus_busy ) // Don't start a new write if already writing.
		return;
	bus_busy = true;
	rw_mode = I2C_WRITE;
	transaction_state = 0;
	adr = address + I2C_WRITE;
	dp = datap;
	start();	// Send START condition to begin transaction.
}

/* Read desired data package of chosen id and save it in dp. */
void i2c_read(uint8_t address, uint8_t id)
{
	if ( bus_busy ) // Don't start a new write if already writing.
		return;
	//while (bus_busy);
	bus_busy = true;
	dataRead = 0;
	rw_mode = I2C_READ;
	transaction_state = 0;
	adr = address + I2C_WRITE;
	dp.id = id;
	start();	// Send START condition to begin transaction.
}

void i2c_wait_for_data()
{
	while(dataRead == 0);
	dataRead = 0;	
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
			TWDR = dp.id;					// Write id to slave.
			transaction_state++;	
			clear_twint();					// Clear interrupt flag.
			break;
			
		case SLAW_NACK_TRANSMITTED:			// SLA+W has been transmitted; NACK received.
			stop();	// Slave didn't respond correctly. Drop the package and stop the transaction.
			break;	
			
		case DATA_ACK_TRANSMITTED:			// Data byte has been transmitted; ACK received.
			if(rw_mode == I2C_WRITE)
				write_package();	// Write mode. Write data.
			else
				switch_read_mode();	// Id written, change to read mode to receive data.
			break;
			
		case DATA_NACK_TRANSMITTED:			// Data byte has been transmitted; NACK received.
			stop(); // Slave didn't respond correctly. Abort. 
			break;
		/* ==================== */
		
		/* ====== READ ====== */
		case SLAR_ACK_TRANSMITTED:	// SLA+R has been transmitted; ACK received.
			TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWINT) | (1<<TWEA);			// Data byte will be received and ACK will be returned.
			break;
		
		case SLAR_NACK_RECEIVED:	// SLA+R has been transmitted; NACK received.
			stop();		// Something went wrong, slave didn't responed. Abort.
			break;
		
		case DATA_ACK_RECEIVED:	// Data byte has been received, ACK returned. 
			recv_data = TWDR;	// Hdata received. 
			clear_twint();	// Data byte has been received, NACK returned.
			break;
			
		case DATA_NACK_RECEIVED:	// Data byte has been received; NACK received.
			dp.data = (recv_data<<8) + TWDR;	// Ldata received.
			recv_datap = dp;	// Complete package read.
			dataRead = 1;		// All data have been read.
			stop();				// End transmission.
			break;		
		/* ================== */
	}
}
			