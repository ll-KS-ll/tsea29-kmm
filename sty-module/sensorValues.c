/************************************************************************
 *																		*
 * Author: Güntech														*
 * Purpose: Storage for the different sensor values						*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/

#include <stdbool.h>
#include "sensorValues.h"

static int angle;
static int frontDistance;
static int frontLeftDistance;
static int backLeftDistance;
static int frontRightDistance;
static int backRightDistance;
static int sensorBar[];

int getCurrentAngle() {
	return angle;
}

