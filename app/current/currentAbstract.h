#ifndef _CURRENT_ABSTRACT_H_
#define _CURRENT_ABSTRACT_H_
#include "public/types.h"
#include "adc/adc.h"

/*
 * types
 * */

enum type_current_state
{
	STATE_CURRENT_NORMAL,
	STATE_CURRENT_DISCHARGE_OC_LEVER1,
	STATE_CURRENT_DISCHARGE_OC_LEVER2,
	STATE_CURRENT_CHARGE_OC_LEVER1,
	STATE_CURRENT_CHARGE_OC_LEVER2,
	STATE_CURRENT_FEEDBACK_OC_LEVER1,
	STATE_CURRENT_FEEDBACK_OC_LEVER2,
	STATE_CURRENT_TOTAL
};

enum current_id
{
    CURRENT_ID_DISCHARGE = 0,
    CURRENT_ID_CHARGE,
    CURRENT_ID_BUS,
    CURRENT_ID_FEEDBACK,
    TOTAL_CURRENTS
};

enum current_adchannel
{
  ADC_DISCHARGE_CURRENT = 0,
  ADC_CHARGE_CURRENT ,
  TOTAL_CURRENT_ADCS
};

/* 
 * 
 * */
#define CURRENT_ADC_READ(i)  Adc_ReadResultMvByChannel(i)

extern int GetSystemState(void);
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableDischarge(void);
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableCharge(void);



// simulator current
TYPE_CURRENT set_simulator_current( TYPE_CURRENT charge_current, TYPE_CURRENT discharge_current );

//TYPE_CURRENT getCurrentById(int id);
TYPE_CURRENT BMS_GetCurrentValueFeedback(void);
TYPE_CURRENT BMS_GetCurrentValueBUS(void);
int observerCurrentUpdate(void); // called by adc update
int BMS_GetErrStatusCurrentByType( UINT8 types );

TYPE_CURRENT BMS_GetCurrentValueMaxAllowableFeedback(void);  //TODO

#endif

