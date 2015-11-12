/*
* I2C Slave for 
*/
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

uint8_t write_data;	// Data write buffer. Put data to write on the bus.
uint8_t recv_data;	// Data receive buffer. Read data from the bus is stored here.


void i2c_init_slave( void );			// Initialize the I2C slave.
void i2c_match_read_slave( void );		// Connect as reader.
void i2c_read_slave( void );			// Read data from bus. (Polling)
void i2c_match_write_slave( void );		// Connect as writer.
void i2c_write_slave( void );			// Write data to bus. 