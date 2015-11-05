/*
 * Styrenheten.c
 *
 * Created: 2015-11-05 10:11:22
 * Author : Lukas
 */ 

#include <avr/io.h>

void initLeftMotorPWM() {
	DDRD |= (1<<5); // set PD5 as output

	TCCR1A |= (1<<COM1A1)|(0<<COM1A0)|(1<<WGM02)|(1<<WGM01)|(1<<WGM00);

	OCR1A = 0x80; // set 50% duty cycle
}

int main(void)
{
	//initLeftMotorPWM();
    /* Replace with your application code */
	
	DDRD |= 0b01111000; // Set ports 3 to 7 to output without changing the rest.
	
    while (1) 
    {
		PORTD = (1<<5)|(0<<6)|(1<<3)|(1<<4); // Control left and right motor
    }
}

