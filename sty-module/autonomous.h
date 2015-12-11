/************************************************************************
 *																		*
 * Author: GünTECH							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/


#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_


typedef enum {north, south, west, east} direction;

void exploreLabyrinth();
void regulateRobot();

#endif /* AUTONOMOUS_H_ */