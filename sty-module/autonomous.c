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

static bool exploring = false;

/* INFO
	360 degrees == 1024 in angle-value
	90 degrees == 256 in angle-value
	Turning left == angle-value increase
	Turning right == angle-value decreases

*/

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