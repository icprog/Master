/*
 * the next is the system state machine module
 * */
#include "public/systemState.h"
#include "public/types.h"
#include "Gpio/gpioFuncs.h"
#include "ctl/charge.h"

struct system_module
{
	UINT8 systemModule;
};

static struct system_module gSystemMode = { SYSTEM_MODE_SLEEP };

UINT8 GetSystemMode(void)
{
	return gSystemMode.systemModule;
}

int SetSystemMode(UINT8 mode)
{
	gSystemMode.systemModule = mode;
	return 0;
}

int SystemStateMachine(void)
{
	UINT8 mode = GetSystemMode();
	switch(mode)
	{
	case SYSTEM_MODE_SLEEP:
		if( (mode = SleepModeDetection())!= 0 )
		{
			if(mode == SYSTEM_ACTIVE_MODE_KEY_TO_ACC)
			{
				SetSystemMode(SYSTEM_MODE_DISCHARGE);
			}
			else if(mode == SYSTEM_ACTIVE_MODE_ACCHARGER_IN)
			{
				SetSystemMode(SYSTEM_MODE_AC_CHARGE);
			}
			else if(mode == SYSTEM_ACTIVE_MODE_DCCHARGER_IN)
			{
				SetSystemMode(SYSTEM_MODE_DC_CHARGE);
			}
		}
		break;
		
	case SYSTEM_MODE_DC_CHARGE:
		if( DC_CHARGER_DISCONNECT() )
		{
			SetSystemMode(SYSTEM_MODE_SLEEP);
		}
		break;
		
	case SYSTEM_MODE_AC_CHARGE:
		if( AC_CHARGER_DISCONNECT() )
		{
			SetSystemMode(SYSTEM_MODE_SLEEP);
		}
		break;
		
	case SYSTEM_MODE_DISCHARGE:
		if(KEY_TO_ON())
		{
			// if bms self check passed, set BMS_SelfCheckPassed bit to 1,and 
			// then send to VCU by can 
			
			// if VCU check self OK , Power on high voltage.
			
			// waitForVCUCheckSelfOK();
			// PowerOnHighVoltage();
		}
		else if(KEY_TO_ACC()) // ACC activate
		{
			// can enable
		}
		else 
		{
			SetSystemMode(SYSTEM_MODE_SLEEP);
		}
		break;
		
	default:
		break;
	}
	
	return 0;
}
