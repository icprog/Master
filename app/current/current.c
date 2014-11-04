/*********************************************************
 *
 * ��оԴ΢�������޹�˾(UCT)
 *
 * File Description
 *
 *   $Revision: 3228 $
 *   $Author: huhai $
 *   $Date: 2014-08-26 14:44:19 +0800 (周二, 26 八月 2014) $
 *
 *********************************************************/

#include "string.h"
#include "adc/adc.h"
#include "temp/temperature.h"
#include "public/public.h"
#include "current/current.h"

//#ifdef _USE_BM01
UINT16 g_UCT_current = 0;
//#endif
#define _USE_BM06
#ifdef _USE_BM06

#define VALUE_K_ADC2C_DISCHARGE  (0.02)
#define VALUE_K_ADC2C_DISCHARGE_OC  (0.0125)
#define VALUE_K_ADC2C_CHARGE  (0.039)
#define VALUE_K_ADC2C_CHARGE_OC  (0.0125)

struct CCurrent
{
	CURRENT_TYPE mCurrentCurrent;
	CURRENT_TYPE mSimulatorCurrent;
	UINT8 mCurrentStatus;
	UINT32 timer_OC;
};

struct CCurrentThresholdValue
{
	CURRENT_TYPE mThretholdCurrentOC;
	UINT32 mThretholdTimerOCFilter;
	float kVoltageToCurrent;
};

struct CCurrentsMember
{
	struct CCurrent mCurrents[TOTAL_CURRENTS];
};
#if 0
CURRENT_TYPE m_currents[TOTAL_CURRENTS];
CURRENT_TYPE m_currents_simulator[TOTAL_CURRENTS];
UINT8 m_currents_status[TOTAL_CURRENTS];
UINT32 timer_OC[TOTAL_CURRENTS];
#endif

struct CCurrentsThresholdConstValue
{
	struct CCurrentThresholdValue mCurrentThreshold[TOTAL_CURRENTS];
};

//static const float k_discharge = 0.02;
//static const float k_discharge_oc = 0.0125;
//static const float k_charge = 0.039;
//static const float k_charge_oc = 0.0125;

static const struct CCurrentsThresholdConstValue gThresholdValue =
{
//		{
//		{ VALUE_TH_OC, VALUE_TH_TIME_FILTER_OC, VALUE_K_ADC2C_CHARGE },
//		{ VALUE_TH_OC, VALUE_TH_TIME_FILTER_OC, VALUE_K_ADC2C_DISCHARGE },
//		{ VALUE_TH_SHORT_CIRCUIT, VALUE_TH_TIME_FILTER_SC,
//				VALUE_K_ADC2C_CHARGE_OC },
//		{ VALUE_TH_SHORT_CIRCUIT, VALUE_TH_TIME_FILTER_SC,
//				VALUE_K_ADC2C_DISCHARGE_OC } } 
};
//
//static const CURRENT_TYPE g_threthold_oc[TOTAL_CURRENTS] =
//{ VALUE_TH_OC, VALUE_TH_OC, VALUE_TH_SHORT_CIRCUIT, VALUE_TH_SHORT_CIRCUIT };
//
//static const UINT32 g_threthold_oc_filter_timer[TOTAL_CURRENTS] =
//{ VALUE_TH_TIME_FILTER_OC, VALUE_TH_TIME_FILTER_OC, VALUE_TH_TIME_FILTER_SC, VALUE_TH_TIME_FILTER_SC };

static struct CCurrentsMember gCurrent;
//static CURRENT_TYPE g_currents[TOTAL_CURRENTS];
//static CURRENT_TYPE g_simulator_charge_current = 0;
//static CURRENT_TYPE g_simulator_discharge_current = 0;
////g_current_discharge = 0;
////static UINT16 g_current_discharge_oc = 0;
////static UINT16 g_current_charge = 0;
//static UINT16 g_current_charge_oc = 0;
#endif

UINT8 getCurrentStatusById(UINT8 id)
{
	return (gCurrent.mCurrents[id].mCurrentStatus);
}


CURRENT_TYPE getCurrentBymV(UINT16 mv, float k)
{
	CURRENT_TYPE current = 0;
	float v = mv * 0.001;
	current = (CURRENT_TYPE) (v / k);
	return (current);
}

CURRENT_TYPE get_current_discharge(void)
{
	return (getCurrentById(CURRENT_ID_DISCHARGE));
}
CURRENT_TYPE get_current_charge(void)
{
	return (getCurrentById(CURRENT_ID_CHARGE));
}
// over current
//CURRENT_TYPE get_current_charge_oc(void)
//{
//	return (getCurrentById(CURRENT_ID_CHARGE_OC));
//}
//CURRENT_TYPE get_current_discharge_oc(void)
//{
//	return (getCurrentById(CURRENT_ID_DISCHARGE_OC));
//}

CURRENT_TYPE set_simulator_current(CURRENT_TYPE charge_current,
		CURRENT_TYPE discharge_current)
{
//	gCurrent.mCurrents[CURRENT_ID_DISCHARGE_OC].mSimulatorCurrent =
//			discharge_current;
//	gCurrent.mCurrents[CURRENT_ID_CHARGE_OC].mSimulatorCurrent = charge_current;
	return (0);
}

