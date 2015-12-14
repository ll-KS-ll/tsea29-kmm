/************************************************************************
 *																		*
 * Author: Güntech														*
<<<<<<< HEAD
 * Purpose: Storage for the different sensor values						*
=======
 * Purpose: Storage for the different sensor values.					*
>>>>>>> GUI
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/

#include <stdbool.h>
#include "sensorValues.h"

// Init all values to zero
static bool start = false;
static bool autonom = false;
static unsigned int frontDistance = 0;
static unsigned int frontLeftDistance = 0;
static unsigned int backLeftDistance = 0;
static unsigned int frontRightDistance = 0;
static unsigned int backRightDistance = 0;
static uint16_t sensorBar[] = {0, 0, 0, 0, 0, 0, 0};
static unsigned int recvSteeringCmd = 0;
	

// Convert values so they are between 70 - 0, larger number equals closer.
void updateRegisters(uint8_t id, uint16_t dataIn) {
	unsigned int data = dataIn;
	bool data_changed = false;
	
	switch (id) {		
		case START:
			if(data)
			{
				start = !start;
				data_changed = true;
			}
			break;
		case AUTONOM:
			if(data)
			 {
				autonom = !autonom;
				data_changed = true;
			 }
			break;
		case FRONT_LEFT:
			if( data != frontRightDistance ) {
				frontLeftDistance = data;
				data_changed = true;				
			}
			break;
		case BACK_LEFT:
			if( data != backLeftDistance ) {
				backLeftDistance = data;
				data_changed = true;
			}
			break;
		case FRONT:
			if( data != frontDistance ) {
				frontDistance = data;
				data_changed = true;
			}			
			break;
		case BACK_RIGHT:
			if( data != backRightDistance ) {
				backRightDistance = data;
				data_changed = true;
			}
			break;
		case FRONT_RIGHT:
			if( data != frontRightDistance ) {
				frontRightDistance = data;
				data_changed = true;
			}
			break;
		case STEER_CMD:
			if (recvSteeringCmd != data)
			{
				recvSteeringCmd = data;
				data_changed = true;
			}
			break;
		/*ful-hack*/
		case LINESENSOR_0:
			sensorBar[0] = data;
			break;
		case LINESENSOR_1:
			sensorBar[1] = data;
			break;
		case LINESENSOR_2:
			sensorBar[2] = data;
			break;
		case LINESENSOR_3:
			sensorBar[3] = data;
			break;
		case LINESENSOR_4:
			sensorBar[4] = data;
			break;
		case LINESENSOR_5:
			sensorBar[5] = data;
			break;
		case LINESENSOR_6:
			sensorBar[6] = data;
			break;
	}
	
	if (data_changed)
	{
		bt_transmit(id);
		bt_transmit(data);
	}
}
