/*
*
*/
#define COM_ADDRESS 0x20


/* Structure of a data package. */
typedef struct {
	uint8_t id;
	uint16_t data;
} data_package;


void i2c_init_slave( void );				// Init the I2C slave.
void i2c_match_read_slave( void );			// Connect as reader.
void i2c_read_slave( void );				// Read data from bus. (Polling)
void i2c_match_write_slave( void );			// Connect as writer.
void i2c_write_slave( void );				// Write data to bus. 

uint8_t write_data;	// Data write buffer. Put data to write on the bus.
uint8_t recv_data;	// Data receive buffer. Read data from the bus is stored here. 