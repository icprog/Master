/*********************************************************
*
* (UCT)
*
* File Description
*
*   $Revision: 2057 $
*   $Author: huhai $
*   $Date: 2014-07-11 14:24:25 +0800 (周五, 11 七月 2014) $
*
*********************************************************/


#ifndef _CURRENT_H_
#define _CURRENT_H_
#include "public/public.h"
#include "current/currentAbstract.h"
#include "log/log.h"


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

extern TYPE_CURRENT BMS_GetCurrentValueBUS(void);
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableDischarge(void);
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableCharge(void);
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableFeedback(void);
int BMS_GetErrStatusCurrentByType( UINT8 types );

int observerCurrentUpdate(void);
void printf_currents(void);
  
#endif
