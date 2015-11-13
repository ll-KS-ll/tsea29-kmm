/************************************************************************
 *																		*
 * Author: Lukas Lindqvist & Jacob Johansson                            *
 * Purpose: Basic driving functions for robot							*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/

#ifndef MOTORKERNEL_
#define MOTORKERNEL_

void initMotor();
void setMotorSpeed(int leftSpeed, int rightSpeed);
void setForward();
void setReverse();
void setRotateLeft();
void setRotateRight();
void stop();

 
#endif