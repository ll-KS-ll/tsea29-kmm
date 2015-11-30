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
	int fault=0;
	sensorBar = getSensorBar();
	for(int i=0; i<7; i++){
		if(i==1){
			numerator+=120;
			denominator+=30;
		}
		else{	
		numerator+=(*(sensorBar+i)*(i+1));
		denominator+=*(sensorBar+i);
		}
	}
	fault = 4.5-(numerator/denominator);
	if(fault<0){
		driveRotateLeft(20,40);
	}
	else if(fault>0){
		driveRotateRight(40,20);
	}
	else if(fault==0){
		driveForward(30,30);
	}
}

int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	/* Boot Claw-/Motor-kernel */
	boot();
	
	_delay_ms(2000);
	uint16_t *sensorBar; 
	char i=0;
	//exploreLabyrinth();
	driveForward(50,50);
	//int test = 0;
	/* Main loop */
	while (1)
	{
		if(i==8){i=0;}
		sensorBar = getSensorBar();
		if(*(sensorBar+i) > 400) {
			stop();
			driveForward(50,50);
			stop();
			while(1){
				follow_marking_tape(sensorBar);
			}
		}
		//test = getBackLeftDistance();	
		//dontCrash();*/
		i++;
	}
}

