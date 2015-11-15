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
	// this should not be set here, it should be set when button is pushed on sens-module to start auto-driving
	running = true;
	
	/*
		Write main loop for exploring labyrinth.
	*/
	while(running) {
		/*	Autonomous logic.
			1. Set current square to explored, and set which directions are walls and not walls.
			2. Find next square to explore. 
				a. If square in front of robot is unexplored set it as target, break;
				b. If square to the right of robot is unexplored set as target, break;
				c. If square to the left of robot is unexplored set as target, break;
				
		*/
		
		break;
	}
	
}

void rotateOnSpot(int startAngle, int endAngle) {
	// fill
}