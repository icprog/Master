#include "current/currentAbstract.h"
//#include "current/current.h"
#include "public/systemState.h"
struct c_current
{
	TYPE_CURRENT current;
	UINT8 AdPort;
};

struct CCurrent
{
	TYPE_CURRENT mCurrentCurrent;
	TYPE_CURRENT mSimulatorCurrent;
	UINT8 mCurrentStatus;
	UINT32 timer_OC;
};

struct CCurrentThresholdValue
{
	CURRENT_TYPE mThreatholdOCLever1;
	CURRENT_TYPE mThreatholdOCLever2;
//	CURRENT_TYPE mThretholdCurrentOC;
//	UINT32 mThretholdTimerOCFilter;
	UINT8 adChannel;
};

struct CCurrentsMember
{
	struct CCurrent mCurrents[TOTAL_CURRENTS];
};

struct CCurrentsThresholdConstValue
{
	struct CCurrentThresholdValue mCurrentThreshold[TOTAL_CURRENTS];
};


/*********************************************************************************
 * private gloable value
 * 
 * ******************************************************************************/
static const struct CCurrentsThresholdConstValue gThresholdValue;  //TODO initial
static struct CCurrentsMember gCurrent;


/*
 * functions 
 * */
static int setCurrentValueById(int id, TYPE_CURRENT current)
{
	assert(id < TOTAL_CURRENTS);
	gCurrent.mCurrents[id].mCurrentCurrent = current;
	return 0;
}

static void updateCurrentStatusValue(void)
{
	UINT8 i = 0;
	struct CCurrent *this;
	const struct CCurrentThresholdValue *pthresholdValue;
	for (i = 0; i < TOTAL_CURRENTS; i++)
	{
		this = &gCurrent.mCurrents[i];
		pthresholdValue = &gThresholdValue.mCurrentThreshold[i];
		if (this->mCurrentCurrent >= pthresholdValue->mThreatholdOCLever1)
		{
			this->mCurrentStatus = SYSTEM_FAULT_LEVER1;
//			if (this->timer_OC > pthresholdValue->mThretholdTimerOCFilter)
//				this->mCurrentStatus = 1;
//			else
//				this->timer_OC++;
		}
		else if (this->mCurrentCurrent >= pthresholdValue->mThreatholdOCLever2)
		{
			this->mCurrentStatus = SYSTEM_FAULT_LEVER2;
//			this->timer_OC = 0;
		}
	}

}


static TYPE_CURRENT getCurrentById(int id)
{
	assert(id<TOTAL_CURRENTS);
	return gCurrent.mCurrents[id].mCurrentCurrent;
}

static int getCurrentStatusById( int id)
{
	assert(id<TOTAL_CURRENTS);
	return gCurrent.mCurrents[id].mCurrentStatus;
}

static CURRENT_TYPE getCurrentBymV(UINT16 mv, float k)
{
	CURRENT_TYPE current = 0;
	double v = mv * 0.001;
	current = (CURRENT_TYPE) (v / k);
	return (current);
}

static CURRENT_TYPE get_current_discharge(void)
{
	return (getCurrentById(CURRENT_ID_DISCHARGE));
}
static CURRENT_TYPE get_current_charge(void)
{
	return (getCurrentById(CURRENT_ID_CHARGE));
}

// simulator current
//CURRENT_TYPE set_simulator_current(TYPE_CURRENT charge_current,
//		TYPE_CURRENT discharge_current)
//{
//	//TODO
//	return 0;
//}

static int updataFeedbackCurrentValue(void)
{
	TYPE_CURRENT current = 0;
	int systemState = getSystemState();
	if (systemState == SYSTEM_MODE_DISCHARGE)
		current = getCurrentById(CURRENT_ID_DISCHARGE);
	setCurrentValueById( CURRENT_ID_FEEDBACK, current);
	return current;
}
static int updataBusCurrentValue(void)
{
		int systemState = getSystemState();
		TYPE_CURRENT current = 0;
		if (systemState == SYSTEM_MODE_DISCHARGE)
		{
			current = getCurrentById(CURRENT_ID_DISCHARGE);
		}
		else if (systemState == SYSTEM_MODE_AC_CHARGE
				|| systemState == SYSTEM_MODE_DC_CHARGE)
		{
			current = getCurrentById(CURRENT_ID_CHARGE);
		}
		else 
		{
			current = 0;
		}
		setCurrentValueById( CURRENT_ID_BUS, current);
		return current;
}

int observerCurrentUpdate(void)
{
	enum UCT_ERR_CODE rv = UCT_SUCCESS;
	TYPE_CURRENT current  = 0;
	int i = 0;
	for (i = 0; i < TOTAL_CURRENTS; i++)
	{
		current = (TYPE_CURRENT)((CURRENT_ADC_READ(gThresholdValue.mCurrentThreshold[i].adChannel) * 3) / 32); //((X * 1.25mV) * 300A ) / (4*1000)
		setCurrentValueById(i, current);
	}
	
	updataBusCurrentValue();
	updataFeedbackCurrentValue();
	updateCurrentStatusValue();
	return (rv);
}



TYPE_CURRENT BMS_GetCurrentValueFeedback(void)
{
	return getCurrentById(CURRENT_ID_FEEDBACK);
}

TYPE_CURRENT BMS_GetCurrentValueBUS(void)
{
	return getCurrentById(CURRENT_ID_BUS);
}


int BMS_GetErrStatusCurrentByType( UINT8 types )
{
	//TODO
	int ret = 0;
	switch(types)
	{
	case STATE_CURRENT_DISCHARGE_OC_LEVER1:
		if( getCurrentStatusById(CURRENT_ID_DISCHARGE) == SYSTEM_FAULT_LEVER1 )
		{
			return 1;
		}
		else 
			return 0;

	case STATE_CURRENT_DISCHARGE_OC_LEVER2:
		if( getCurrentStatusById(CURRENT_ID_DISCHARGE) == SYSTEM_FAULT_LEVER2 )
		{
			return 1;
		}
		else 
			return 0;
	case STATE_CURRENT_CHARGE_OC_LEVER1:
		if( getCurrentStatusById(CURRENT_ID_CHARGE) == SYSTEM_FAULT_LEVER1 )
		{
			return 1;
		}
		else 
			return 0;

	case STATE_CURRENT_CHARGE_OC_LEVER2:
		if( getCurrentStatusById(CURRENT_ID_CHARGE) == SYSTEM_FAULT_LEVER2 )
		{
			return 1;
		}
		else 
			return 0;
	case STATE_CURRENT_FEEDBACK_OC_LEVER1:
		if( getCurrentStatusById(CURRENT_ID_FEEDBACK) == SYSTEM_FAULT_LEVER1 )
		{
			return 1;
		}
		else 
			return 0;

	case STATE_CURRENT_FEEDBACK_OC_LEVER2:
		if( getCurrentStatusById(CURRENT_ID_FEEDBACK) == SYSTEM_FAULT_LEVER2 )
		{
			return 1;
		}
		else 
			return 0;
	default:
		return 0;
	}
}

static void printf_currents(void)
{
	UINT8 i = 0;
	for (i = 0; i < TOTAL_CURRENTS; i++)
	{
		mprintf("current[%u]:%uA ", i, getCurrentById(i));
	}
	mprintf("\r\n");
}
