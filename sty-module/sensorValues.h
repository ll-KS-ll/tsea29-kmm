/********************************************************************************
 *																				*
 * Author: Güntech - Lukas Lindqvist/Victor Karlsson Sehlin/Viktor Wällstedt	*
 * Purpose: Storage for the different sensor values								*
 * Language: C																	*
 * File type: Header															*
 * Version: 1.0																	*
 *																				*
/********************************************************************************/

#include <stdint-gcc.h>
#include <stdbool.h>

#ifndef SENSORVALUES_H_
#define SENSORVALUES_H_

/*
 * updateRegisters(id, data) - Updates the storage with the data it received from the I2C-bus
 */
void updateRegisters(uint8_t id, uint16_t data);

/*
 * Getters - Returns the value in storage
 */
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