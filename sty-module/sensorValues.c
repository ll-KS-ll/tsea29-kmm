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
static float angle = 0;
static unsigned char gyroValue = 0;
static unsigned char offset = 128;
static float gyroRate;
static uint16_t frontDistance = 0;
static uint16_t frontLeftDistance = 0;
static uint16_t backLeftDistance = 0;
static uint16_t frontRightDistance = 0;
static uint16_t backRightDistance = 0;
static uint16_t sensorBar[] = {0, 0, 0, 0, 0, 0, 0, 0};

/* Variables used for converting frontDistance to cm */
float mathf;
float voltsPerUnit = 0.0049;

unsigned int sideIrToCm(uint16_t data) {
	unsigned int math = 0;
	math = data;
	math = (6050 / math);
	
	if(math >= 2) {
		math -= 2; // fix linear error (-2)
	}
	if(math < 10) {
		math = 10; // min limit at 10cm
	}
	if(math > 80) {
		math = 80; // max limit at 80cm
	}
	return math;
}

// Convert values so they are between 70 - 0, larger number equals closer.
void updateRegisters(uint8_t id, uint16_t dataIn) {
	int data = dataIn;
	int temp = 0;
	switch (id) {
		
		case 1:
			gyroValue = data;
			gyroRate = (gyroValue - offset);
			angle += gyroRate / 100;
			break;
		
		case 2:
			break;
		
		case 3:
			frontLeftDistance = sideIrToCm(data);
			break;
		
		case 4:
			backLeftDistance = sideIrToCm(data);
			break;
		
		case 5:
			mathf = (float)data * voltsPerUnit;
			mathf = 60.495 * pow(mathf, -1.1904);
			if(mathf < 20) mathf = 20;
			if(mathf > 150) mathf = 150;
			frontDistance = (unsigned int) mathf;
			break;
		
		case 6:
			backRightDistance = sideIrToCm(data);
			break;
		
		case 7:
			frontRightDistance = sideIrToCm(data);
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

