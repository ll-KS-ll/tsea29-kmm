/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Boot the sty-module											*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include "motorKernel.h"
#include "clawKernel.h"

void boot() {
	initMotor();
	initClaw();
}