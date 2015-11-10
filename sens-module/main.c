/*
 * sens-module.c
 *
 * Created: 2015-11-09 17:13:54
 * Author : Victor
 */ 
#define F_CPU 1000000UL // Set clock frequency to 1 MHz

#include <avr/io.h>
#include <util/delay.h>
#include <i2c_master.h>

int main(void)
{
    i2c_init_master();
	
    while (1) 
    {
		/* Write data to bus */
		write_data = 'a';
		i2c_start();						// Send start condition
		i2c_write_address(COM_ADDRESS+I2C_WRITE);	// Write address and data direction bit(write) on SDA
		i2c_write_data(write_data);			// Write data in slave
		i2c_stop();							// Send stop condition
		
		/* Read data from bus */
		/*
		i2c_start();
		i2c_read_address(address+read); // Function to write address and data direction bit(read) on SDA
		i2c_read_data(); // Function to read data from slave
		i2c_stop();
		*/
		
		_delay_ms(1000);
    }
}

