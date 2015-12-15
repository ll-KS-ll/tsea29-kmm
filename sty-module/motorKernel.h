/************************************************************************
 *																		*
 * Author: Güntech - Lukas Lindqvist/Jacob Johansson                    *
 * Purpose: Kernel for the motors										*
 * Language: C															*
 * File type: Header													*
 * Version: 1.0															*
 *																		*
/************************************************************************/


#ifndef MOTORKERNEL_H_
#define MOTORKERNEL_H_

void initMotor();
void adjust(int u);
void driveForward(int leftSpeed, int rightSpeed);
void driveReverse(int leftSpeed, int rightSpeed);
void driveRotateLeft(int leftSpeed, int rightSpeed);
void driveRotateRight(int leftSpeed, int rightSpeed);
void stop();
void turnLeft(int turns);
void turnRight(int turns);
 
#endif /* MOTORKERNEL_H_ */