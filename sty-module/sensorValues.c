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
static uint16_t frontDistance = 0;
static uint16_t frontLeftDistance = 0;
static uint16_t backLeftDistance = 0;
static uint16_t frontRightDistance = 0;
static uint16_t backRightDistance = 0;
static uint16_t sensorBar[] = {0, 0, 0, 0, 0, 0, 0, 0};


void updateRegisters(uint8_t id, uint16_t data) {

	
	angle = data;
	//switch (dataId) {
		//case 5:
			//break;
	//}
}

int getCurrentAngle() {
	return angle;
}

