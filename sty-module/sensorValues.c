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
<<<<<<< HEAD
volatile static unsigned int regulate_value = 0;

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
=======
static unsigned int sensorBar[] = {0, 0, 0, 0, 0, 0, 0};
static unsigned int sensorBarCalibration[] = {0, 0, 0, 0, 0, 0, 0};
static unsigned int seesTape = 0;
static unsigned int tapeReg = 5;

>>>>>>> refs/remotes/origin/autonom

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
			
		case 2:
			regulate_value = data;
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

		case 17:
			gyroValue = data;
			gyroRate = (gyroValue - offset);
			angle += gyroRate / 100;
			break;
		case 18:
			if(data)
			{
				seesTape = !seesTape;
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

<<<<<<< HEAD
uint16_t getRegulateValue(){
	return regulate_value;
}

uint16_t getCurrentAngle() {
	return angle;
=======
bool getIsThereTape() {
	return false;
}

int getCurrentAngle() {
	return (int) angle;
}

unsigned int *getSensorBar() {
	return sensorBar;
>>>>>>> refs/remotes/origin/autonom
}

unsigned int *getCalibrationBar() {
	return sensorBarCalibration;
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

