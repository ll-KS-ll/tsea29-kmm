/************************************************************************
 *																		*
 * Author: Güntech														*
 * Purpose: Storage for the different sensor values						*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/

#include "sensorValues.h"

// Init all values to zero
static float angle = 0;
static unsigned char gyroValue = 0;
static unsigned char offset = 128;
static float gyroRate;
static bool start = false;
static bool autonom = false;
static unsigned int frontDistance = 0;
static unsigned int frontLeftDistance = 0;
static unsigned int backLeftDistance = 0;
static unsigned int frontRightDistance = 0;
static unsigned int backRightDistance = 0;
static bool seesTape = false;
static unsigned int tapeReg;
static unsigned int activeSteeringCmd = 0;

// Convert values so they are between 70 - 0, larger number equals closer.
void updateRegisters(uint8_t id, uint16_t dataIn) {
	int data = dataIn;
	int temp = 0;
	
	switch (id) {
		case 0:
			if(data)
			{
				 start = !start;
			}
			break;
		case 1:
			if(data)
			 {
				 autonom = !autonom;
			 }
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
		
		case 9:
			activeSteeringCmd = data;
			break;

		case 17:
			gyroValue = data;
			gyroRate = (gyroValue - offset);
			angle += gyroRate / 100;
			break;
		case 18:
			if(data)
			{
				seesTape = true;
			} else {
				seesTape = false;
			}
			break;
		case 19:
			tapeReg = data;
			break;
		
	}
}

unsigned int getTapeReg() {
	return tapeReg;
}

bool getSeesTape() {
	return seesTape;
}

bool getStart(){
	return start;
}

bool getAutonom(){
	return autonom;
}

int getCurrentAngle() {
	return (int) angle;
}

unsigned int getFrontDistance() {
	return frontDistance;
}

unsigned int getFrontLeftDistance() {
	return frontLeftDistance;
}

unsigned int getFrontRightDistance() {
	return frontRightDistance;
}

unsigned int getBackLeftDistance() {
	return backLeftDistance;
}

unsigned int getBackRightDistance() {
	return backRightDistance;
}

unsigned int getActiveCmd() {
	return activeSteeringCmd;
}
