/************************************************************************
 *																		*
 * Author: GünTECH - Lukas Lindqvist		                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Header													*
 * Version: 1.0															*
 *																		*
/************************************************************************/


#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_

/*
 * solveLabyrinth - Fully explores labyrinth and thereafter goes to the location
 * where the item to be picked up is located and then takes it to start.
 */
void solveLabyrinth();

/*
 * initOneSquareTimer - Initializes the timer used to drive only one square at the time
 */ 
void initOneSquareTimer();


#endif /* AUTONOMOUS_H_ */