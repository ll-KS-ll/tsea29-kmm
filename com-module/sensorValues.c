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
static bool start = false;
static bool autonom = false;
static unsigned int frontDistance = 0;
static unsigned int frontLeftDistance = 0;
static unsigned int backLeftDistance = 0;
static unsigned int frontRightDistance = 0;
static unsigned int backRightDistance = 0;
static uint16_t sensorBar[] = {0, 0, 0, 0, 0, 0, 0};


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
		/*ful-hack*/
		case 10:
			sensorBar[0] = data;
			break;
			
		case 11:
			sensorBar[1] = data;
			break;
			
		case 12:
			sensorBar[2] = data;
			break;
			
		case 13:
			sensorBar[3] = data;
			break;
			
		case 14:
			sensorBar[4] = data;
			break;
			
		case 15:
			sensorBar[5] = data;
			break;
			
		case 16:
			sensorBar[6] = data;
			break;
		case 17:
			gyroValue = data;
			gyroRate = (gyroValue - offset);
			angle += gyroRate / 100;
			break;
	}
}
bool getStart(){
	return start;
}

bool getAutonom(){
	return autonom;
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

