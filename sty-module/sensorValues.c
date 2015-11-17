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

// Init all values to zero
static int angle = 0;
static int frontDistance = 0;
static int frontLeftDistance = 0;
static int backLeftDistance = 0;
static int frontRightDistance = 0;
static int backRightDistance = 0;
static int sensorBar[] = {0, 0, 0, 0, 0, 0, 0, 0}


void updateRegisters(uint8_t dataId, uint16_t dataPacket) {
	switch dataId {
		case 5:
			angle = dataPacket;
			break;
	}
}
int getCurrentAngle() {
	return angle;
}

