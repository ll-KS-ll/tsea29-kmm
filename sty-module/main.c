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
#include <stdbool.h>
#include "i2c_slave.h" // Is slave module
#include "autonomous.h"
#include "boot.h"
#include "sensorValues.h"
#include "variables.h"
#include "gyroController.h"

// temporary, only for testing
#include "motorKernel.h"




void follow_marking_tape(){
	volatile uint16_t value;
	uint16_t *sensorBar;
	int numerator;
	int denominator;
	volatile float fault=0;
	while(true){
		numerator = 0;
		denominator = 0;
		sensorBar = getSensorBar();
		for(int i=0; i<7; i++){	
			value = *(sensorBar+i);
			value++;
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
	}
	
	/*if (fault < 0) driveForward(30, 30 + (fault * 50));
	else driveForward(30 + (fault * 50), 30);*/
}

int main(void)
{
	/* Initialize com-module as a slave on I2C-bus with the address of com-module. */
	i2c_init_slave( STY_ADDRESS );

	/* Boot Claw-/Motor-kernel */
	boot();
	
	/* Enable the Global Interrupt Enable flag so that interrupts can be processed. */
	sei();
		
	/* Chilla liiide */
	_delay_ms(1000); 
	
	//driveForward(50,50);
	follow_marking_tape();
	/* Main loop */
	while (1)
	{
		//exploreLabyrinth();
	}
}

