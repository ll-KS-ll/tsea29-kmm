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
	/* Initialize sensor module as I2C master */
	i2c_init_master();
	
	data_package data = {0, "ab"};
	
	/* Main loop */
    while (1) 
    {
		
		i2c_write(COM_ADDRESS, data);
		/*
		i2c_write_byte(COM_ADDRESS, 'k'); // Write data to bus.
		_delay_ms(10);

		i2c_read_byte(COM_ADDRESS); // Read data from bus.
		_delay_ms(10);
		
		i2c_write_byte(COM_ADDRESS, recv_data);
		_delay_ms(1000);
		*/
    }
}

