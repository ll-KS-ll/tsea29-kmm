/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#define F_CPU 15000000UL
#include <stdbool.h>
#include <util/delay.h>
#include "motorKernel.h"
#include "autonomous.h"
#include "sensorValues.h"
#include "variables.h"
#include "gyroController.h"

direction currentDirection = north;

/* INFO
	360 degrees == 1024 in angle-value
	90 degrees == 256 in angle-value
	Turning left == angle-value increase
	Turning right == angle-value decreases

*/

/* Using PD-regulator to make robot drive in middle of corridor */
int pdReg(){
	int u = 0;
	int e = 0;
	int t = 0;		
	
	// get the distances
	int fl = getFrontLeftDistance();
	int fr = getFrontRightDistance();
	int bl = getBackLeftDistance();
	int br = getBackRightDistance();
	
	// e = how close to robot is to the walls.
	e = ((fl + bl) - (fr + br)) / 2;

	// t = How wrongly the robot is rotated
	t = ((fl - bl) + (br - fr)) / 2;

	/* KP and KD konstants say how much the robot will react 
		being wrongly turned and positioned between the walls. */	
	u = KP * e + KD * t; 
	
	return u;
}

int leftReg(){
	int u = 0;
	int t = 0;		
	
	// get the distances
	int fl = getFrontLeftDistance();
	int bl = getBackLeftDistance();
	
	if(bl < 15 && fl < 15) {
		return -30;
	}
	// t = How wrongly the robot is rotated
	t = (fl - bl) ;

	/* KP and KD konstants say how much the robot will react 
		being wrongly turned and positioned between the walls. */	
	u = 5 * t; 
	
	return u;
}

int rightReg(){
	int u = 0;
	int t = 0;		
	
	// get the distances
	int fr = getFrontRightDistance();
	int br = getBackRightDistance();

	if(br < 15 && fr < 15) {
		return 30;
	}
	// t = How wrongly the robot is rotated
	t = (br - fr);

	/* KP and KD konstants say how much the robot will react 
		being wrongly turned and positioned between the walls. */	
	u = 5 * t; 
	
	return u;
}


void leftRegulator() {
	int regulate = leftReg();
	adjust(regulate);
}

void rightRegulator() {
	int regulate = rightReg();
	adjust(regulate);
}

void pdRegulator() {
	int regulate = pdReg();
	adjust(regulate);
}

void turnLeft() {
	startGyroInterrupts();
	float targetAngle = (int)getCurrentAngle() + 32;
	while(true) {
		driveRotateLeft(50, 50);
		if((int)getCurrentAngle() >= targetAngle) {
			stop();
			stopGyroInterrupts();
			return;
		}
		
	}
}

void turnRight() {
	startGyroInterrupts();
	int targetAngle = (int) getCurrentAngle() - 37;
	while(true) {
		driveRotateRight(50, 50);
		if((int)getCurrentAngle() <= targetAngle) {
			stop();
			stopGyroInterrupts();
			return;
		}
		
	}
}

