/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Kernel for the motors										*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/


#ifndef MOTORKERNEL_H_
#define MOTORKERNEL_H_

void initMotor();
void goForwardWithCurrentSpeed();
void adjust(int u);
void driveForward(int leftSpeed, int rightSpeed);
void driveReverse(int leftSpeed, int rightSpeed);
void driveRotateLeft(int leftSpeed, int rightSpeed);
void driveRotateRight(int leftSpeed, int rightSpeed);
void stop();
void turnLeft(int turns);
void turnRight(int turns);
 
#endif /* MOTORKERNEL_H_ */