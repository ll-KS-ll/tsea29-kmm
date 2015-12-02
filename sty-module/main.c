/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Drive robot through labyrinth								*
 * Language: C															*
 *																		*
/************************************************************************/

#define F_CPU 15000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "i2c_slave.h" // Is slave module
#include "autonomous.h"
#include "boot.h"
#include "sensorValues.h"

// temporary, only for testing
#include "motorKernel.h"

/* Test method used to now crash into wall */

//void dontCrash()
//{
	//unsigned int frontDis = 0;
	//frontDis = getFrontDistance();
	//
	//if(frontDis >= MIN_DISTANCE_TO_FRONT_WALL && !stopped) {
		//stopped = true;
		//stop();
	//} else if (frontDis <= 600 && frontDis >= 200 && stopped){
		//driveReverse(DEFAULT_SPEED, DEFAULT_SPEED);
	//} else if (frontDis <= 500) {
		//driveForward(DEFAULT_SPEED, DEFAULT_SPEED);
		//stopped = false;
	//}
	//
//}

void follow_marking_tape(uint16_t *sensorBar){
	int numerator=0;
	int denominator=0;
	float fault=0;
	sensorBar = getSensorBar();
	for(int i=0; i<7; i++){	
		numerator+=(*(sensorBar+i)*(i+1));
		denominator+=*(sensorBar+i);
	}
	fault = 4-(numerator/denominator);
	if(fault<-0.1){
		driveRotateLeft(50,50);
	}
	else if(fault>0.1){
		driveRotateRight(50,50);
	}
	else if(fault<=0.1 && fault >=-0.1){
		driveForward(20,20);
	}
	
	/*if (fault < 0) driveForward(30, 30 + (fault * 50));
	else driveForward(30 + (fault * 50), 30);*/
}

int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	/* Boot Claw-/Motor-kernel */
	boot();
	
	_delay_ms(4000);
	uint16_t *sensorBar; 
	char i=0;
	//exploreLabyrinth();
	//driveForward(50,50);
	//int test = 0;
	/* Main loop */
	while (1)
	{
		follow_marking_tape(sensorBar);
		_delay_ms(50);
		//test = getBackLeftDistance();	
		//dontCrash();
		i++;
	}
}

