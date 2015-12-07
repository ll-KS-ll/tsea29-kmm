/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Source													*
 *																		*
 ************************************************************************/

#include "labyrinthMap.h"
#include <iostream>

//insert the first node to the map
void initMap(){
//head
    node n;
//body
    n.neighbours[north] = CLOSED;
    n.neighbours[east] = CLOSED;
    n.neighbours[south] = CLOSED;
    n.neighbours[west] = CLOSED;
    n.status = START;
    labyrinth[xpos][ypos] = n;
    updateNewNeighbours();
    identifyNeighbours();	//get all neighbours
}


void addNode(){
//head
    node currentNode;
    currentNode = labyrinth[xpos][ypos];
//body
    if(currentNode.status == START){
    }else if(currentNode.status == UNEXPLORED){
        currentNode.status = EXPLORED;
        labyrinth[xpos][ypos] = currentNode;
        updateNewNeighbours();
        identifyNeighbours();
    }
    getCurrentNodeStatus(xpos, ypos);
}

//adds new nodes if the node is UNEXPLORED
void updateNewNeighbours(){
    node n;
    n.status = UNEXPLORED;
    n.neighbours[north] = CLOSED;
    n.neighbours[east] = CLOSED;
    n.neighbours[south] = CLOSED;
    n.neighbours[west] = CLOSED;

    //insert UNEXPLORED nodes around the current-node
    if(labyrinth[xpos][ypos-1].status == UNKNOWN){  //north
        labyrinth[xpos][ypos-1] = n;
    }if(labyrinth[xpos+1][ypos].status == UNKNOWN){	//east
        labyrinth[xpos+1][ypos] = n;
    }if(labyrinth[xpos][ypos+1].status == UNKNOWN){	//south
        labyrinth[xpos][ypos+1] = n;
    }if(labyrinth[xpos-1][ypos].status == UNKNOWN){	//west
        labyrinth[xpos-1][ypos] = n;
    }
}

//find all possible neighbours for the current node
void identifyNeighbours(){
//head
    uint16_t frontDistance;
    uint16_t frontLeftDistance;
    uint16_t backLeftDisantnce;
    uint16_t frontRightDistance;
    uint16_t backRightDistance;
    int front;
    int left;
    int right;
    int back;
    node n;
    n = labyrinth[xpos][ypos];
//body
    switch(currentDirection){
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
        case DUNNO:
            front = north;
            right = east;
            back = south;
            left = west;
            break;
    }

    //determine north and change variables accordingly
    frontDistance = getFrontDistance();
    frontLeftDistance = getFrontLeftDistance();
    backLeftDisantnce = getBackLeftDistance();
    frontRightDistance = getFrontRightDistance();
    backRightDistance = getBackRightDistance();

    //check the distance for each direction
    if (frontDistance >= 50){
        labyrinth[xpos][ypos].neighbours[front] = OPEN;
    } if (frontRightDistance >= 40 || backRightDistance >= 40){
        labyrinth[xpos][ypos].neighbours[right] = OPEN;
    }   labyrinth[xpos][ypos].neighbours[back] = OPEN;
    if (frontLeftDistance >= 40 || backLeftDisantnce >= 40){
        labyrinth[xpos][ypos].neighbours[left] = OPEN;
    }

    return;
}

//traverse the map via the nodes to a target node
path *nextTarget(){
//head
    path *root = (path *) malloc(sizeof(*root));
    path *c = (path *) malloc(sizeof(*c));
    found_way = false;
    root->p = DUNNO;
    resetVisited();

//body
    //addNode();
    root->next = traverseLabyrint(xpos, ypos, DUNNO);
    root = root->next;
    c = root;
    updatePosition(c);      //Unsure where to put updatePosition(); and addNode();
    found_way = false;

    return root;
}

//help function to traverse the labyrinth
path *traverseLabyrint(int x, int y, dir last){
//head
    node currentNode = labyrinth[x][y];
    struct path *way =  (path *) malloc(sizeof(*way));

//body
    way->p = DUNNO;
    way->next = NULL;
    if(visited[x][y]){
		free(way);
		return way;
    }
    visited[x][y] = true;
    if (currentNode.status == UNEXPLORED){	//zero case
        found_way = true;
        return way;
    }else {
        if (currentNode.neighbours[north] == OPEN && !found_way && last != SOUTH && y > 1){
            way->p = NORTH;
            currentDirection = NORTH;
            way->next = traverseLabyrint(x,y-1,NORTH);
        } if (currentNode.neighbours[east] == OPEN && !found_way && last != WEST && x < 31){
            way->p = EAST;
            currentDirection = EAST;
            way->next = traverseLabyrint(x+1,y,EAST);
        } if (currentNode.neighbours[south] == OPEN && !found_way && last != NORTH && y < 31){
            way->p = SOUTH;
            currentDirection = SOUTH;
            way->next = traverseLabyrint(x,y+1,SOUTH);
        } if (currentNode.neighbours[west] == OPEN && !found_way && last != EAST && x > 1){
            way->p = WEST;
            currentDirection = WEST;
            way->next = traverseLabyrint(x-1,y,WEST);
        }
    }
    return way;
}

//traverse to the start
path getOut(){
    path *escapePath = NULL;


    return *escapePath;
}

//make a good traverse function
void traverseGetOut(){

}

//read given path to determine where we are
void updatePosition(path *p){
    while(p){
        switch(p->p){
            case NORTH:
                ypos-=1;
                break;
            case EAST:
                xpos+=1;
                break;
            case SOUTH:
                ypos+=1;
                break;
            case WEST:
                xpos-=1;
                break;
            case DUNNO:
                break;
        }
        p = p->next;
    }
}
/*
 *
 */
void selectNewPosition(int x, int y){
    xpos = x;
    ypos = y;
    getCurrentNodeStatus(xpos,ypos);

}

void resetVisited(){
    for(int x = 0; x < x_size; x++){
        for(int y = 0; y < y_size; y++){
            if(visited[x][y]){
                visited[x][y] = false;
            }
        }
    }
}
