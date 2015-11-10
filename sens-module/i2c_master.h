#define GENERAL_CALL_ADDRESS 0x00	// Used to transmitt data to all slaves.
#define SENS_ADDRESS 0x10			// Address of the master at sens-module.
#define COM_ADDRESS 0x20			// Establish connection with com-module.
#define STY_ADDRESS 0x30			// Establish connection with styr-module.
#define I2C_READ 1					// Read bit, signals read mode.
#define I2C_WRITE 0					// Write bit, signals write mode.

void i2c_start(void);
void i2c_repeated_start(void);
void i2c_init_master(void);
void i2c_write_address(unsigned char);
void i2c_read_address(unsigned char);
void i2c_write_data(unsigned char);
void i2c_read_data(void);
void i2c_stop(void);

unsigned char write_data, recv_data;