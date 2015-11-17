/************************************************************************
 *																		*
 * Author: Güntech														*
 * Purpose: Storage for the different sensor values						*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/


#ifndef SENSORVALUES_H_
#define SENSORVALUES_H_

<<<<<<< HEAD
void updateRegisters(uint8_t dataId, uint16_t dataPacket);
int getCurrentAngle();
=======
uint16_t getCurrentAngle();
uint16_t getFrontDistance();
uint16_t getFrontLeftDistance();
uint16_t getFrontRightDistance();
uint16_t getBackLeftDistance();
uint16_t getBackRightDistance();

void setCurrentAngle(uint16_t newAngle);
void setFrontDistance(uint16_t newDistance);
void setFrontLeftDistance(uint16_t newDistance);
void setFrontRightDistance(uint16_t newDistance);
void setBackLeftDistance(uint16_t newDistance);
void setBackRightDistance(uint16_t newDistance);
>>>>>>> origin/sty-module

#endif /* SENSORVALUES_H_ */