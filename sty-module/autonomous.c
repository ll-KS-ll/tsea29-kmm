/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include <stdbool.h>
#include "autonomous.h"
#include "motorKernel.h"
#include "sensorValues.h"

static bool running = false;

void exploreLabyrinth() {
	running = true;
	
	/*
		Write main loop for exploring labyrinth.
	*/
	while(running) {
		break; 	
	}
	
}

void rotateOnSpot(int startAngle, int endAngle) {
	// fill
}