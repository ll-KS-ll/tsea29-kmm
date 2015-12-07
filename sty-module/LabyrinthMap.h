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
typedef struct {
    nodeStatus status;
    ways neighbours[4];
} node;

struct path{
    dir p;
    path* next;
};

struct vector{
    path* next_path;
};


//var
const int north = 0;
const int east = 1;
const int south = 2;
const int west = 3;
const int x_size = 32;
const int y_size = 32;
static int xpos = 15;
static int ypos = 31;
static dir currentDirection = NORTH;
static node labyrinth[x_size][y_size];		//increase size if necessary
static bool visited[x_size][y_size];
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
