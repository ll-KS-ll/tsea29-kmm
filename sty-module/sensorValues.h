/************************************************************************
 *																		*
 * Author: G�ntech														*
 * Purpose: Storage for the different sensor values						*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/


#ifndef SENSORVALUES_H_
#define SENSORVALUES_H_

void updateRegisters(uint8_t dataId, uint16_t dataPacket);
int getCurrentAngle();

#endif /* SENSORVALUES_H_ */