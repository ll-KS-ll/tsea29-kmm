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
#include "bluetooth.h"

#ifndef SENSORVALUES_H_
#define SENSORVALUES_H_

#define START 0
#define AUTONOM 1
#define FRONT_LEFT 3
#define BACK_LEFT 4
#define FRONT 5
#define BACK_RIGHT 6
#define FRONT_RIGHT 7
#define STEER_CMD 8
#define LINESENSOR_0 10
#define LINESENSOR_1 11
#define LINESENSOR_2 12
#define LINESENSOR_3 13
#define LINESENSOR_4 14
#define LINESENSOR_5 15
#define LINESENSOR_6 16
#define TAPE_FOUND 18

void updateRegisters(uint8_t id, uint16_t data);
bool getStart();
bool getAutonom();
uint16_t *getSensorBar();
uint16_t getFrontDistance();
uint16_t getFrontLeftDistance();
uint16_t getFrontRightDistance();
uint16_t getBackLeftDistance();
uint16_t getBackRightDistance();

#endif /* SENSORVALUES_H_ */