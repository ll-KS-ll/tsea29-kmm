/************************************************************************
 *																		*
 * Author: Güntech - Jacob Johansson/Lukas Lindqvist                    *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Header													*
 * Version: 1.0															*
 *																		*
 ************************************************************************/



#ifndef LABYRINTHMAP_H_
#define LABYRINTHMAP_H_

#include <stdio.h>
#include <stdlib.h>
#include "sensorValues.h"

#define x_size 24
#define y_size 24
#define north 0
#define east 1
#define south 2
#define west 3
#define none 4

//enums
typedef enum {UNEXPLORED, EXPLORED, START, FESTISBOX} nodeStatus;
typedef enum {OPEN, CLOSED} ways;
typedef enum {NORTH, EAST, SOUTH, WEST, DUNNO}	dir;

//struct
typedef struct node {
    nodeStatus status;
    ways neighbours[4];
} node;

//Maps of driving paths
dir correctPathNorth[x_size][y_size];
dir correctPathWest[x_size][y_size];
dir correctPathEast[x_size][y_size];
dir correctPathSouth[x_size][y_size];

/*
 * initMap() - Places robot in map and sets up starting node. And resets all other nodes.
 */
void initMap();

/*
 * addNode(dir curDir) - Adds the node the robot is on to the map
 */
void addNode(dir curDir);

/*
 * addFestisNode(dir curDir) - Sets the current node as the node with the item.
 */
void addFestisNode(dir curDir);

/*
 * getCurrentNodeStatus(int x, int y) - Returns the nodeStatus of the node on the given location
 */
nodeStatus getCurrentNodeStatus(int x, int y);

/*
 * findClosest(nodeStatus find) - Using the recursive functions it finds the shortest path to the given nodeStatus ie. UNEXPLORED, START or FESTISBOX
 */
int findClosest(nodeStatus find);

/*
 * recursiveDirectionFind(int x, int y, nodeStatus find) - Recursive functions used to the find the cloeset path from given position to a nodeStatus
 */
bool recursiveNorthFind(int x, int y, nodeStatus find);
bool recursiveEastFind(int x, int y, nodeStatus find);
bool recursiveWestFind(int x, int y, nodeStatus find);
bool recursiveSouthFind(int x, int y, nodeStatus find);

/*
 * resetVisited() - Resets the map that tells if a recursive function has been to a square or not
 */
void resetVisited();

void setX(int x);
void setY(int y);
int getX();
int getY();



#endif /* LABYRINTHMAP_H_ */
