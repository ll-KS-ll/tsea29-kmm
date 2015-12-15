/************************************************************************
 *																		*
 * Author: Güntech - Lukas Lindqvist/Sara Svensson/Håkan Gudmundsson	*
 * Purpose: To read the gyro										    *
 * Language: C															*
 * File type: Header													*
 * Version: 1.0															*
 *																		*
/************************************************************************/

#include <stdint-gcc.h>

#ifndef GYROCONTROLLER_H_
#define GYROCONTROLLER_H_

/* 
 * initGyro() - Initializes A/D-converter and interrupts so that we can read a value every 10ms
 */
void initGyro();

/*
 * adcReadGyro(uint8_t ch) - Reads data from the gyro
 */
uint8_t adcReadGyro(uint8_t ch);

void stopGyroInterrupts();
void startGyroInterrupts();



#endif /* GYROCONTROLLER_H_ */