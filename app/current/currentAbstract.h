#ifndef _CURRENT_ABSTRACT_H_
#define _CURRENT_ABSTRACT_H_
/*
 * current module
 * by mgc 20141105
 * */

#include "public/types.h"
#include "adc/adc.h"
#include "public/systemState.h"


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
 * macros
 * */
//TODO
#define ADC_CHANNEL_CHARGE
#define ADC_CHANNEL_DISCHARGE

#define THREATHOLD_VALUE_CAHRGE_OC_LEVER1
#define THREATHOLD_VALUE_CAHRGE_OC_LEVER2
#define THREATHOLD_VALUE_DISCAHRGE_OC_LEVER1
#define THREATHOLD_VALUE_DISCAHRGE_OC_LEVER2
#define THREATHOLD_VALUE_FEEDBACK_OC_LEVER1
#define THREATHOLD_VALUE_FEEDBACK_OC_LEVER2



/* 
 * external fuction interfaces
 * */
#define CURRENT_ADC_READ(i)  Adc_ReadResultMvByChannel(i)
extern int GetSystemState(void);


/*private interfaces*/
//TYPE_CURRENT getCurrentById(int id);
TYPE_CURRENT BMS_GetCurrentValueFeedback(void);
TYPE_CURRENT BMS_GetCurrentValueBUS(void);
int observerCurrentUpdate(void); // called by adc update
int BMS_GetErrStatusCurrentByType( UINT8 types );


extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableFeedback(void);  //TODO
// simulator current
TYPE_CURRENT set_simulator_current( TYPE_CURRENT charge_current, TYPE_CURRENT discharge_current ); //TODO
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableDischarge(void);  //TODO
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableCharge(void);   //TODO
#endif