void currentStatusCheck(void)
{
	UINT8 i = 0;
	struct CCurrent *this;
	const struct CCurrentThresholdValue *pthresholdValue;
	for (i = 0; i < TOTAL_CURRENTS; i++)
	{
		this = &gCurrent.mCurrents[i];
		pthresholdValue = &gThresholdValue.mCurrentThreshold[i];
		if (this->mCurrentCurrent >= pthresholdValue->mThretholdCurrentOC)
		{
			if (this->timer_OC > pthresholdValue->mThretholdTimerOCFilter)
				this->mCurrentStatus = 1;
			else
				this->timer_OC++;
		}
		else
		{
			this->timer_OC = 0;
		}
	}

#if 0
	if (getCurrentById(CURRENT_ID_DISCHARGE) >= VALUE_TH_OC)
	{
		if (timer_OC[CURRENT_ID_DISCHARGE] > VALUE_TH_TIME_FILTER_OC)
		t_oc_protect = 1;
		else
		timer_OC[CURRENT_ID_DISCHARGE]++;
	}
	else
	timer_OC[CURRENT_ID_DISCHARGE] = 0;

	/*short circuit check*/
	if (getCurrentById(CURRENT_ID_CHARGE_OC) >= VALUE_TH_SHORT_CIRCUIT)
	{
		if (timer_OC[CURRENT_ID_CHARGE_OC] > VALUE_TH_TIME_FILTER_SC)
		{
			t_sc_protect = 1;
		}
		else
		{
			timer_OC[CURRENT_ID_CHARGE_OC]++;
		}
	}
	else
	timer_OC[CURRENT_ID_CHARGE_OC] = 0;
	if (getCurrentById(CURRENT_ID_DISCHARGE_OC) >= VALUE_TH_SHORT_CIRCUIT)
	{
		if (timer_OC[CURRENT_ID_DISCHARGE_OC] > VALUE_TH_TIME_FILTER_SC)
		t_sc_protect = 1;
		else
		timer_OC[CURRENT_ID_DISCHARGE_OC]++;
	}
	else
	timer_OC[CURRENT_ID_DISCHARGE_OC] = 0;
#endif
}

int observerCurrentUpdate(void)
{
	enum UCT_ERR_CODE rv = UCT_SUCCESS;
	UINT8 i = 0;
	/*
	 UINT8 temp_lo = 0;
	 UINT8 temp_hi = 0;
	 UINT16 current = 0;

	 ATD0CTL5 = 0x81;              // Analog Input Channel Select : AN01

	 while ( !ATD0STAT2_CCF0 );

	 temp_lo = ATD0DR0L;
	 temp_hi = ATD0DR0H;
	 current = ( ( temp_hi << 8 ) & 0x0F00 ) | ( temp_lo & 0xFF );
	 */
#ifdef _USE_BM06
	for (i = 0; i < TOTAL_CURRENTS; i++)
	{
		gCurrent.mCurrents[i].mCurrentCurrent = getCurrentBymV(getmVbyAdc(i),
				gThresholdValue.mCurrentThreshold[i].kVoltageToCurrent);
	}
//g_currents[CURRENT_ID_DISCHARGE] = getCurrentBymV(
//		getmVbyAdc(ADC_DISCHARGE_CURRENT), k_discharge);
//g_currents[CURRENT_ID_DISCHARGE_OC] = getCurrentBymV(
//		getmVbyAdc(ADC_DISCHARGE_OC), k_discharge_oc);
//g_currents[CURRENT_ID_CHARGE] = getCurrentBymV(getmVbyAdc(ADC_CHARGE_CURRENT),
//		k_charge);
//g_currents[CURRENT_ID_CHARGE_OC] = getCurrentBymV(getmVbyAdc(ADC_CHARGE_OC),
//		k_charge_oc);
#endif
//g_UCT_current = ( read_adc(AD_CURRENT_CHANNEL) * 3 ) / 32; //((X * 1.25mV) * 300A ) / (4*1000)
	i = get_simulator_mode();
	if ((i & MASK_BIT_SIMULATOR_MODE_CURRENT) == 0)
	{
//		gCurrent.mCurrents[CURRENT_ID_CHARGE_OC].mCurrentCurrent =
//				gCurrent.mCurrents[CURRENT_ID_CHARGE_OC].mSimulatorCurrent;
//		gCurrent.mCurrents[CURRENT_ID_DISCHARGE_OC].mCurrentCurrent =
//				gCurrent.mCurrents[CURRENT_ID_DISCHARGE_OC].mSimulatorCurrent;
//	g_currents[CURRENT_ID_DISCHARGE_OC] = g_simulator_discharge_current;
//	g_currents[CURRENT_ID_CHARGE_OC] = g_simulator_charge_current;
//	g_currents[CURRENT_ID_DISCHARGE_OC] = g_simulator_discharge_current;
	}

	currentStatusCheck();
	return (rv);
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
