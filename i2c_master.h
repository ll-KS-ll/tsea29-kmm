void TWI_start(void);
void TWI_repeated_start(void);
void TWI_init_master(void);
void TWI_write_address(unsigned char);
void TWI_read_address(unsigned char);
void TWI_write_data(unsigned char);
void TWI_read_data(void);
void TWI_stop(void);

unsigned char address=0x20, read=1, write=0;
unsigned char write_data=0x01, recv_data;