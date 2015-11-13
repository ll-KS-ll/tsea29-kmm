/*
 * sty-module.c
 *
 * Created: 2015-11-13 11:19:38
 * Author : Victor
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <i2c_slave.h>	// Sty module is a Slave on the I2C-bus. 
#include <util/delay.h>

int main(void)
{
    /* Initialize com-module as a slave on I2C-bus with the address of com-module. */
    i2c_init_slave( STY_ADDRESS );
    /* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
    sei();
	
	/* Main loop */
    while (1) 
    {
		/* The data package datap is automatically read when sens-module sends new data. */
		
		/* Example of how to get stuff from data packages. */
		uint8_t pid = datap.id;
		uint16_t pdata = datap.data;
		
		_delay_ms(2000);	// Chilla lite
    }
}

