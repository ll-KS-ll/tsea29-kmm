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

int alignLeft(){
	int u = 0;
	int t = 0;		
	
	// get the distances
	int fl = getFrontLeftDistance();
	int bl = getBackLeftDistance();
	
	// t = How wrongly the robot is rotated
	t = (fl - bl) ;
	
	/* only align if error is worthy of aligning */
	if(t > 2 || t < -2) {
	
	u = 5 * t;
	
	return u;
		} else {
	return 0;
}
}

int leftReg(){
	int u = 0;
	int t = 0;
	
	// get the distances
	int fl = getFrontLeftDistance();
	int bl = getBackLeftDistance();
	
	if(fl < 16 && bl < 16) {
		return -10;
	}
	
	// t = How wrongly the robot is rotated
	t = (fl - bl);
	
	u = 5 * t;
	
	return u;
}

int alignRight(){
	int u = 0;
	int t = 0;
	
	// get the distances
	int fr = getFrontRightDistance();
	int br = getBackRightDistance();

	// t = How wrongly the robot is rotated
	t = (br - fr);
	/* only align if error is worthy of aligning */
	if(t > 2 || t < -2) {
		
		u = 5 * t;
		
		return u;
	} else {
		return 0;
	}

}

int rightReg(){
	int u = 0;
	int t = 0;		
	
	// get the distances
	int fr = getFrontRightDistance();
	int br = getBackRightDistance();

	if(fr < 16 && br < 16) {
		return 10;
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
			_delay_ms(50);
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

bool inPath() {
	if(getFrontDistance >= SIDE_OPEN && (getFrontLeftDistance() <= SIDE_OPEN && getFrontRightDistance() <= SIDE_OPEN)) {
		return true;
	} else {
		return false;
	}
}

//Move the equivalent of one node
void moveOneNode(nodeStatus find){
	startOneSquareInterrupts();
	while(true) {
		findNextTurnCrossingOrDeadend();
		if(find != FESTISBOX) {
			if(oneSquare >= ONE_SQUARE || getFrontDistance() <= FRONT_CLOSED) {
				stopOneSquareInterrupts();
				stop();
				break;
			}
			} else if(find == FESTISBOX) {
			if(oneSquare >= ONE_SQUARE || getFrontDistance() <= FRONT_CLOSED || getIsThereTape()) {
				stopOneSquareInterrupts();
				stop();
				break;
			}
		}
	}
}

void exitCrossingOrTurn(nodeStatus find) {
	startOneSquareInterrupts();
	while(true) {
		regulateRobot();
		if(find != FESTISBOX) {
			if(oneSquare >= ONE_SQUARE || getFrontDistance() <= FRONT_CLOSED) {
				stopOneSquareInterrupts();
				stop();
				break;
			}
		} else if(find == FESTISBOX) {
			if(oneSquare >= ONE_SQUARE || getFrontDistance() <= FRONT_CLOSED || getIsThereTape()) {
				stopOneSquareInterrupts();
				stop();
				break;
			}
		}
	}

static dir currentDir = NORTH;

void advOneNodeInCorrectPath(dir map[][x_size], nodeStatus find)
{
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
				turnLeft(1);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = WEST;
				setX(getX()-1);
			} else if(map[getX()][getY()] == EAST) {
				turnRight(1);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = EAST;
				setX(getX()+1);
			} else if(map[getX()][getY()] == SOUTH){
				if((getBackLeftDistance() + getFrontLeftDistance()) >= (getBackRightDistance() + getFrontRightDistance())) {
					turnLeft(2);
					} else {
					turnRight(2);
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
				turnLeft(1);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = SOUTH;
				setY(getY()+1);
			} else if(map[getX()][getY()] == NORTH) {
				turnRight(1);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = NORTH;
				setY(getY()-1);
			} else if(map[getX()][getY()] == EAST){
				if((getBackLeftDistance() + getFrontLeftDistance()) >= (getBackRightDistance() + getFrontRightDistance())) {
					turnLeft(2);
				} else {
					turnRight(2);
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
				turnLeft(1);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = EAST;
				setX(getX()+1);
			} else if(map[getX()][getY()] == WEST) {
				turnRight(1);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = WEST;
				setX(getX()-1);
			} else if(map[getX()][getY()] == NORTH){
				if((getBackLeftDistance() + getFrontLeftDistance()) >= (getBackRightDistance() + getFrontRightDistance())) {
					turnLeft(2);
					} else {
					turnRight(2);
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
				turnLeft(1);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = NORTH;
				setY(getY()-1);
			} else if(map[getX()][getY()] == SOUTH) {
				turnRight(1);
				correctSelf();
				exitCrossingOrTurn(find);
				currentDir = SOUTH;
				setY(getY()+1);
			} else if(map[getX()][getY()] == WEST){
				if((getBackLeftDistance() + getFrontLeftDistance()) >= (getBackRightDistance() + getFrontRightDistance())) {
					turnLeft(2);
					} else {
					turnRight(2);
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

void placeSelfCloserToWall() {
	while(getFrontDistance() >= 24 && getFrontDistance() <= 50) {
		driveForward(20, 20);
	}
	stop();
}

void correctSelf() {
	if(getFrontLeftDistance() <= SIDE_OPEN && getBackLeftDistance() <= SIDE_OPEN && getFrontLeftDistance() >= 15 && getBackRightDistance() >= 15) {
		while(alignLeft() < 0) {
			driveRotateRight(20, 20);
		}
		while(alignLeft() > 0) {
			driveRotateLeft(20, 20);
		}
	}
	stop();
	if(getFrontRightDistance() <= SIDE_OPEN && getBackRightDistance() <= SIDE_OPEN && getFrontRightDistance() >= 15 && getBackRightDistance() >= 15) {
		while(alignRight() > 0 ) {
			driveRotateLeft(20, 20);
		}
		while(alignRight() < 0) {
			driveRotateRight(20, 20);
		}
	}
	stop();
}

bool moveToNode(nodeStatus find)
{
	int path = findClosest(find);
	if(path == 0) {
		while(correctPathNorth[getX()][getY()] != DUNNO) {
			advOneNodeInCorrectPath(correctPathNorth, find);
			placeSelfCloserToWall();
			correctSelf();
		}
	} else if(path == 1) {
		while(correctPathEast[getX()][getY()] != DUNNO) {
			advOneNodeInCorrectPath(correctPathNorth, find);
			placeSelfCloserToWall();
			correctSelf();
		}
	} else if(path == 2) {
		while(correctPathWest[getX()][getY()] != DUNNO) {
			advOneNodeInCorrectPath(correctPathNorth, find);
			placeSelfCloserToWall();
			correctSelf();
		}
	} else if(path == 3) {
		while(correctPathSouth[getX()][getY()] != DUNNO) {
			advOneNodeInCorrectPath(correctPathNorth, find);
			placeSelfCloserToWall();
			correctSelf();
		}
	} else if(path == 4) {
		/* No more unexplored */
		return true;
	}		
	return false;
}

/* Not using map atm */
void exploreLabyrinth() {
	bool labyrinthExplored = false;
	bool festisBoxReached = false;
	bool exitedLabyrinth = false;
	
	
	openClaw();
	_delay_ms(200);
	
	/*
	Write main loop for exploring labyrinth.
	*/
	while(!labyrinthExplored) {
		labyrinthExplored = moveToNode(UNEXPLORED);
		addNode(currentDir);
	}
	
	lowerClaw();
	_delay_ms(500);
	closeClaw();
	_delay_ms(500);
	raiseClaw();
	
	// ******************************************
	// ACTIVATE CODE WHEN LINESENSOR IS FINISHED
	// ******************************************
	
	/*
	while(labyrinthExplored && !festisBoxReached) {
		festisBoxReached = moveToNode(FESTISBOX);
	}
	*/
	
	/*
		Insert code for following tape and picking up festisbox
	*/
	
	
	/* Go back to start */
	while(labyrinthExplored && !exitedLabyrinth) {
		exitedLabyrinth = moveToNode(START);
	}

}