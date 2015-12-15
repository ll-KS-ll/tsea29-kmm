/********************************************************************************
 *																				*
 * Author: Güntech - Lukas Lindqvist											*
 * Purpose: Storage for the different variables 								*
 * Language: C																	*
 * File type: Header															*
 * Version: 1.0																	*
 *																				*
/********************************************************************************/

#include <stdint-gcc.h>

#ifndef VARIABLES_H_
#define VARIABLES_H_

/* Motor variables */
#define TOTAL_POWER 1100
#define MAX_POWER 900
#define MAX_SPEED 65
#define MIN_SPEED 35
#define TURN_SPEED 40
#define ZERO_SPEED 0

/* Distance variables */
#define FRONT_CLOSED 32
#define FRONT_SPOTTED 85
#define SIDE_OPEN 45
#define ONE_SQUARE_DISTANCE 60
#define ONE_SQUARE 140

/* Turn variables */
#define LEFT_TURN 32
#define RIGHT_TURN 35

/* PD-Regulation variables */
#define KP 3
#define KD 12

/* Gyro port variable */
#define GYRO 1




#endif /* VARIABLES_H_ */