#define COM_ADDRESS 0x20
#define I2C_READ 1
#define I2C_WRITE 0

void i2c_start(void);
void i2c_repeated_start(void);
void i2c_init_master(void);
void i2c_write_address(unsigned char);
void i2c_read_address(unsigned char);
void i2c_write_data(unsigned char);
void i2c_read_data(void);
void i2c_stop(void);

unsigned char write_data, recv_data;