/*
 * Styrenheten.c
 *
 * Created: 2015-11-05 10:11:22
 * Author : Lukas
 */ 

#include <avr/io.h>

void initLeftMotorPWM() {
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0)|(1<<WGM02)|(1<<WGM01)|(1<<WGM00);
	OCR1A = 0x80; // set 50% duty cycle
	DDRD |= (1<<DDB5); // set PD5 as output
}

int main(void)
{
	//initLeftMotorPWM();
    /* Replace with your application code */
	
	DDRD = DDRD | 0b01010000; // Set ports 4 & 6 to output without changing the rest.
	
    while (1) 
    {
		PORTD = (0<<4)|(1<<6);
    }
}

