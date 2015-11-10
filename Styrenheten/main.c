/*
* Styrenheten.c
*
* Created: 2015-11-05 10:11:22
* Author : Lukas
*/

#define F_CPU 15000000UL
#include <avr/io.h>
#include <util/delay.h>


void initMotorPWM() {
	/*
	* Port 6 = DirLeft, Port 5 = PWMLeft
	* Port 3 = DirRight, Port 4 = PWMRight
	*/
	
	/************************************************************************/
	/* Setting up Timer Control Register A/B for fast PWM			        */
	/************************************************************************/
	//TCCRnx, COMnx, WGn1:0
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1);
	TCCR1B |= (1<<WGM12)|(1<<WGM11)|(1<<CS11)|(1<<CS10);

	
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
	OCR1A = leftSpeed; 
	OCR1B = rightSpeed; 
}

void forward() {
	PORTD = (1<<3)|(0<<6);
}

void reverse() {
	PORTD = (0<<3)|(1<<6);
}

void rotateLeft() {
	PORTD = (1<<3)|(1<<6);
}

void rotateRight() {
	PORTD = (0<<3)|(0<<6);
}

void testMotor() {
	forward();
	setMotorSpeed(0,0);
	_delay_ms(2000);
	setMotorSpeed(150, 150);
	_delay_ms(3000);
	setMotorSpeed(0,0);
	_delay_ms(2000);
	reverse();
	setMotorSpeed(150, 150);
	_delay_ms(3000);
	setMotorSpeed(0,0);
	_delay_ms(2000);
	rotateLeft();
	setMotorSpeed(150, 150);
	_delay_ms(3000);
	setMotorSpeed(0,0);
	_delay_ms(2000);
	rotateRight();
	setMotorSpeed(150, 150);
	_delay_ms(3000);
}

//Inte klart än
void initClawPWM() {
	TCCR3A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR3B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
	
	ICR3=4999;  //fPWM=50Hz (Period = 20ms Standard).
	PORTB |= (1<<6);
}

void openClaw() {
	OCR3A = 280;	//45 < x < 90 degree
}

void closeClaw(){
	OCR3A = 97;		//0 degree
}

void testClaw(){
	closeClaw();
	_delay_ms(1000);
	openClaw();
	_delay_ms(1000);
}

int main(void)
{
	//unsigned int counter = 0;
	// unsigned int i;
	// Initializes the PWM signal output for the motors.
	initMotorPWM();
	//initClawPWM();
	DDRD |= 0x78; // 0111_1000;
	
	while (1)
	{
		//testMotor();
		//testClaw();
		
		forward();
		setMotorSpeed(128, 64);
	}
	
}

