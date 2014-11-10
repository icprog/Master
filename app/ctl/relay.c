#include "ctl/relay.h"
#include "voltage/voltage.h"
int OpenRelayNegative(void)
{
	//TODO
	return 0;
}
int OpenRelayPrecharge(void)
{
	//TODO
	return 0;
}

int OpenRelayPositive(void)
{
	//TODO
	return 0;

}
int OpenRelayPrechargeByTimerS(UINT32 times)
{
	//TODO
	return 0;

}

int ShutdownRelayNegative(void)
{
	//TODO
	return 0;

}
int ShutdownRelayPositive(void)
{
	//TODO
	return 0;
}

/*
 * if VCU check self OK, then do this
 * */
int PowerOnHighVoltage(void)
{
	UINT32 v_load = GetLoadVoltage();
	OpenRelayNegative();
	OpenRelayPrecharge();
	do
	{
		v_load = GetLoadVoltage();
	} while (v_load < (BMS_GetVoltageValueBatteryTotal() * 80 / 100));
	OpenRelayPositive();
	OpenRelayPrechargeByTimerS(5);
	return 0;
}

/*
 * if get VCU command "relay command = 1", then do this
 * or err lever 1 happen, and 5S past.
 * */
int PowerOffHighVoltage(void)
{
	ShutdownRelayNegative();
	ShutdownRelayPositive();
	return 0;
}
