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


/* ====== ADDRESSES ====== */
#define SENS_ADDRESS 0x10	// Address of sensor module. 
#define COM_ADDRESS 0x20	// Establish connection with com-module.
#define STY_ADDRESS 0x30	// Establish connection with styr-module.
/* ======================= */

/* ====== STATUS CODES ====== */
#define NO_RELEVANT_STATE_INFO 0xF8	// No relevant state information available; TWINT = "0". Wait or proceed current transfer.
#define STOP_COND_RECEIVED 0xA0		// A STOP condition or repeated START condition has been received while still addressed as slave.

#define SLAW_REQUEST_RECEIVED 0x60	// Own SLA+W has been received; ACK has been returned.
#define DATA_ACK_RECEIVED 0x80		// Previously addressed with own SLA+W; data has been received; ACK has been returned
#define DATA_NACK_RECEIVED 0x88		// Previously addressed with own SLA+W; data has been received; NACK has been returned

#define GENERAL_CALL_RECEIVED 0x70	// General call address has been received; ACK has been returned.
#define GENERAL_CALL_DATA_ACK 0x90	// Previously addressed with general call; data has been received; ACK has been returned.
#define GENERAL_CALL_DATA_NACK 0x98	// Previously addressed with general call; data has been received; NACK has been returned.

#define SLAR_REQUEST_RECEIVED 0xA8	// Own SLA+R has been received; ACK has been returned.
#define DATA_ACK_TRANSMITTED 0xB8	// Data byte in TWDR has been transmitted; ACK has been received.
#define DATA_NACK_TRANSMITTED 0xC0	// Data byte in TWDR has been transmitted; NACK has been received.
#define LAST_DATA_ACK_TRANSMITTED 0xC8	// Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received
/* ========================== */


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
/*
void i2c_match_read_slave( void );		// Connect as reader. (Polling)
void i2c_read_slave( void );			// Read byte of data from bus. (Polling)
void i2c_match_write_slave( void );		// Connect as writer. (Polling)
void i2c_write_slave( void );			// Write byte data to bus. (Polling)
void i2c_read_package( void );			// Read an entire data package. (Polling)
*/