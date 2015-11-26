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
static uint16_t angle = 0;
static uint16_t frontDistance = 0;
static uint16_t frontLeftDistance = 0;
static uint16_t backLeftDistance = 0;
static uint16_t frontRightDistance = 0;
static uint16_t backRightDistance = 0;
static uint16_t sensorBar[] = {0, 0, 0, 0, 0, 0, 0, 0};


void updateRegisters(uint8_t id, uint16_t data) {
	
	switch (id) {
		
		case 1:
			angle = data;
			break;
		
		case 2:
			break;
		
		case 3:
			frontLeftDistance = data;
			break;
		
		case 4:
			backLeftDistance = data;
			break;
		
		case 5:
			frontDistance = data;
			break;
		
		case 6:
			backRightDistance = data;
			break;
		
		case 7:
			frontRightDistance = data;
			break;
	}
}

uint16_t getCurrentAngle() {
	return angle;
}

uint16_t getFrontDistance() {
	return frontDistance;
}

uint16_t getFrontLeftDistance() {
	return frontLeftDistance;
}

uint16_t getFrontRightDistance() {
	return frontRightDistance;
}

uint16_t getBackLeftDistance() {
	return backLeftDistance;
}

uint16_t getBackRightDistance() {
	return backRightDistance;
}

