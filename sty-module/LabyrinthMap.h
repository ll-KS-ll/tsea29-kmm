/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Storage and functions for the labyrinth map structure		*
 * Language: C															*
 * File type: Header													*
 *																		*
 ************************************************************************/



#ifndef LABYRINTHMAP_H_
#define LABYRINTHMAP_H_

#include <stdio.h>
#include <stdlib.h>
#include "sensorValues.h"

#define x_size 24
#define y_size 24

//enum
typedef enum {UNEXPLORED, EXPLORED, START, FESTISBOX} nodeStatus;
typedef enum {OPEN, CLOSED} ways;
typedef enum {NORTH, EAST, SOUTH, WEST, DUNNO}	dir;

//struct
typedef struct node {
    nodeStatus status;
    ways neighbours[4];
} node;


dir correctPath[x_size][y_size];

//functions
void initMap();
void addNode(dir curDir);
void getCurrentNodeStatus(int, int);
void identifyNeighbours(dir curDir);

void findClosestUnexplored();
bool recursiveFindUnExplored(int, int);

void setX(int x);
void setY(int y);
int getX();
int getY();

void traverseGetOut();
void selectNewPosition(int ,int);
void getCurrentNodeNeighbour(int, int);
void resetVisited();


#endif /* LABYRINTHMAP_H_ */
