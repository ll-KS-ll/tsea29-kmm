/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Boot the sty-module											*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include "boot.h"
#include "motorKernel.h"
#include "clawKernel.h"
#include "gyroController.h"

void boot() {
	initMotor();
	//initClaw();
	initGyro();
}