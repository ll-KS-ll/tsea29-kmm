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

void dontCrash()
{
	unsigned int frontDis = 0;
	static bool stopped = false;
	frontDis = getFrontDistance();
	
	if(frontDis >= 500 && !stopped) {
		stopped = true;
		stop();
	} else if (frontDis <= 600 && frontDis >= 200 && stopped){
		driveReverse(DEFAULT_SPEED - 100, DEFAULT_SPEED- 100);
	} else if (frontDis <= 500) {
		driveForward(DEFAULT_SPEED- 100, DEFAULT_SPEED- 100);
		stopped = false;
	}
	
}

int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	_delay_ms(1000);	// Chilla lite.
	
	//exploreLabyrinth();
	/* Main loop */
	
	uint16_t angle = 0;
	while (1)
	{
		//angle = getCurrentAngle();
		//if (angle < 200)
		//{
		//	driveRotateLeft(DEFAULT_SPEED, DEFAULT_SPEED);
		//}
		//else stop();
		//dontCrash();
	}
}

