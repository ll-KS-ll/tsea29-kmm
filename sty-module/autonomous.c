/************************************************************************
 *																		*
 * Author: G�ntech							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#define F_CPU 15000000UL
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "motorKernel.h"
#include "autonomous.h"
#include "sensorValues.h"
#include "variables.h"
#include "labyrinthMap.h"

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

	/* KP and KD constants say how much the robot will react 
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

void findNextTurnCrossingOrDeadend()
{
		/* What we want to do:
			- Drive forward until 
				- Wall is hit, if we can left or right, but not both, do that and keep driving.
				- A passage to either left or right or both is found and we can drive forward, stop.
				- A dead end is hit, stop(do a 180).
		*/
		if(getFrontDistance() >= ONE_SQUARE_DISTANCE && (getBackLeftDistance() >= SIDE_OPEN || getBackRightDistance() >= SIDE_OPEN)) {
			/* All crossings except T-crossing is found */
			_delay_ms(150);
			stop();
			return;
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontLeftDistance() >= SIDE_OPEN && getFrontRightDistance() >= SIDE_OPEN) {
			/* T-crossing is found */
			stop();
			return;
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN) {
			/* A dead end is found, stop */
			stop();
			return;
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() >= SIDE_OPEN) {
			stop();
			return;
			
			/* Right turn found, make a turn and if no wall wall is spotted in front, leftRegulate until wall on right is found */
			turnRight();
			_delay_ms(100);

			while (getBackRightDistance() >= SIDE_OPEN && getBackLeftDistance() <= SIDE_OPEN) {
					regulateRobot();
			}
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontRightDistance() <= SIDE_OPEN && getFrontLeftDistance() >= SIDE_OPEN) {
			stop();
			return;
			
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


/* Timer interrupt:
	256 - (14 745 000 / 1024(prescaler) / 5(frequency)) = 144
	Set TCNT to 144 and it will overflow once every 10 ms. */
unsigned int oneSquare;

ISR(TIMER2_OVF_vect) {
	oneSquare++;
	TCNT2 = 144;
}

void startOneSquareInterrupts() {
	oneSquare = 0;
	TCNT2 = 144;
	TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);
}

void stopOneSquareInterrupts() {
	TCCR2B = (0<<CS22)|(0<<CS21)|(0<<CS20);
}


void initOneSquareTimer() {	
	TIMSK2 = (1<<TOIE2);
	TCNT2 = 144;
	TCCR2B = (0<<CS22)|(0<<CS21)|(0<<CS20);
	
}

bool inDeadEnd() {
	if(getFrontDistance <= SIDE_OPEN && getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN) {
		return true;
	} else {
		return false;
	}
}

bool inLeftTurn() {
	if(getFrontDistance <= SIDE_OPEN && getFrontLeftDistance() >= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN) {
		return true;
	} else {
		return false;
	}
}

bool inRightTurn() {
	if(getFrontDistance <= SIDE_OPEN && getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() >= SIDE_OPEN) {
		return true;
	} else {
		return false;
	}
}

bool inCrossing() {
	if(getFrontDistance >= SIDE_OPEN && (getFrontLeftDistance() >= SIDE_OPEN || getFrontRightDistance() >= SIDE_OPEN)) {
		return true;
	} else {
		return false;
	}
}

bool inPath() {
	if(getFrontDistance >= SIDE_OPEN && (getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN)) {
		return true;
	} else {
		return false;
	}
}

//Move the equivalent of one node
void moveOneNode(){
	startOneSquareInterrupts();
	while(true) {
		findNextTurnCrossingOrDeadend();
		if(oneSquare >= ONE_SQUARE) {
			stopOneSquareInterrupts();
			stop();
			break;
		}
	}
}

void exitCrossingOrTurn() {
	startOneSquareInterrupts();
	while(true) {
		regulateRobot();
		if(oneSquare >= ONE_SQUARE) {
			stopOneSquareInterrupts();
			stop();
			break;
		}
	}
}

static dir currentDir = NORTH;
static dir targetDir = DUNNO;

void advanceOneNodeInPath()
{
	switch(currentDir) {
		case NORTH:
			if(targetDir == NORTH) {
				if(inPath()) {
					moveOneNode();
				} else {
					exitCrossingOrTurn();
				}
			} else if(targetDir == WEST) {
				turnLeft();
				exitCrossingOrTurn();
				currentDir = WEST;
			} else if(targetDir == EAST) {
				turnRight();
				exitCrossingOrTurn();
				currentDir = EAST;
			} else {
				turnLeft();
				turnLeft();
				moveOneNode();
				currentDir = SOUTH;
			}
			break;
		case WEST:
			if(targetDir = WEST) {
				if(inPath()) {
					moveOneNode();
				} else {
					exitCrossingOrTurn();
				}
			}else if(targetDir = SOUTH) {
				turnLeft();
				exitCrossingOrTurn();
				currentDir = SOUTH;
			} else if(targetDir == NORTH) {
				turnRight();
				exitCrossingOrTurn();
				currentDir = NORTH;
			} else {
				turnLeft();
				turnLeft();
				moveOneNode();
				currentDir = EAST;
			}
			break;
		case SOUTH:
			if(targetDir = SOUTH) {
				if(inPath()) {
					moveOneNode();
				} else {
					exitCrossingOrTurn();
				}
			}else if(targetDir = EAST) {
				turnLeft();
				exitCrossingOrTurn();
				currentDir = EAST;
			} else if(targetDir == WEST) {
				turnRight();
				exitCrossingOrTurn();
				currentDir = WEST;
			} else {
				turnLeft();
				turnLeft();
				moveOneNode();
				currentDir = NORTH;
			}
			break;
		case EAST:
			if(targetDir = EAST) {
				if(inPath()) {
					moveOneNode();
				} else {
					exitCrossingOrTurn();
				}
			}else if(targetDir = NORTH) {
				turnLeft();
				exitCrossingOrTurn();
				currentDir = NORTH;
			} else if(targetDir == SOUTH) {
				turnRight();
				exitCrossingOrTurn();
				currentDir = SOUTH;
			} else {
				turnLeft();
				turnLeft();
				moveOneNode();
				currentDir = WEST;
			}
			break;
	}
}

void testShit()
{
	driveForward(50,50);
	_delay_ms(50);
	stop();
	_delay_ms(100);
}

/* Not using map atm */
void exploreLabyrinth() {
	bool labyrinthExplored = false;
	
	_delay_ms(200);
	
	
	/*
	Write main loop for exploring labyrinth.
	*/
	while(!labyrinthExplored) {
		path *ourPath = malloc(sizeof(path));
		
		_delay_ms(500);
		
		ourPath = nextTarget();

		//if(!found_way) {
			//testShit();
			//break;
		//}
		
		while(ourPath->p != DUNNO) {
			path *tmp = malloc(sizeof(path));
			
			lowerClaw();
			_delay_ms(500);
			raiseClaw();
			_delay_ms(500);
			
			targetDir = ourPath->p;
			
			advanceOneNodeInPath();
			
			_delay_ms(2000);
			
			tmp = ourPath;
			ourPath = ourPath->next;
			free(tmp);
		
		}
		addNode();
		
		
		openClaw();
		_delay_ms(500);
		closeClaw();
		_delay_ms(500);
		
		free(ourPath);
	}
	//
	openClaw();
	_delay_ms(500);
	//lowerClaw();
	//_delay_ms(500);
	closeClaw();
	_delay_ms(500);
	//raiseClaw();
	
}





