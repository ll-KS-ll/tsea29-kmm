/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 *																		*
 ************************************************************************/

#define F_CPU 15000000UL
#include "labyrinthMap.h"
#include "variables.h"
#include "clawKernel.h"
#include <util/delay.h>

//var
#define north 0
#define east 1
#define south 2
#define west 3


static int xpos = 11;
static int ypos = 11;
static node labyrinth[x_size][y_size];		//increase size if necessary


node create_default_node() {
	node n;
	n.status = UNEXPLORED;
	n.neighbours[0] = CLOSED;
	n.neighbours[1] = CLOSED;
	n.neighbours[2] = CLOSED;
	n.neighbours[3] = CLOSED;
		
	return n;
}

//insert the first node to the map
void initMap() {
	
	// Set all nodes to unexplored
	for(int x = 0; x < x_size; x++) {
		for(int y = 0; y < y_size; y++) {
			labyrinth[x][y] = create_default_node();
			correctPath[x][y] = DUNNO;
		}
	}
	

	
	// Set start node to start and update walls.
	labyrinth[xpos][ypos].status = START;
	updateWalls(NORTH);
	//set wall behind robot when starting as closed
	labyrinth[xpos][ypos].neighbours[south] = CLOSED;
}

// When robot has reached unexplored, set it as explored and update its walls.
void addNode(dir curDir){
    if(labyrinth[xpos][ypos].status == UNEXPLORED){
        labyrinth[xpos][ypos].status = EXPLORED;
		updateWalls(curDir);
    }
}

void updateWalls(dir curDir) {
	int front;
	int left;
	int right;
	int back;
	
	switch(curDir) {
		 case NORTH:
			front = north;
			right = east;
			back = south;
			left = west;
			break;
		 case EAST:
			 front = east;
			 right = south;
			 back = west;
			 left = north;
			 break;
		 case SOUTH:
			front = south;
			right = west;
			back = north;
			left = east;
			break;
		 case WEST:
			front = west;
			right = north;
			back = east;
			left = south;
			break;
	}
	
	if (getFrontDistance() >= SIDE_OPEN){
		labyrinth[xpos][ypos].neighbours[front] = OPEN;
	}
	if (getFrontRightDistance() >= SIDE_OPEN || getBackRightDistance() >= SIDE_OPEN){
		labyrinth[xpos][ypos].neighbours[right] = OPEN;
	}
	if (getFrontLeftDistance() >= SIDE_OPEN || getBackLeftDistance() >= SIDE_OPEN){
		labyrinth[xpos][ypos].neighbours[left] = OPEN;
	}
	
	labyrinth[xpos][ypos].neighbours[back] = OPEN;
}

static bool visited[x_size][y_size];
static bool found_way = false;;

//traverse the map via the nodes to a target node
void findClosestUnexplored(){
	
    resetVisited();
	resetCorrectPath();


    bool found_way = recursiveFindUnExplored(xpos, ypos);		
}

//help function to traverse the labyrinth
bool recursiveFindUnExplored(int x, int y){
	
    if(labyrinth[x][y].status == UNEXPLORED){
		return true;
    }
	
	if(visited[x][y]) {
		return false;
	}
	
    visited[x][y] = true;
	
	
	
	if (labyrinth[x][y].neighbours[north] == OPEN && y != 0){
		if(recursiveFindUnExplored(x, y-1)) {
			correctPath[x][y] = NORTH;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[east] == OPEN  && x < x_size - 1){
		if(recursiveFindUnExplored(x+1, y)) {
			correctPath[x][y] = EAST;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[south] == OPEN && y < y_size - 1){
		if(recursiveFindUnExplored(x, y+1)) {
			correctPath[x][y] = SOUTH;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[west] == OPEN && x != 0){
		if(recursiveFindUnExplored(x-1, y)) {
			correctPath[x][y] = WEST;
			return true;
		}
	}
    
	return false;
}


//make a good traverse function
void traverseGetOut(){

}

//read given path to determine where we are
void setX(int x) {
	xpos = x;
}

void setY(int y) {
	ypos = y;
}

int getX() {
	return xpos;
}

int getY() {
	return ypos;
}

void resetVisited(){
    for(int x = 0; x < x_size; x++){
        for(int y = 0; y < y_size; y++){
			visited[x][y] = false;
        }
    }
}

void resetCorrectPath() {
	for(int x = 0; x < x_size; x++) {
		for(int y = 0; y < y_size; y++) {
			correctPath[x][y] = DUNNO;
		}
	}
}