/* Not using map atm */
void exploreLabyrinth() {
	
	/*
	Write main loop for exploring labyrinth.
	*/
	driveUntilNextCrossingOrDeadEnd: while(true) {
		/* What we want to do:
			- Drive forward until 
				- Wall is hit, if we can left or right, but not both, do that and keep driving.
				- A passage to either left or right is found and we can drive forward, stop.
				- A dead end is hit, stop.
		*/
		if(getFrontDistance() <= FRONT_CLOSED && getFrontRightDistance() <= SIDE_OPEN && getFrontLeftDistance() <= SIDE_OPEN) {
			/* Dead end is found */
			stop();
			/* Signal dead end found */
			turnLeft();
			turnLeft();
			break;
		} else if (getFrontDistance() <= FRONT_SPOTTED && getFrontDistance() >= FRONT_CLOSED+1) {
			iSeeWall: while (getFrontDistance() >= FRONT_CLOSED) {
				while(getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() >= SIDE_OPEN && getFrontDistance() >= FRONT_CLOSED && getFrontDistance() <= FRONT_SPOTTED) {
					leftRegulator();
				}
				while(getFrontLeftDistance() >= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN && getFrontDistance() >= FRONT_CLOSED && getFrontDistance() <= FRONT_SPOTTED) {
					rightRegulator();
				}
				pdRegulator();
			}
		} else if (getFrontDistance() <= FRONT_CLOSED) {
			/* A wall is met in front and its not a dead end. */
			if(getBackLeftDistance() >= SIDE_OPEN && getBackRightDistance() >= SIDE_OPEN) {
				/* It's a T-crossing, stop */
				stop();
				break;
			}else if(getFrontLeftDistance() >= SIDE_OPEN) {
				/* It's a left turn, turn left */
				turnLeft();
				/* If robot sees a wall a head, go straight to it */
				if(getFrontDistance() <= FRONT_SPOTTED) {
					while(getFrontDistance() >= FRONT_CLOSED) {
						driveForward(50, 50);
					}
				}
				/* While left side is open, regulate only to drive perpendicular to right wall */
				while(getBackLeftDistance() >= SIDE_OPEN) {
					rightRegulator();
				}
			} else if (getFrontRightDistance() >= SIDE_OPEN) {
				/* It's a right turn, turn right */
				turnRight();
				/* If robot sees a wall a head, go straight to it */
				if(getFrontDistance() <= FRONT_SPOTTED) {
					while(getFrontDistance() >= FRONT_CLOSED) {
						driveForward(50, 50);
					}
				}
				/* While left side is open, regulate only to drive perpendicular to left wall */
				while(getBackRightDistance() >= SIDE_OPEN) {
					leftRegulator();
				}
			}
		} else if(getFrontDistance() >= FRONT_SPOTTED && (getBackLeftDistance() >= SIDE_OPEN || getBackRightDistance() >= SIDE_OPEN)) {
			/* No wall in front and either a left or right turn is met, stop */
			stop();
			/* signal crossing found */
			turnRight();
			turnRight();
			break;
		} else {
			pdRegulator();			
		}
	}
	
	//
	//exploring: while(1) {
		//goingForward: while(getFrontDistance() >= MIN_DISTANCE_TO_FRONT_WALL) {
			//// Entering crossing
			//if(((getFrontLeftDistance() >= 50) || (getFrontRightDistance() >= 50)) && !inCrossing && !exitingCrossing && !enteringCrossing) {
				//enteringCrossing = true;
				//inCrossing = false;
				//exitingCrossing = false;
			//} else if(((getBackLeftDistance() >= 50) || (getBackRightDistance() >= 50)) && enteringCrossing && !inCrossing && !exitingCrossing) {
				//enteringCrossing = false;
				//inCrossing = true;
				//exitingCrossing = false;
				//
				//stop();
				//
				///* Here we are in the middle of a crossing */
				///* We know need to decide which direction to go */
				//if(turns < 2) {
					//turnLeft();
					//turns++;
				//} else {
					//turnRight();
					//turns++;
				//}
				//
				//if(turns == 4) {
					//turns = 0;
				//}
				//
				//// update location, we are in a crossing.
			//} else if(((getFrontLeftDistance() <= 50) && (getFrontRightDistance() <= 50)) && inCrossing) {
				//enteringCrossing = false;
				//inCrossing = false;
				//exitingCrossing = true;
			//} else if(((getBackLeftDistance() <= 50) && (getBackRightDistance() <= 50)) && exitingCrossing) {
				//enteringCrossing = false;
				//inCrossing = false;
				//exitingCrossing = false;
			//}
			//
			//if(enteringCrossing || inCrossing || exitingCrossing) {
				//driveForward(50, 50);
			//} else {
				//pdRegulate();
			//}
		//}
		//stop();
		//
		///* If we are in a dead end, make a 180 */
		//deadEnd: if(getFrontDistance() <= 25 && getFrontLeftDistance() <= 25 && getFrontRightDistance() <= 25) {
			//turnLeft();
			//turnLeft();
		//}
		///* If we are in a T-crossing, alternate between right and left turn */
		//tCross: if(tTurn) {
			//
		//}
		//
	//}
}

	//Move the equivalent of one node
static void moveOneNode(){
	
}



