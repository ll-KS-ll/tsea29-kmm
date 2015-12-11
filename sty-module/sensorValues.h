/************************************************************************
 *																		*
 * Author: Güntech														*
 * Purpose: Storage for the different sensor values						*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/

#include <stdint-gcc.h>
#include <stdbool.h>

#ifndef SENSORVALUES_H_
#define SENSORVALUES_H_


void updateRegisters(uint8_t id, uint16_t data);

bool getStart();
bool getAutonom();
<<<<<<< HEAD
uint16_t getCurrentAngle();
uint16_t getRegulateValue();
uint16_t getFrontDistance();
uint16_t getFrontLeftDistance();
uint16_t getFrontRightDistance();
uint16_t getBackLeftDistance();
uint16_t getBackRightDistance();
=======
int getCurrentAngle();
unsigned int *getSensorBar();
unsigned int *getCalibrationBar();
unsigned int getFrontDistance();
unsigned int getFrontLeftDistance();
unsigned int getFrontRightDistance();
unsigned int getBackLeftDistance();
unsigned int getBackRightDistance();
bool getSeesTape();
unsigned int getTapeReg();
bool getIsThereTape();

>>>>>>> refs/remotes/origin/autonom
uint8_t test;



#endif /* SENSORVALUES_H_ */