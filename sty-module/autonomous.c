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

#define kp 1
#define kd 1

static bool drivingForward = false;

direction currentDirection = north;

/* INFO
	360 degrees == 1024 in angle-value
	90 degrees == 256 in angle-value
	Turning left == angle-value increase
	Turning right == angle-value decreases

*/

/* Not using map atm */
void exploreLabyrinth() {
	/*
	Write main loop for exploring labyrinth.
	*/
	exploring: while(1) {
		drivingForward: while(getFrontDistance() <= MIN_DISTANCE_TO_FRONT_WALL) {
			goStraight();
		}
		stop();
	}
	
	
}

void goStraight() {
	if(pdRegulator() > 0) {
		adjustLeft();
	} else if (pdRegulator() < 0) {
		adjustRight();
	}
	goForwardWithCurrentSpeed();
}

/* Private function used for rotating 90 degrees left */
/* Unknown in-data from gyro */
//static void rotateLeftOnce() {
	//uint16_t currentAngle = getCurrentAngle();
	//uint16_t endAngle = currentAngle + 256;
	//driveRotateLeft();
	//setMotorSpeed(350, 350);
	//while (1) {
		//currentAngle = getCurrentAngle();
		//if(currentAngle >= endAngle) {
			//stop();
			//break;
		//}
		//_delay_ms(10);
	//}
//}
//
///* Private function used for rotating 90 degrees right */
///* Unknown in-data from gyro */
//static void rotateRightOnce() {
	//uint16_t currentAngle = getCurrentAngle();
	//uint16_t endAngle = currentAngle - 256;
	//driveRotateRight();
	//setMotorSpeed(350, 350);
	//while (1) {
		//currentAngle = getCurrentAngle();
		//if(currentAngle >= endAngle) {
			//stop();
			//break;
		//}
		//_delay_ms(10);
	//}
//}

/* Private function used by algorithms to rotate robot on the spot to the desired direction */
static void rotateOnSpot(direction targetDirection) {
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

	//Move the equivalent of one node
static void moveOneNode(){
	
}

/* Using PD-regulator to make robot drive in middle of corridor */
/*
	u[n] = Kp * e[n] + Kd * (e[n]-e[n-1])
	u[n] -> how much to turn. u[n] < 0 turn right, u[n] > 0 turn left, u[n] = 0 go straight
	Kp	 -> constant
	e[n] ->	how wrong our direction is
	Kd	 -> constant
*/
int16_t pdRegulator(){
	int16_t u, e;
	static int16_t preE;
	
	/* Choose to regulate depending on which side of robot is closest to the walls */
	if(getFrontLeftDistance() + getBackLeftDistance() >= getFrontRightDistance() + getBackRightDistance()) {
		e = getBackLeftDistance() - getFrontLeftDistance();
		} else {
		e = getFrontRightDistance() - getBackRightDistance();
	}
	
	u = kd * e; // Regulate only so we drive perpendicular to the walls
	/* TO BE IMPLEMENTED */
	// Regulate so we also drive in the middle between the walls on either side
	// + kp * (e-preE);
	
	preE = e;
	
	return u;
}

