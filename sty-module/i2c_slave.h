/*
* I2C Slave. Interrupt driven implementation of a i2c slave. Data packages are automatically read to "datap".  
*
* Usage:
*	Call initialize-function with address of this slave.
*	Enable interrupts. include <avr/interrupt.h> and call sei().
*	When data is incoming it will be written to "datap".
*	To get id of data package use datap.id
*	To get data of data package use datap.data, obs uint16_t.
* 
*/


#define SENS_ADDRESS 0x10	// Address of sensor module. 
#define COM_ADDRESS 0x20	// Establish connection with com-module.
#define STY_ADDRESS 0x30	// Establish connection with styr-module.
/* STATUS CODES */
#define NO_RELEVANT_STATE_INFO 0xF8	// No relevant state information available; TWINT = "0". Wait or proceed current transfer.
#define SLAW_REQUEST_RECEIVED 0x60	// Write request to this slave is received. Answer with ACK/NACK.
#define SLAW_DATA_RECEIVED 0x80		// Write data to this slave is received. Answer with ACK/NACK.
#define SLAR_REQUEST_RECEIVED 0xA8	// Read request to this slave is received. Answer with ACK/NACK.
#define SLAR_DATA_TRANSMITTED 0xC0	// Data in TWDR has been transmitted.
#define STOP_COND_RECEIVED 0xA0		// A STOP condition or repeated START condition has been received.


/* Structure of a data package. */
typedef struct {
	uint8_t id;
	uint16_t data;
} data_package;

/* Buffers */
data_package* datap;	// Read data packages from the bus are stored here.
uint8_t write_data;		// Data write buffer. Put data to write on the bus.
uint8_t recv_data;		// Data receive buffer. Read partial data from the bus is stored here.

void i2c_init_slave( uint8_t address );	// Initialize the I2C slave.
void i2c_match_read_slave( void );		// Connect as reader. (Polling)
void i2c_read_slave( void );			// Read byte of data from bus. (Polling)
void i2c_match_write_slave( void );		// Connect as writer. (Polling)
void i2c_write_slave( void );			// Write byte data to bus. (Polling)
void i2c_read_package( void );			// Read an entire data package. (Polling)