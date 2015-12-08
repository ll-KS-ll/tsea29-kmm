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

void regulateRobot() {
	if(getBackLeftDistance() <= SIDE_OPEN && getFrontLeftDistance() <= SIDE_OPEN && getBackRightDistance() <= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN) {
		pdRegulator();
	} else if (getBackLeftDistance() <= SIDE_OPEN && getFrontLeftDistance() <= SIDE_OPEN) {
		leftRegulator();
	} else if (getBackRightDistance() <= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN) {
		rightRegulator();
	} else {
		driveForward(50, 50);
	}
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
	_delay_ms(1000);
	/*
	Write main loop for exploring labyrinth.
	*/
	driveUntilNextCrossingOrDeadEnd: while(true) {
		/* What we want to do:
			- Drive forward until 
				- Wall is hit, if we can left or right, but not both, do that and keep driving.
				- A passage to either left or right or both is found and we can drive forward, stop.
				- A dead end is hit, stop(do a 180).
		*/
		if(getFrontDistance() >= ONE_SQUARE && (getBackLeftDistance() >= SIDE_OPEN || getBackRightDistance() >= SIDE_OPEN)) {
			/* All crossings except T-crossing is found */
			_delay_ms(100);
			stop();
			break;
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontLeftDistance() >= SIDE_OPEN && getFrontRightDistance() >= SIDE_OPEN) {
			/* T-crossing is found */
			stop();
			break;
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN) {
			/* A dead end is found, stop */
			stop();
			turnLeft();
			turnLeft();
			//break;
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() >= SIDE_OPEN) {
			/* Right turn found, make a turn and if no wall wall is spotted in front, leftRegulate until wall on right is found */
			turnRight();
			_delay_ms(100);

			while (getBackRightDistance() >= SIDE_OPEN && getBackLeftDistance() <= SIDE_OPEN) {
					regulateRobot();
			}
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontRightDistance() <= SIDE_OPEN && getFrontLeftDistance() >= SIDE_OPEN) {
		/* Left turn found, make a turn and if no wall wall is spotted in front, rightRegulate until wall on right is found */
			turnLeft();
			_delay_ms(100);
			
			while (getBackLeftDistance() >= SIDE_OPEN && getBackRightDistance() <= SIDE_OPEN) {
				regulateRobot();
			}
			
		} else {
			regulateRobot();
		}
		
	}
}

	//Move the equivalent of one node
static void moveOneNode(){
	
}



