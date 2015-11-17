/************************************************************************
 *																		*
 * Author: G�ntech							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include <stdbool.h>
#include "autonomous.h"
#include "motorKernel.h"
#include "sensorValues.h"

static bool exploring = false;

void exploreLabyrinth() {
	exploring = true;
	
	/*
		Write main loop for exploring labyrinth.
	*/
	while(exploring) {
		break; 	
	}
	
}

void rotateOnSpot(int startAngle, int endAngle) {
	// fill
}