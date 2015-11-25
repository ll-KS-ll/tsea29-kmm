/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Kernel for the motors										*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/

#include "variables.h"

#ifndef MOTORKERNEL_H_
#define MOTORKERNEL_H_

void initMotor();
void adjustLeft();
void adjustRight();
void driveForward(int leftSpeed, int rightSpeed);
void driveReverse(int leftSpeed, int rightSpeed);
void driveRotateLeft(int leftSpeed, int rightSpeed);
void driveRotateRight(int leftSpeed, int rightSpeed);
void stop();



 
#endif /* MOTORKERNEL_H_ */