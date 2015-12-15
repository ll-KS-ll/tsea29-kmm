/************************************************************************
 *																		*
 * Author: Güntech - Lukas Lindqvist/Jacob Johansson					*
 * Purpose: Kernel for the claw										    *
 * Language: C															*
 * File type: Header													*
 * Version: 1.0															*
 *																		*
/************************************************************************/



#ifndef CLAWKERNEL_H_
#define CLAWKERNEL_H_

/* 
 * initClaw() - initializes the Fast PWM signal for the claw.
 */
void initClaw();

void openClaw();
void closeClaw();
void lowerClaw();
void raiseClaw();

#endif /* CLAWKERNEL_H_ */