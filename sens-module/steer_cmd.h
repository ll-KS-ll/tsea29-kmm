/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Process steering commands.									*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/


#ifndef STEER_CMD_H_
#define STEER_CMD_H_

#include <avr/interrupt.h>
#include "i2c_master.h"

uint16_t com_steer_cmd();
uint16_t sty_steer_cmd();
 
#endif /* STEER_CMD_H_ */