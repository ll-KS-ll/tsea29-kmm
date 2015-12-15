/************************************************************************
 *																		*
 * Author: Güntech - Lukas Lindqvist		                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 * Version: 1.0															*
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

#define ONCE 1
#define TWICE 2

static bool atFestisBox = false;
static bool foundFestisBoxAlready = false;
static dir currentDir = NORTH;

/*
 * pdReg() - returns data using all four ir-sensors to drive in the middle and perpendicular to walls on both sides
 */
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

/*
 * alignLeft() - returns data that indicates how wrongly the robot is turned in respect to the wall on the left
 */
int alignLeft(){
	int t = 0;		
	
	// get the distances
	int fl = getFrontLeftDistance();
	int bl = getBackLeftDistance();
	
	// t = How wrongly the robot is rotated
	t = (fl - bl);
	
	return t;
}

/*
 * leftReg() - returns data using leftside ir-sensors to drive in the perpendicular to the walls and not to close to them
 */
int leftReg(){
	int u = 0;
	int t = 0;
	
	// get the distances
	int fl = getFrontLeftDistance();
	int bl = getBackLeftDistance();
	
	if(fl < 13 && bl < 13) {
		return -10;
	}
	// t = How wrongly the robot is rotated
	t = (fl - bl);
	
	u = 5 * t;
	
	return u;
}

/*
 * alignLeft() - returns data that indicates how wrongly the robot is turned in respect to the wall on the right
 */
int alignRight(){
	int t = 0;
	
	// get the distances
	int fr = getFrontRightDistance();
	int br = getBackRightDistance();

	// t = How wrongly the robot is rotated
	t = (br - fr);
	
	return t;

}

/*
 * leftReg() - returns data using rightside ir-sensors to drive in the perpendicular to the walls and not to close to them
 */
int rightReg(){
	int u = 0;
	int t = 0;		
	
	// get the distances
	int fr = getFrontRightDistance();
	int br = getBackRightDistance();

	if(fr < 13 && br < 13) {
		return 10;
	}
	
	// t = How wrongly the robot is rotated
	t = (br - fr);

	u = 5 * t; 
	
	return u;
}

/*
 * leftRegulator() - Gets the value from leftReg() and then adjusts the robot using the adjust() function
 */
void leftRegulator() {
	int regulate = leftReg();
	adjust(regulate);
}

/*
 * rightRegulator() - Gets the value from rightReg() and then adjusts the robot using the adjust() function
 */
void rightRegulator() {
	int regulate = rightReg();
	adjust(regulate);
}

/*
 * pdRegulator() - Gets the value from pdReg() and then adjusts the robot using the adjust() function
 */
void pdRegulator() {
	int regulate = pdReg();
	adjust(regulate);
}

/*
 * regulateRobot() - Depending on which walls it can see on both sides it uses the appropriate regulation
 */
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

/*
 * findNextTurnCrossingOrDeadend() - Does exactly what the name implies
 */
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
			_delay_ms(100);
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
		} else if (getFrontDistance() <= FRONT_CLOSED && getFrontRightDistance() <= SIDE_OPEN && getFrontLeftDistance() >= SIDE_OPEN) {
			stop();
			return;	
		} else {
			regulateRobot();
		}
		

}


/*
 * Creates a timer that will overflow every 10ms. So when oneSquare has reached 100, one seconds has pased.
 */
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

/*
 * inPath() - returns true if it can see walls on both sides.
 */
bool inPath() {
	if(getFrontDistance >= SIDE_OPEN && (getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN)) {
		return true;
	} else {
		return false;
	}
}

/*
 * moveOneNode(nodeStatusFind) - Drives one node using oneSquare timer. If it finds tape and is looking for it, it will stop.
 */
void moveOneNode(nodeStatus find){
	startOneSquareInterrupts();
	
	while(true) {
		findNextTurnCrossingOrDeadend();
		if(getSeesTape() && !foundFestisBoxAlready) {
			atFestisBox = true;
		}
		if(find == FESTISBOX && getSeesTape()) {
			stopOneSquareInterrupts();
			stop();
			break;
		}
		if(oneSquare >= ONE_SQUARE || getFrontDistance() <= FRONT_CLOSED) {
			stopOneSquareInterrupts();
			stop();
			break;
		}
	}
}

/*
 * exitCrossingOrTurn(nodeStatusFind) - Drives one node using oneSquare timer. If it finds tape and is looking for it, it will stop.
 */
