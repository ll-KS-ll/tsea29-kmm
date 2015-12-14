/************************************************************************
 *																		*
 * Author: Güntech							                            *
 * Purpose: Process steering commands.									*
 * Language: C															*
 * File type: Source													*
 *																		*
/************************************************************************/

#include "steer_cmd.h"

int sty_rw = 0;
int com_rw = 0;

uint16_t sty_steer_cmd()
{
	cli();
	uint16_t data = 0;
	if (sty_rw == 0) {
		sty_rw = 1;
		i2c_read(STY_ADDRESS, 8); // Ask STYR for active steering command.
	} else if (styDataRead == 1) {
		data = sty_recv_datap.data;
		styDataRead = 0;
		sty_rw = 0;
	}
	sei();
	return data;
}

uint16_t com_steer_cmd()
{
	cli();
	uint16_t data = 0;
	if (com_rw == 0) {
		com_rw = 1;
		i2c_read(COM_ADDRESS, 9); // Ask COM for requested steering command.
	} else if (comDataRead == 1) {
		data = com_recv_datap.data;
		comDataRead = 0;
		com_rw = 0;
	}
	sei();
	return data;
}