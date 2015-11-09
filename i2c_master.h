void i2c_start(void);
void i2c_repeated_start(void);
void i2c_init_master(void);
void i2c_write_address(unsigned char);
void i2c_read_address(unsigned char);
void i2c_write_data(unsigned char);
void i2c_read_data(void);
void i2c_stop(void);

unsigned char address=0x20, read=1, write=0;
unsigned char write_data=0x01, recv_data;