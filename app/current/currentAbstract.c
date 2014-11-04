#include "current/currentAbstract.h"

TYPE_CURRENT getCurrentById(UINT8 id)
{
	//TODO
	return 0;
}

// simulator current
CURRENT_TYPE set_simulator_current( CURRENT_TYPE charge_current, CURRENT_TYPE discharge_current )
{
	//TODO
	return 0;
}


void printf_currents(void)
{
	UINT8 i = 0;
	for (i = 0; i < TOTAL_CURRENTS; i++)
	{
		mprintf("current[%u]:%uA ", i, getCurrentById(i));
	}
	mprintf("\r\n");
}
