#ifndef _CURRENT_ABSTRACT_H_
#define _CURRENT_ABSTRACT_H_
#include "public/types.h"
#include "adc/adc.h"


enum current_id
{
    CURRENT_ID_DISCHARGE = 0,
    CURRENT_ID_CHARGE,
    CURRENT_ID_FEEDBACK,
    TOTAL_CURRENTS
};

enum current_adchannel
{
  ADC_DISCHARGE_CURRENT = 0,
  ADC_CHARGE_CURRENT ,
  TOTOAL_ADCS
};

#define getmVbyAdc(i)  Adc_ReadResultMvByChannel(i)

CURRENT_TYPE getCurrentById(UINT8 id);

// simulator current
CURRENT_TYPE set_simulator_current( CURRENT_TYPE charge_current, CURRENT_TYPE discharge_current );

#endif

