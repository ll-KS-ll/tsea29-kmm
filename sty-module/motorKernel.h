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
void setMotorSpeed(int leftSpeed, int rightSpeed);
void setForward();
void setReverse();
void setRotateLeft();
void setRotateRight();
void stop();

 
#endif /* MOTORKERNEL_H_ */