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
#include "sensorValues.h"

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
			correctPathNorth[x][y] = DUNNO;
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
    if(labyrinth[xpos][ypos].status == UNEXPLORED && !getIsThereTape()) {
        labyrinth[xpos][ypos].status = EXPLORED;
		updateWalls(curDir);
    } else if(labyrinth[xpos][ypos].status == UNEXPLORED && getIsThereTape()) {
		labyrinth[xpos][ypos].status = FESTISBOX;
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


//traverse the map via the nodes to a target node
int findClosest(nodeStatus find){
	
    resetVisited();
	resetCorrectPaths();
    bool found_way_north = recursiveNorthFind(xpos, ypos, find);
    resetVisited();
    bool found_way_east = recursiveEastFind(xpos, ypos, find);
    resetVisited();
    bool found_way_west = recursiveWestFind(xpos, ypos, find);
    resetVisited();
    bool found_way_south = recursiveSouthFind(xpos, ypos, find);
	
	int cost_north = 0;
	int cost_east = 0;
	int cost_west = 0;
	int cost_south = 0;
	
	
	for(int x = 0; x < x_size - 1; x++) {
		for(int y = 0; y < y_size - 1; y++) {
			if(correctPathNorth[x][y] != DUNNO) {
				cost_north++;
			}
			if(correctPathEast[x][y] != DUNNO) {
				cost_east++;
			}
			if(correctPathWest[x][y] != DUNNO) {
				cost_west++;
			}
			if(correctPathSouth[x][y] != DUNNO) {
				cost_south++;
			}
		}
	}
	if(found_way_north && cost_north <= cost_east && cost_north <= cost_west && cost_north <= cost_south) {
		return 0;
	} else if(found_way_east && cost_east <= cost_west && cost_east <= cost_south) {
		return 1;
	} else if(found_way_west && cost_west <= cost_south) {
		return 2;
	} else if(found_way_south) {
		return 3;
	} else {
		return 4;
	}
	
}

//help function to traverse the labyrinth
bool recursiveNorthFind(int x, int y, nodeStatus find){
	
    if(labyrinth[x][y].status == find){
		return true;
    }
	
	if(visited[x][y]) {
		return false;
	}
	
    visited[x][y] = true;
	
	if (labyrinth[x][y].neighbours[north] == OPEN && y != 0){
		if(recursiveNorthFind(x, y-1, find)) {
			correctPathNorth[x][y] = NORTH;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[west] == OPEN && x != 0){
		if(recursiveNorthFind(x-1, y, find)) {
			correctPathNorth[x][y] = WEST;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[south] == OPEN && y < y_size - 1){
		if(recursiveNorthFind(x, y+1, find)) {
			correctPathNorth[x][y] = SOUTH;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[east] == OPEN  && x < x_size - 1){
		if(recursiveNorthFind(x+1, y, find)) {
			correctPathNorth[x][y] = EAST;
			return true;
		}
	}
	
	
	return false;
}

//help function to traverse the labyrinth
bool recursiveEastFind(int x, int y, nodeStatus find){
	
	if(labyrinth[x][y].status == find){
		return true;
	}
	
	if(visited[x][y]) {
		return false;
	}
	
	visited[x][y] = true;
	
	if (labyrinth[x][y].neighbours[east] == OPEN  && x < x_size - 1){
		if(recursiveEastFind(x+1, y, find)) {
			correctPathEast[x][y] = EAST;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[north] == OPEN && y != 0){
		if(recursiveEastFind(x, y-1, find)) {
			correctPathEast[x][y] = NORTH;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[west] == OPEN && x != 0){
		if(recursiveEastFind(x-1, y, find)) {
			correctPathEast[x][y] = WEST;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[south] == OPEN && y < y_size - 1){
		if(recursiveEastFind(x, y+1, find)) {
			correctPathEast[x][y] = SOUTH;
			return true;
		}
	}
	
	
	
	return false;
}

//help function to traverse the labyrinth
bool recursiveWestFind(int x, int y, nodeStatus find){
	
	if(labyrinth[x][y].status == find){
		return true;
	}
	
	if(visited[x][y]) {
		return false;
	}
	
	visited[x][y] = true;
	
	if (labyrinth[x][y].neighbours[west] == OPEN && x != 0){
		if(recursiveWestFind(x-1, y, find)) {
			correctPathWest[x][y] = WEST;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[north] == OPEN && y != 0){
		if(recursiveWestFind(x, y-1, find)) {
			correctPathWest[x][y] = NORTH;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[east] == OPEN  && x < x_size - 1){
		if(recursiveWestFind(x+1, y, find)) {
			correctPathWest[x][y] = EAST;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[south] == OPEN && y < y_size - 1){
		if(recursiveWestFind(x, y+1, find)) {
			correctPathWest[x][y] = SOUTH;
			return true;
		}
	}
	

	return false;
}

//help function to traverse the labyrinth
bool recursiveSouthFind(int x, int y, nodeStatus find){
	
	if(labyrinth[x][y].status == find){
		return true;
	}
	
	if(visited[x][y]) {
		return false;
	}
	
	visited[x][y] = true;
	
	if (labyrinth[x][y].neighbours[south] == OPEN && y < y_size - 1){
		if(recursiveSouthFind(x, y+1, find)) {
			correctPathSouth[x][y] = SOUTH;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[west] == OPEN && x != 0){
		if(recursiveSouthFind(x-1, y, find)) {
			correctPathSouth[x][y] = WEST;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[north] == OPEN && y != 0){
		if(recursiveSouthFind(x, y-1, find)) {
			correctPathSouth[x][y] = NORTH;
			return true;
		}
	}
	if (labyrinth[x][y].neighbours[east] == OPEN  && x < x_size - 1){
		if(recursiveSouthFind(x+1, y, find)) {
			correctPathSouth[x][y] = EAST;
			return true;
		}
	}
	
	
	
	return false;
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

void resetCorrectPaths() {
	for(int x = 0; x < x_size; x++) {
		for(int y = 0; y < y_size; y++) {
			correctPathNorth[x][y] = DUNNO;
			correctPathWest[x][y] = DUNNO;
			correctPathEast[x][y] = DUNNO;
			correctPathSouth[x][y] = DUNNO;
		}
	}
}