/*
 * slave.c
 * Program for Slave Mode
 * Created: 2015-11-05 14:47:54
 *  Author: Viktor & Victor
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <i2c_slave.h>


void i2c_init_slave(void)
{
	/* Set the address of the slave. */
	TWAR = COM_ADDRESS;
	
	/* Set ACK, enable I2C pins and enable interrupt. */
	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
	
	/* Set I2C pins as input and activate pull-up. */
	DDRC = (0<<DDC0) | (0<<DDC1);
	PORTC = (1<<DDC0) | (1<<DDC1);
	
	recv_data = '0';
}

/* Set i2c hardware to answer with an ACK when conditions comes from master. */
void set_ack( void )
{
	TWCR = TWCR | (1<<TWEA);
}

/* Set i2c hardware to answer with an NACK when conditions comes from master. */
void set_nack( void )
{
	TWCR = TWCR & ~(0x00 | (1<<TWEA));
}

/* Clear the i2c interrupt flag. */
void clear_twint( void )
{
	/* ================= */
	//TWCR = TWCR | (1<<TWINT); // Clear interrupt flag.
	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE) | (1<<TWINT);
	/* ================= */
}

void i2c_write_slave(void)
{
	TWDR= write_data;				// Fill TWDR register with the data to be sent.
	TWCR= (1<<TWEN)|(1<<TWINT);		// Enable TWI, Clear TWI interrupt flag.
	while((TWSR & NO_RELEVANT_STATE_INFO) != SLAR_DATA_TRANSMITTED);	// Wait for the acknowledgment.
}

void i2c_match_write_slave(void)
{
	/* Match the slave address and slave direction bit(write)  */
	while((TWSR & NO_RELEVANT_STATE_INFO)!= SLAR_REQUEST_RECEIVED) // Loop till correct acknowledgment have been received
		clear_twint();
}

void i2c_read_slave(void)
{
	/* Clear TWI interrupt flag, Set acknowledgment, Enable TWI. */ 
	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));	// Wait for TWINT flag
	while((TWSR & NO_RELEVANT_STATE_INFO)!= SLAW_DATA_RECEIVED );		// Wait for acknowledgment
	recv_data=TWDR;					// Get value from TWDR
}

void i2c_match_read_slave(void) 
{
	/* Match the slave address and slave direction bit(read) */
	while((TWSR & NO_RELEVANT_STATE_INFO)!= SLAW_REQUEST_RECEIVED)  // Loop till correct acknowledgment have been received
		clear_twint();
}

/*
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
*/

/* Interrupt handler for I2C interrupts. */
ISR(TWI_vect){
	uint8_t status = (TWSR & NO_RELEVANT_STATE_INFO);	// Get status code of incoming I2C interrupt.
	switch ( status ) {
		
		case SLAW_REQUEST_RECEIVED:	// Request from master to write.
			/* Incoming data */
			clear_twint();	// ACK sent, clear interrupt flag.
			break;
			
		case SLAW_DATA_RECEIVED:	// Data from master is received.
			/* Read data */
			recv_data=TWDR;	// Load incoming data into recv_data.
			clear_twint();	// ACK sent, clear interrupt flag.
			break;
			
		case SLAR_REQUEST_RECEIVED:	// Request from master to read.
			/* Load data to write */
			set_nack();		// Enable NACK to generate correct status for master.
			TWDR = 'd';		// Load outgoing data with 'd'.
			clear_twint();	// ACK sent, clear interrupt flag.
			break;	
			
		case SLAR_DATA_TRANSMITTED:	// Data to master has been transmitted.
			/* Data written */ 
			set_ack();		// Enable ACK again.
			clear_twint();	// NACK sent, clear interrupt flag.
			break;
		
		case STOP_COND_RECEIVED: // Stop condition has come.
			/* Stop */
			clear_twint();	// ACK sent, clear interrupt flag.
			break;
		
		/*
		default:
			clear_twint(); // Just respond like everything is okey even though it's probably not! #YOLO	
		*/
	}
}
