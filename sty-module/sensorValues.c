/************************************************************************
 *																		*
 * Author: G�ntech														*
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
static uint16_t sensorBar[] = {0, 0, 0, 0, 0, 0, 0, 0};

// Convert values so they are between 70 - 0, larger number equals closer.
void updateRegisters(uint8_t id, uint16_t dataIn) {
	int data = dataIn;
	int temp = 0;
	switch (id) {
		case 1:
		break;
		case 2:
		break;
		case 3:
		if(data > 700) data = 700;
		temp = (700 - data) / 10;
		frontLeftDistance = temp;
		break;
		case 4:
		if(data > 700) data = 700;
		temp = (700 - data) / 10;
		backLeftDistance = temp;
		break;
		case 5:
		if(data > 700) data = 700;
		temp = (700 - data) / 10;
		frontDistance = temp;
		break;
		case 6:
		if(data > 700) data = 700;
		temp = (700 - data) / 10;
		backRightDistance = temp;
		break;
		case 7:
		if(data > 700) data = 700;
		temp = (700 - data) / 10;
		frontRightDistance = temp;
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

