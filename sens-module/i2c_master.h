#define GENERAL_CALL_ADDRESS 0x00	// Used to transmitt data to all slaves.
#define SENS_ADDRESS 0x10			// Address of the master at sens-module.
#define COM_ADDRESS 0x20			// Establish connection with com-module.
#define STY_ADDRESS 0x30			// Establish connection with styr-module.
#define I2C_READ 1					// Read bit, signals read mode.
#define I2C_WRITE 0					// Write bit, signals write mode.
/* STATUS CODES */
#define NO_RELEVANT_STATE_INFO 0xF8		// No relevant state information available; TWINT = "0". Wait or proceed current transfer.
#define START_COND_TRANSMITTED 0x08		// A Start condition has been transmitted.
#define RSTART_COND_TRANSMITTED	0x10	// A Repeated Start condition has been transmitted. 
#define SLAW_ACK_RECEIVED 0x18			// SLA+W has been transmitted; ACK has been received.
#define SLAR_ACK_RECEIVED 0x40			// SLA+R has been transmitted; ACK has been received.
#define DATA_ACK_RECEIVED 0x28			// Data byte has been transmitted; ACK has been received. 
#define DATA_NACK_RECEIVED 0x58			// Data byte has been transmitted; NOT ACK has been received.

/* Structure of a data package. */
typedef struct {
	uint8_t id;
	uint16_t data;
} data_package;

/* Internal functions */
void i2c_init_master( void );				// Initialize processor as master on i2c_bus.
void i2c_start( void );						// Send a start condition.
void i2c_repeated_start( void );			// Send a repeated start condition.
void i2c_stop( void );						// Send stop condition.
void i2c_send_address( uint8_t address );	// Send address of slave.
void i2c_send_data( uint8_t data );			// Send data to slave.
void i2c_read_data( void );					// Read data from slave.
/* External functions */
void i2c_write_byte( uint8_t address, uint8_t byte );	// Write a byte to slave with specified address.
void i2c_read_byte( uint8_t address );					// Read a byte from slave with specified address. 
void i2c_write( uint8_t address, data_package data);	// Transmitt a data package to slave with specified address.

uint8_t recv_data;	// Received data from the bus is put here.


