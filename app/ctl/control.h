/*
 * control.h
 * err check and state update check
 * add by mgc 2014-11-10
 * */

#include "public\public.h"
#ifndef _CONTROL_H_
#define _CONTROL_H_


int AlarmUpdateVoltage(void);	
int AlarmUpdateTemperature(void);	
int AlarmUpdateCurrent(void);	
int AlarmUpdateSoc(void);	
int AlarmUpdateInsulutionResistance(void);	
int AlarmUpdateRelayModule(void);	
int AlarmUpdateBmsSelfCheck(void);	

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
