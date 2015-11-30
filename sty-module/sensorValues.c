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
static uint8_t chanel_out = 8;
static uint16_t frontDistance = 0;
static uint16_t frontLeftDistance = 0;
static uint16_t backLeftDistance = 0;
static uint16_t frontRightDistance = 0;
static uint16_t backRightDistance = 0;
static uint16_t sensorBar[] = {0, 0, 0, 0, 0, 0, 0};

// Convert values so they are between 70 - 0, larger number equals closer.
void updateRegisters(uint8_t id, uint16_t dataIn) {
	int data = dataIn;
	int temp = 0;
	switch (id) {
		
		case 1:
			angle = data;
			break;
		
		case 2:
			if(chanel_out == 0) {chanel_out = 8;}
			chanel_out--;
			sensorBar[chanel_out] = data;
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

uint16_t *getSensorBar() {
	return sensorBar;
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

