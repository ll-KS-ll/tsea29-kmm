/*
*
*/

void i2c_init_slave( void );	// Init the I2C slave.
void i2c_match_read_slave( void );			// Connect as reader.
void i2c_read_slave( void );				// Read data from bus. (Polling)
void i2c_match_write_slave( void );			// Connect as writer.
void i2c_write_slave( void );				// Write data to bus. 

unsigned char write_data;	// Data write buffer. Put data to write on the bus.
unsigned char recv_data;	// Data receive buffer. Read data from the bus is stored here. 