/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include "labyrinthMap.h"

int dir;
int north = 0;
int east = 1;
int south = 2;
int west = 3;
int x_size = 32;
int y_size = 32;
int xpos = 15;
int ypos = 31;
node lastNode;
node labyrinth[x_size][y_size];		//increase size if necessary
dir targetPath;

/*
	[ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
	
	[ ] [ ] [F] [U] [E] [ ] [ ] [ ]
	         H   |   |
	[U]-[E]-[E]=[E]=[E] [ ] [ ] [ ]
		 |	 |	 |	 H
	[U]-[E] [U] [U]-[E]-[U] [ ] [ ]
		 |		 |	 H
	[ ] [E]-[E]-[E]-[S]-[U] [ ] [ ]
*/
//insert the first node to the map
void initMap(){
//head
	ways neighbours[4];
	node currentNode;
//body
	//reset the map
	for (i = 0; i < x_size; i++){
		for (j = 0; j < y_size; j++){
			labyrinth[i][j].status = NULL;
			labyrinth[i][j].neighbours = NULL;
		} 
	}

	updateNewNeighbours();
	identifyNeighbours(&neighbours);	//get all neighbours
	currentNode.status = START;
	currentNode.neighbours = neighbours;
	labyrinth[15][30] = currentNode;	//insert the node in the map
}


void addNode(){
//head
	ways neighbours[4];
	node currentNode = labyrinth[xpos][ypos];
//body
	if(currentNode.status == UNEXPLORED){
		updateNewNeighbours();
		identifyNeighbours(&neighbours);
		currentNode.status = EXPLORED;
		currentNode.neighbours = neighbours;
		labyrinth[xpos][ypos] = currentNode;
	}
}


void updateNewNeighbours(){
	node n;
	n.status = UNEXPLORED;
	n.neighbours = NULL;
	
	//insert UNEXPLORED nodes around the currentnode
	if(!labyrinth[xpos][ypos-1]){	//north
		labyrinth[xpos][ypos-1] = n;
	}if(!labyrinth[xpos+1][ypos]){	//east
		labyrinth[xpos+1][ypos] = n;
	}if(!labyrinth[xpos][ypos+1]){	//south
		labyrinth[xpos][ypos+1] = n;
	}if(!labyrinth[xpos-1][ypos]){	//west
		labyrinth[xpos-1][ypos] = n;
	}	
}

//find all possible neighbours for the current node
void identifyNeighbours(ways *neighbours){
//init								//[north,west,south,east]
	
	
//body
	//do some magic with robot direction compared to the labyrinth
	getDirection();
	
	//determine north and change variables accordingly
	getFrontDistance();
	//check the distance for each direction
	
	
	neighbours[north] = OPEN;
	neighbours[east] = CLOSED;
	neighbours[south] = OPEN;
	neighbours[west] = OPEN;
	
	return;
}

//traverse the map via the nodes to a target node
path nextTarget(){
	path *targetPath;
	node currentNode = labyrinth[xpos][ypos];
	node tmpNode = currentNode;
	node targetNode;

	traverseLabyrint(xpos, ypos, &targetPath);
	
	return targetPath;
}

//help function to traverse the labyrinth
void traverseLabyrint(int x, int y, path *way){
	node currentNode = labyrinth[x][y];
	if (currentNode.status == UNEXPLORED){	//zero case
		return;
	} else {
		if (currentNode.neighbours[north] == OPEN){
			way->p = NORTH;
			way = way->next;
			traverseLabyrint(x,y-1,&way);
		} if (currentNode.neighbours[east] == OPEN){
			way->p = EAST;
			way = way->next;
			traverseLabyrint(x+1,y,&way);
		} if (currentNode.neighbours[south] == OPEN){
			way->p = SOUTH;
			way = way->next;
			traverseLabyrint(x,y+1,&way);
		} if (currentNode.neighbours[west] == OPEN){
			way->p = WEST;
			way = way->next;
			traverseLabyrint(x-1,y,&way);
		}
	}
	return;
}

//traverse to the start
path getOut(){
	path escapePath[];
	
	
	return escapePath;
}

void traverseGetOut()

//read motor-commands to determine where we are
void updatePosition(dir p){
	
	
}