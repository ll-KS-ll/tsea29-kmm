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


//enum
typedef enum {UNKNOWN, UNEXPLORED, EXPLORED, START, FESTISBOX} nodeStatus;
typedef enum {OPEN, CLOSED} ways;
typedef enum {NORTH, EAST, SOUTH, WEST, DUNNO}	dir;

//struct
typedef struct node {
    nodeStatus status;
    ways neighbours[4];
} node;

typedef struct path {
    dir p;
    struct path* next;
} path;

typedef struct vector {
    path* next_path;
} vector;


static bool found_way;

//functions
void initMap();
void addNode();
void updateNewNeighbours();
void getCurrentNodeStatus(int, int);
void identifyNeighbours();
path *nextTarget();
path *traverseLabyrint(int, int, dir);
path getOut();
void traverseGetOut();
void updatePosition(path *);
void selectNewPosition(int ,int);
void getCurrentNodeNeighbour(int, int);
void resetVisited();


#endif /* LABYRINTHMAP_H_ */
