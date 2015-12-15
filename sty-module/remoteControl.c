/************************************************************************
 *																		*
 * Author: G�ntech - Victor Karlsson Sehlin/Viktor W�llstedt            *
 * Purpose: Execute command from the PC-Software.						*
 * Language: C															*
 * File type: Source													*
 * Version: 1.0															*
 *																		*
/************************************************************************/

#include "remoteControl.h"
#include "motorKernel.h"
#include "sensorValues.h"

void executeCurrentCommand()
{
	switch (getActiveCmd())
	{
		case 0: // NOP
			break;
		case 1: // Forward
			driveForward(30,30);
			break;
		case 2: // Stop
			stop();
			break;
		case 3: // Reverse
			driveReverse(30,30);
			break;
		case 4: // Turn Right
			driveForward(40,20);
			break;
		case 5: // Turn Left
			driveForward(20,40);
			break;
		case 6: // Rotate Right
			driveRotateRight(30,30);
			break;
		case 7: // Rotate Left
			driveRotateLeft(30,30);
			break;
		case 8: // Calibrate --- TODO
			break;
	}	
}