void exitCrossingOrTurn(nodeStatus find) {
	startOneSquareInterrupts();
	while(true) {
		regulateRobot();
		if(getSeesTape() && !foundFestisBoxAlready) {
			atFestisBox = true;
		}
		if(find == FESTISBOX && getSeesTape()) {
			stopOneSquareInterrupts();
			stop();
			break;
		}
		if(oneSquare >= ONE_SQUARE || getFrontDistance() <= FRONT_CLOSED) {
			stopOneSquareInterrupts();
			stop();
			break;
		}
	}
}

/*
 * advOneNodeInCorrectPath(dir map[][], nodeStatus find) - Advances one node in the direction given by the 2d-array passed into
 * the function. 
 */
void advOneNodeInCorrectPath(dir map[][x_size], nodeStatus find) {
	switch(currentDir) {
		case NORTH:
			if(map[getX()][getY()] == NORTH) {
				if(inPath()) {
					moveOneNode(find);
				} else {
					exitCrossingOrTurn(find);
				}
				setY(getY()-1);
			} else if(map[getX()][getY()] == WEST) {
				turnLeft(ONCE);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = WEST;
				setX(getX()-1);
			} else if(map[getX()][getY()] == EAST) {
				turnRight(ONCE);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = EAST;
				setX(getX()+1);
			} else if(map[getX()][getY()] == SOUTH){
				if((getBackLeftDistance() + getFrontLeftDistance()) >= (getBackRightDistance() + getFrontRightDistance())) {
					turnLeft(TWICE);
				} else {
					turnRight(TWICE);
				}
				
				correctSelf();
				
				if(inPath()) {
					moveOneNode(find);
				} else {
					exitCrossingOrTurn(find);
				}
				currentDir = SOUTH;
				setY(getY()+1);
			}
			break;
		case WEST:
			if(map[getX()][getY()] == WEST) {
				if(inPath()) {
					moveOneNode(find);
				} else {
					exitCrossingOrTurn(find);
				}
				setX(getX()-1);
			}else if(map[getX()][getY()] == SOUTH) {
				turnLeft(ONCE);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = SOUTH;
				setY(getY()+1);
			} else if(map[getX()][getY()] == NORTH) {
				turnRight(ONCE);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = NORTH;
				setY(getY()-1);
			} else if(map[getX()][getY()] == EAST){
				if((getBackLeftDistance() + getFrontLeftDistance()) >= (getBackRightDistance() + getFrontRightDistance())) {
					turnLeft(TWICE);
				} else {
					turnRight(TWICE);
				}
				
				correctSelf();
				
				if(inPath()) {
					moveOneNode(find);
				} else {
					exitCrossingOrTurn(find);
				}
				currentDir = EAST;
				setX(getX()+1);
			}
			break;
		case SOUTH:
			if(map[getX()][getY()] == SOUTH) {
				if(inPath()) {
					moveOneNode(find);
				} else {
					exitCrossingOrTurn(find);
			}
				setY(getY()+1);
			}else if(map[getX()][getY()] == EAST) {
				turnLeft(ONCE);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = EAST;
				setX(getX()+1);
			} else if(map[getX()][getY()] == WEST) {
				turnRight(ONCE);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = WEST;
				setX(getX()-1);
			} else if(map[getX()][getY()] == NORTH){
				if((getBackLeftDistance() + getFrontLeftDistance()) >= (getBackRightDistance() + getFrontRightDistance())) {
					turnLeft(TWICE);
				} else {
					turnRight(TWICE);
				}
				
				correctSelf();
				
				if(inPath()) {
					moveOneNode(find);
				} else {
					exitCrossingOrTurn(find);
				}
				currentDir = NORTH;
				setY(getY()-1);
			}
			break;
		case EAST:
			if(map[getX()][getY()] == EAST) {
				if(inPath()) {
					moveOneNode(find);
				} else {
					exitCrossingOrTurn(find);
				}
				setX(getX()+1);
			}else if(map[getX()][getY()] == NORTH) {
				turnLeft(ONCE);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = NORTH;
				setY(getY()-1);
			} else if(map[getX()][getY()] == SOUTH) {
				turnRight(ONCE);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = SOUTH;
				setY(getY()+1);
			} else if(map[getX()][getY()] == WEST){
				if((getBackLeftDistance() + getFrontLeftDistance()) >= (getBackRightDistance() + getFrontRightDistance())) {
					turnLeft(TWICE);
				} else {
					turnRight(TWICE);
				}
			
				correctSelf();
			
				if(inPath()) {
					moveOneNode(find);
				} else {
					exitCrossingOrTurn(find);
				}
				currentDir = WEST;
				setX(getX()-1);
			}
			break;
	}
}

