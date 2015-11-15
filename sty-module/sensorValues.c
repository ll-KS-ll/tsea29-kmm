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

static uint16_t angle;
static uint16_t frontDistance;
static uint16_t frontLeftDistance;
static uint16_t backLeftDistance;
static uint16_t frontRightDistance;
static uint16_t backRightDistance;
static uint16_t sensorBar[];

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
	return backRightDistance
}

void setCurrentAngle(uint16_t newAngle) {
	angle = newAngle;
}

void setFrontDistance(uint16_t newDistance) {
	frontDistance = newDistance;
}

void setFrontLeftDistance(uint16_t newDistance) {
	frontLeftDistance = newDistance;
}

void setFrontRightDistance(uint16_t newDistance) {
	frontRightDistance = newDistance;
}

void setBackLeftDistance(uint16_t newDistance) {
	backLeftDistance = newDistance;
}

void setBackRightDistance(uint16_t newDistance) {
	backRightDistance = newDistance;
}

