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
	TWBR = 65;							// Bit rate: 100 kHz for F_SCL=14.745 MhZ
	TWSR = (0<<TWPS1) | (0<<TWPS0);		// Setting pre-scalar bits 00 = 4^0 = 0
	// SCL freq= F_CPU/(16 + 2*(TWBR)*4^TWPS)
	
	/* Enable TWI and Interrupts. */
	TWCR = (1<<TWEN) | (1<<TWIE);
}

/* Clear the i2c interrupt flag. */
void clear_twint( void )
{
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWINT);	// Keep i2c enabled. Clear interrupt flag. 
}

/*
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
*/


uint8_t adr;				// Address of slave to write/read to/from.
data_package dp;			// Data package to write.
bool bus_busy;				// Boolean for when bus is busy doing a transaction.
uint8_t transaction_state;	// State variables, keeping track of where in the transaction we are.

/* Send STOP condition. */
void stop( void ) {
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWSTO) | (1<<TWINT);	// Keep i2c enabled, keep interrupt enabled, send stop, clear interrupt flag;
	bus_busy = false;
}

void i2c_write(uint8_t address, data_package datap)
{
	// TODO: Add a timeout to transactions, in case the bus is frozen.
	if ( bus_busy ) // Don't start new write while still writing.
		return;
	bus_busy = true;
	transaction_state = 0;
	adr = address + I2C_WRITE;
	dp = datap;
	/* Send START */
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWSTA) | (1<<TWINT); // Keep i2c enabled. Activate START condition. Clear interrupt flag.	
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
			TWDR = dp.id;
			transaction_state++;	
			clear_twint();					// Clear interrupt flag.
			break;
			
		case SLAW_NACK_TRANSMITTED:			// SLA+W has been transmitted; NACK received.
			/* Slave didn't respond correctly. Drop package and stop the transaction. */
			stop();
			break;	
			
		case DATA_ACK_TRANSMITTED:			// Data byte has been transmitted; ACK received.
			if ( transaction_state == 1 ) {
				TWDR = dp.data>>8;		
				transaction_state++;
				clear_twint();					// Clear interrupt flag.
			} else if ( transaction_state == 2 ) {
				TWDR = dp.data;
				transaction_state++;
				clear_twint();					// Clear interrupt flag.
			} else {
				stop();
			}
			break;
			
		case DATA_NACK_TRANSMITTED:			// Data byte has been transmitted; NACK received.
			/* Slave didn't respond correctly. Abort. */
			stop();
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
			//bus_busy = false;
			break;
		
		case DATA_ACK_RECEIVED:				// Data byte has been received, ACK returned.
			// Datan �r h�r! 
			//read_data();
			// TODO: Return ACK or NACK, that is the question.
			TWCR = (1<<TWINT | 1<<TWEA);	// Data byte has been received, ACK returned.		
			break;
			
		case DATA_NACK_RECEIVED:				// Data byte has been received; NACK received.
			//read_data();
			TWCR = (1<<TWSTO | 1<<TWINT);		// Data byte has been received. Slave fucked up.
			//bus_busy = false;
			break;		
		/* ================== */
	}
}
			