/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Storage and functions for the labyrinth map structure		*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/



#ifndef LABYRINTHMAP_H_
#define LABYRINTHMAP_H_

#include <stdio.h>
#include <stdlib.h>
#include "autonomous.h"
#include "sensorValues.h"

//enum
typedef enum {UNEXPLORED, EXPLORED, START, FESTISBOX} nodeStatus;	
typedef enum {OPEN, CLOSED} ways;
typedef enum {NORTH, EAST, SOUTH, WEST}	dir;

//structs

typedef struct {
	 nodeStatus status;
	 openNeighbours neighbours[];
} node;

typedef struct {
	dir p;
	struct path *next;
} path;
 
//var
int north = 0;
int east = 1;
int south = 2;
int west = 3;
int x_size = 32;
int y_size = 32;
int xpos = 15;
int ypos = 31;
dir currentDirection = NORTH;
//func 
/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include "labyrinthMap.h"

node lastNode;
node labyrinth[x_size][y_size];		//increase size if necessary
dir targetPath;

void initMap();
void addNode();
void updateNewNeighbours();
openNeighbours[] identifyNeighbours();
path nextTarget();
void traverseLabyrint(int, int, path);
path getOut();
void traverseGetOut();
void updatePosition();


#endif /* LABYRINTHMAP_H_ */