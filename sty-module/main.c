/*
* Styrenheten.c
*
* Created: 2015-11-05 10:11:22
* Author : Lukas
*/

#define F_CPU 15000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void initMotorPWM() {
	/*
	* Port 6 = DirLeft, Port 5 = PWMLeft
	* Port 3 = DirRight, Port 4 = PWMRight
	*/
	
	/************************************************************************/
	/* Setting up Timer Control Register A/B for fast PWM			        */
	/************************************************************************/
	//TCCRnx, COMnx, WGn1:0
	TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11)|(1<<WGM10);
	TCCR1B = (1<<WGM12)|(1<<CS11);

	
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
	cli();
	OCR1A = leftSpeed; 
	OCR1B = rightSpeed;
	sei(); 
}

void setForward() {
	PORTD = (1<<3)|(0<<6);
}

void setReverse() {
	PORTD = (0<<3)|(1<<6);
}

void setRotateLeft() {
	PORTD = (1<<3)|(1<<6);
}

void setRotateRight() {
	PORTD = (0<<3)|(0<<6);
}

void driveForward(int leftSpeed, int rightSpeed) {
	setForward();
	setMotorSpeed(leftSpeed,rightSpeed);
}

void rotateLeft() {
	setRotateLeft();
	setMotorSpeed(300, 300);
}

void rotateRight() {
	setRotateRight();
	setMotorSpeed(300, 300);
}

void ninetyDegreesTime() {
	_delay_ms(1420);
}

void oneSquare() {
	_delay_ms(1500);
}

void stop() {
	setMotorSpeed(0, 0);
}

//Inte klart än
void initClawPWM() {
	
	//set clock
	TCCR3A |= (1<<COM3A1)|(1<<COM3B1)|(1<<WGM30);
	TCCR3B |= (1<<WGM32)|(1<<CS31)|(1<<CS30);
	
	//set output pin
	DDRB |= 0x40; // 0100 0000
}

void openClaw() {
	OCR3A = 150;
}

void closeClaw(){
	OCR3A = 113;
}

void testClaw(){
	openClaw();
}


void thereAndBackAgain() {
	driveForward(250, 250);
	oneSquare();
	oneSquare();
	stop();
	rotateLeft();
	ninetyDegreesTime();
	ninetyDegreesTime();
	stop();
	driveForward(250, 250);
	oneSquare();
	oneSquare();
	stop();
	rotateRight();
	ninetyDegreesTime();
	ninetyDegreesTime();
	stop();
}
int main(void)
{
	//unsigned int counter = 0;
	// unsigned int i;
	// Initializes the PWM signal output for the motors.
	initMotorPWM();
	initClawPWM();
	
	DDRD |= 0x78; // 0111_1000;

	//while(1) {
		//thereAndBackAgain();
	//}
	rotateLeft();
	ninetyDegreesTime();
	ninetyDegreesTime();
	stop();
	
}

