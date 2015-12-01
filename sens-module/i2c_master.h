/*
* I2C Master. Interrupt based implementation of an i2c master.
*
*/

/* ====== ADDRESSES ====== */
#define GENERAL_CALL_ADDRESS 0x00	// Used to transmit data to all slaves.
#define SENS_ADDRESS 0x10			// Address of the master at sens-module.
#define COM_ADDRESS 0x20			// Establish connection with com-module.
#define STY_ADDRESS 0x30			// Establish connection with styr-module.
#define I2C_READ 1					// Read bit, signals read mode.
#define I2C_WRITE 0					// Write bit, signals write mode.
/* ======================= */

/* ====== STATUS CODES ====== */
#define NO_RELEVANT_STATE_INFO 0xF8		// No relevant state information available; TWINT = "0". Wait or proceed current transfer.
#define START_COND_TRANSMITTED 0x08		// A Start condition has been transmitted.
#define RSTART_COND_TRANSMITTED	0x10	// A Repeated Start condition has been transmitted. 

#define SLAW_ACK_TRANSMITTED 0x18		// SLA+W has been transmitted; ACK has been received.
#define SLAW_NACK_TRANSMITTED 0x20		// SLA+W has been transmitted; NACK has been received.
#define DATA_ACK_TRANSMITTED 0x28		// Data byte has been transmitted; ACK has been received.
#define DATA_NACK_TRANSMITTED 0x30		// Data byte has been transmitted; NACK has been received.

#define SLAR_ACK_TRANSMITTED 0x40		// SLA+R has been transmitted; ACK has been received.
#define SLAR_NACK_RECEIVED 0x48			// SLA+R has been transmitted; NACK has been received.
#define DATA_ACK_RECEIVED 0x50			// Data byte has been received, ACK has been received.
#define DATA_NACK_RECEIVED 0x58			// Data byte has been received; NACK has been received.
/* =========================== */


/* Structure of a data package. */
typedef struct {
	uint8_t id;
	uint16_t data;
} data_package;

data_package recv_datap;	// Received data packages from slaves can be put here.


void i2c_init_master( void );							// Initialize processor as master on i2c_bus.
void i2c_write(uint8_t address, data_package datap);	// Write a data package to slave with specified address. If bus is busy, package is dropped.
void i2c_read(uint8_t address, uint8_t id);				// Read data from data package with specified id from a slave with specified address. Read data can be access from recv_datap. If bus is busy, package is dropped. 