/*
 * placeSlefCloserToWall() - Drives up to a wall infront of it
 */
void placeSelfCloserToWall() {
	while(getFrontDistance() >= 23 && getFrontDistance() <= 35) {
		driveForward(20, 20);
	}
	stop();
}

/*
 * correctSelf() - Straightens out robot so its perpendicular to any walls beside it
 */
void correctSelf() {
	if(getFrontLeftDistance() <= SIDE_OPEN && getBackLeftDistance() <= SIDE_OPEN && getFrontLeftDistance() >= 12 && getBackRightDistance() >= 12) {
		while(alignLeft() < 1) {
			driveRotateRight(20, 20);
		}
		while(alignLeft() > 1) {
			driveRotateLeft(20, 20);
		}
	}
	stop();
	if(getFrontRightDistance() <= SIDE_OPEN && getBackRightDistance() <= SIDE_OPEN && getFrontRightDistance() >= 12 && getBackRightDistance() >= 12) {
		while(alignRight() > 1 ) {
			driveRotateLeft(20, 20);
		}
		while(alignRight() < 1) {
			driveRotateRight(20, 20);
		}
	}
	stop();
}

/*
 * moveToNode(nodeStatus find) - Finds the closest node of type given
 * as parameter and drives there.
 */
bool moveToNode(nodeStatus find)
{
	int path = findClosest(find);
	if(path == north) {
		while(correctPathNorth[getX()][getY()] != DUNNO) {
			advOneNodeInCorrectPath(correctPathNorth, find);
			placeSelfCloserToWall();
			correctSelf();
		}
	} else if(path == east) {
		while(correctPathEast[getX()][getY()] != DUNNO) {
			advOneNodeInCorrectPath(correctPathEast, find);
			placeSelfCloserToWall();
			correctSelf();
		}
	} else if(path == south) {
		while(correctPathSouth[getX()][getY()] != DUNNO) {
			advOneNodeInCorrectPath(correctPathSouth, find);
			placeSelfCloserToWall();
			correctSelf();
		}
	} else if(path == west) {
		while(correctPathWest[getX()][getY()] != DUNNO) {
			advOneNodeInCorrectPath(correctPathWest, find);
			placeSelfCloserToWall();
			correctSelf();
		}
	} else if(path == none) {
		/* No more unexplored */
		return true;
	}		
	return false;
}

/*
 * followTape() - Regulation for following the tape
 */
void followTape() {
	unsigned int regValue = getTapeReg();
	
	if(regValue < 6) {
		driveRotateRight(18, 18);
	} else if(regValue > 6) {
		driveRotateLeft(18, 18);
	} else {
		driveForward(18, 18);
	}
}

/*
 * solveLabyrinth() - Solves the labyrinth
 */
void solveLabyrinth() {
	bool labyrinthExplored = false;
	bool festisBoxReached = false;
	bool exitedLabyrinth = false;
	
	/*
		Main loop for exploring labyrinth.
	*/
	while(!labyrinthExplored) {
		labyrinthExplored = moveToNode(UNEXPLORED);
		_delay_ms(200);
		if(atFestisBox && !foundFestisBoxAlready) {
			addFestisNode(currentDir);
			atFestisBox = false;
			foundFestisBoxAlready = true;
		} else {
			addNode(currentDir);
		}
	}
	// Checks if the last square it explored is the square with the item
	// If so it will pick it up
	if(getCurrentNodeStatus(getX(), getY()) == FESTISBOX) {
		driveReverse(30, 30);
		_delay_ms(2000);
		stop();
		while(!getSeesTape()) {
			driveForward(40, 40);
		}
		stop();
	} else {
		// Go home after exploring labyrinth, then drive to item
		// Pick it up and go back home.
		exitedLabyrinth = moveToNode(START);
		festisBoxReached = moveToNode(FESTISBOX);	
	}
	
	// Picks up item
	openClaw();
	driveForward(20, 20);
	_delay_ms(200);
	stop();
	while(!getSeesTape()) {
		followTape();
	}
	stop();
	
	lowerClaw();
	_delay_ms(500);
	closeClaw();
	_delay_ms(500);
	raiseClaw();

	// Go back to start
	exitedLabyrinth = moveToNode(START);
	
	while(!getSeesTape()) {
		driveForward(50, 50);
	}
	

}