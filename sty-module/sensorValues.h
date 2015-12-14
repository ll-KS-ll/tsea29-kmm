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
int getCurrentAngle();
unsigned int getFrontDistance();
unsigned int getFrontLeftDistance();
unsigned int getFrontRightDistance();
unsigned int getBackLeftDistance();
unsigned int getBackRightDistance();
bool getSeesTape();
unsigned int getTapeReg();
unsigned int getActiveCmd();

#endif /* SENSORVALUES_H_ */