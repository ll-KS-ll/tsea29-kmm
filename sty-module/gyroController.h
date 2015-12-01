/*
 * gyroController.h
 *
 * Created: 2015-11-30 18:46:08
 *  Author: Lukas
 */ 

#include <stdint-gcc.h>

#ifndef GYROCONTROLLER_H_
#define GYROCONTROLLER_H_

void initGyro();
uint8_t adcReadGyro(uint8_t ch);
void stopGyroInterrupts();
void startGyroInterrupts();



#endif /* GYROCONTROLLER_H_ */