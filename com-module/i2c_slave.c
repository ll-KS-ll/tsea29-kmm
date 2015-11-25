/*
 * slave.c
 * Program for Slave Mode
 * Created: 2015-11-05 14:47:54
 *  Author: Viktor & Victor
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <i2c_slave.h>
#include <stdbool.h>

/* Variables to track progress of package */
bool id, datah;
/* Variables for simple two state buffer for reading. */
uint8_t buffer;
data_package* datap_buffer_ptr;
data_package datap_buffer1, datap_buffer2;	// Data package buffer.


void i2c_init_slave( uint8_t address )
{
	/* Set the address of the slave. */
	TWAR = address;
	
	/* Set ACK, enable I2C pins and enable interrupt. */
	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
	
	/* Set I2C pins as input and activate pull-up. */
	DDRC = (0<<DDC0) | (0<<DDC1);
	PORTC = (1<<DDC0) | (1<<DDC1);
	
	id = true;
	datah = true;
	recv_data = '0';
	//datap_buffer1 = {0, 0};
	datap = &datap_buffer1;
	buffer = 0;
	
}

/* Clear the i2c interrupt flag. */
void clear_twint( void )
{
	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE) | (1<<TWINT);	// Activate ACK as response. Keep i2c enabled. Keep interrupts enabled. Clear interrupt flag.
}

/* Unknown state. */
void i2c_write_slave(void)
{
	TWDR= write_data;				// Fill TWDR register with the data to be sent.
	TWCR= (1<<TWEN)|(1<<TWINT);		// Enable TWI, Clear TWI interrupt flag.
	while((TWSR & NO_RELEVANT_STATE_INFO) != SLAR_DATA_TRANSMITTED);	// Wait for the acknowledgment.
}

/* Unknown state. */
void i2c_match_write_slave(void)
{
	/* Match the slave address and slave direction bit(write)  */
	while((TWSR & NO_RELEVANT_STATE_INFO)!= SLAR_REQUEST_RECEIVED) // Loop till correct acknowledgment have been received
		clear_twint();
}

/* Unknown state. */
void i2c_read_slave(void)
{
	/* Clear TWI interrupt flag, Set acknowledgment, Enable TWI. */ 
	TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));	// Wait for TWINT flag
	while((TWSR & NO_RELEVANT_STATE_INFO)!= SLAW_DATA_RECEIVED );		// Wait for acknowledgment
	recv_data=TWDR;					// Get value from TWDR
}

/* Unknown state. */
void i2c_match_read_slave(void) 
{
	/* Match the slave address and slave direction bit(read) */
	while((TWSR & NO_RELEVANT_STATE_INFO)!= SLAW_REQUEST_RECEIVED)  // Loop till correct acknowledgment have been received
		clear_twint();
}


/* Interrupt handler for I2C interrupts. */
ISR(TWI_vect){
	uint8_t status = (TWSR & NO_RELEVANT_STATE_INFO);	// Get status code of incoming I2C interrupt.
	switch ( status ) {
		
		case SLAW_REQUEST_RECEIVED:	// Request from master to write.
			/* Incoming data */
			clear_twint();	// ACK sent, clear interrupt flag.
			break;
			
		case SLAW_DATA_RECEIVED: // Data from master is received.
			/* Read data */
			recv_data=TWDR;	// Load incoming data into recv_data.
			
			/* Select buffer */
			if (buffer ) {
				datap_buffer_ptr = &datap_buffer1;
				buffer = 1;
			} else {
				datap_buffer_ptr = &datap_buffer2;
				buffer = 0;
			}
			
			/* Read data package */
			if ( id ) {
				datap_buffer_ptr->id = recv_data;
				id = false;
			} else if ( datah ) {
				datap_buffer_ptr->data = (recv_data<<8);
				datah = false;	
			} else {
				datap_buffer_ptr->data = datap_buffer_ptr->data | recv_data;
				id = datah = true;
				datap = datap_buffer_ptr; // Set data package.
			}
			clear_twint();	// ACK sent, clear interrupt flag.
			break;
			
		case SLAR_REQUEST_RECEIVED:	// Request from master to read.
			/* Load data to write */
			TWDR = write_data;							// Load outgoing data with write_data.
			TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWINT);	// Keep i2c enabled. Keep interrupts enabled. Clear interrupt flag. NACK.
			break;	
			
		case SLAR_DATA_TRANSMITTED:	// Data to master has been transmitted.
			/* Data written */ 
			clear_twint();	// NACK sent, clear interrupt flag.
			break;
		
		case STOP_COND_RECEIVED: // Stop or Repeated Start condition has come.
			/* Stop */
			clear_twint();	// ACK sent, clear interrupt flag.
			break;
		
		/*
		default:
			clear_twint(); // Just respond like everything is okey even though it's probably not! #YOLO	
		*/
	}
}
