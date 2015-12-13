#include <avr/io.h>
#include <avr/interrupt.h>
#include "bluetooth.h"
#include "i2c_slave.h"

void bt_init( unsigned long f_cpu )
{
	/* Set baud rate */
	uint16_t baudrate = (((f_cpu / (BAUD * 16UL))) - 1);	// Calculate baud rate to put in UBRR. 
	UBRR0H = (uint8_t)(baudrate>>8);
	UBRR0L = (uint8_t)baudrate;
	/* Enable receiver, transmitter and receiver interrupt. */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

void bt_transmit( uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !(UCSR0A & (1<<UDRE0)) )
		;

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

ISR(USART0_RX_vect)
{
	write_data = UDR0; //steeringCmd;
}
