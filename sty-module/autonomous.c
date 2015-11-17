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

direction currentDirection = north;

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


/* Private function used for rotating 90 degrees left */
static void rotateLeftOnce() {
	uint16_t currentAngle = getCurrentAngle();
	uint16_t endAngle = currentAngle + 256;
	setRotateLeft();
	setMotorSpeed(250, 250);
	while (1) {
		currentAngle = getCurrentAngle();
		if(currentAngle >= endAngle) {
			stop();
			break;
		}
		_delay_ms(10);
	}
}

/* Private function used for rotating 90 degrees right */
static void rotateRightOnce() {
	uint16_t currentAngle = getCurrentAngle();
	uint16_t endAngle = currentAngle - 256;
	setRotateRight();
	setMotorSpeed(250, 250);
	while (1) {
		currentAngle = getCurrentAngle();
		if(currentAngle >= endAngle) {
			stop();
			break;
		}
		_delay_ms(10);
	}
}

/* Private function used by algorithms to rotate robot on the spot to the desired direction */
void rotateOnSpot(direction targetDirection) {
	if(currentDirection == targetDirection) {
		return;
	}
	switch(currentDirection) {
		case north:
			if (targetDirection == west) {
				rotateLeftOnce();
			} else if (targetDirection == south) {
				rotateLeftOnce();
				rotateLeftOnce();
			} else {
				rotateRightOnce();
			}
			break;
		case west:
			if (targetDirection == north) {
				rotateRightOnce();
			} else if (targetDirection == south) {
				rotateLeftOnce();
			} else {
				rotateLeftOnce();
				rotateLeftOnce();
			}
			break;
		case south:
			if (targetDirection == west) {
				rotateRightOnce();
			} else if (targetDirection == east) {
				rotateLeftOnce();
			} else {
				rotateLeftOnce();
				rotateLeftOnce();
			}
			break;
		case east:
			if(targetDirection == south) {
				rotateRightOnce();
			} else if (targetDirection == north) {
				rotateLeftOnce();
			} else {
				rotateLeftOnce();
				rotateLeftOnce();
			}
			break;
	}
	// Update current direction
	currentDirection = targetDirection;
}

void rotateOnSpot(int startAngle, int endAngle) {
	// fill
}

	//Move the equivalent of one node
void moveOneNode(){
	
}
	//Adjust position to move straight in the labyrinth
void autoAdjustPosition(){
	
}

