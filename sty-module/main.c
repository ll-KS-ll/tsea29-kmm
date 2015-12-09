/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Drive robot through labyrinth								*
 * Language: C															*
 *																		*
/************************************************************************/

#define F_CPU 14575000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "i2c_slave.h" // Is slave module
#include "autonomous.h"
#include "boot.h"
#include "sensorValues.h"
#include "variables.h"
#include "gyroController.h"
#include "clawKernel.h"

// temporary, only for testing
#include "motorKernel.h"
#include "clawKernel.h"




void follow_marking_tape(){
	stop();
	uint16_t sensor_value;
	uint16_t *sensorBar;
	int numerator;
	int denominator;
	bool arrived = false;
	float fault=0;
	while(!arrived){
		arrived = true;
		numerator = 0;
		denominator = 0;
		sensorBar = getSensorBar();
		for(int i=0; i<7; i++){	
			sensor_value = *(sensorBar+i);
			if(sensor_value < 100 && arrived) arrived = false;
			numerator+=sensor_value*(i+1);
			denominator+=sensor_value;
		}
		fault = (numerator/denominator);
		if(fault<4){
			driveRotateLeft(25,25);
		}
		else if(fault>4.2){
			driveRotateRight(25,25);
		}
		else{
			driveForward(15,15);
			}
		}
	stop();
}

bool is_tape(){
	uint16_t *sensorBar = getSensorBar();
	uint16_t sensor_value;
	for(int i=0; i<7; i++){
		sensor_value = *(sensorBar + i);
		if (sensor_value > 200){
			return true;
		}
	}
	return false;
}

int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );

	/* Boot Claw-/Motor-/Gyro-kernel */
	boot();
	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
	
	_delay_ms(1000);
	
	initMap();
	
	//openClaw();
	//_delay_ms(500);
	////lowerClaw();
	////_delay_ms(500);
	////closeClaw();
	////_delay_ms(500);
	////raiseClaw();
	////_delay_ms(500);
	//
	////while(1) {
		////regulateRobot();
	////}
	//
	//volatile bool test;
	//while(1) {
		//test = getStart();
	//}
	//
	/* Main loop */
	while (1)
	{
		if(getAutonom()) {
			if(getStart()) {
				/* reset start so it only runs the labyrinth once */
				updateRegisters(0, 1);
				exploreLabyrinth();
				
			}
		} else if(!getAutonom()) {
			/* Do shit that the PC says to do */
			if(getStart()) {
				/* reset start so it only runs the labyrinth once */
				updateRegisters(0, 1);
				turnRight();
				
			}
		}
	}
}

