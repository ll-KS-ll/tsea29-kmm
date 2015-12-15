/************************************************************************
 *																		*
 * Author: Güntech - Lukas Lindqvist		                            *
 * Purpose: Boot the sty-module											*
 * Language: C															*
 * File type: Source													*
 * Version: 1.0															*
 *																		*
/************************************************************************/

#include "boot.h"
#include "motorKernel.h"
#include "clawKernel.h"
#include "gyroController.h"
#include "autonomous.h"

/*
 * boot() - boots up all the systems in the sty-module
 */
void boot() {
	initMotor();
	initClaw();
	initGyro();
	initOneSquareTimer();
}