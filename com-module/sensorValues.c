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
static unsigned int recvSteeringCmd = 0;
	
/* Variables used for converting frontDistance to cm */
float mathf;
float voltsPerUnit = 0.0049;

unsigned int sideIrToCm(uint16_t data) {
	mathf = data;
	mathf = pow((3027.4/mathf), 1.2134);
	if(mathf > 80) mathf = 80;
	if(mathf < 10) mathf = 10;
	return (unsigned int)mathf;
}

// Convert values so they are between 70 - 0, larger number equals closer.
void updateRegisters(uint8_t id, uint16_t dataIn) {
	unsigned int data = dataIn;
	
	switch (id) {		
		case 0:
			if(data)
			{
				start = !start;
				flag_array |= 1<<0;
			}
			break;
		case 1:
			if(data)
			 {
				autonom = !autonom;
				flag_array |= 1<<1;
			 }
			break;
		case 3:
			frontLeftDistance = data = sideIrToCm(data);
			flag_array |= 1<<2;
			break;
		case 4:
			backLeftDistance = data = sideIrToCm(data);
			flag_array |= 1<<3;
			break;
		case 5:
			mathf = (float)data * voltsPerUnit;
			mathf = 60.495 * pow(mathf, -1.1904);
			if(mathf < 20) mathf = 20;
			if(mathf > 150) mathf = 150;
			frontDistance = data = (unsigned int) mathf;
			flag_array |= 1<<4;
			break;
		case 6:
			backRightDistance = data = sideIrToCm(data);
			flag_array |= 1<<5;
			break;
		case 7:
			frontRightDistance = data = sideIrToCm(data);
			flag_array |= 1<<6;
			break;
		case 8:
			recvSteeringCmd = data;
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
	
	bt_transmit(id);
	bt_transmit(data);
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

