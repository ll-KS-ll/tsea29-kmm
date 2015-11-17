/*
 * sens-module.c
 *
 * Created: 2015-11-09 17:13:54
 * Author : Victor
 */ 
#define F_CPU 1000000UL // Set clock frequency to 1 MHz

#include <avr/io.h>
#include <util/delay.h>
#include <i2c_master.h>	// Sensor module is a i2c master.

int main(void)
{
	/* Initialize sensor module as I2C master */
	i2c_init_master();
	
	_delay_ms(5000); // Chilla lite!
	
	/* Create a data package to send. */
	data_package datap = {0x56, 0xAAAA}; // Creates a data package with "01010110" as id and "10101010 10101010" as data. 
	
	/* Main loop */
    while (1) 
    {
		i2c_write_package(COM_ADDRESS, datap);	// Write an entire package to com-module.
		_delay_ms(2000);	// Chilla lite va.
    }
}

