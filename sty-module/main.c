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

int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	/* Boot Claw-/Motor-kernel */
	boot();
	
	_delay_ms(2000);
	
	exploreLabyrinth();
	
	//int test = 0;
	/* Main loop */
	while (1)
	{
		
		//test = getBackLeftDistance();	
		//dontCrash();
	}
}

