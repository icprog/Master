#ifndef _RELAY_H_
#define _RELAY_H_
#include "public/public.h"
#include "Gpio/gpio.h"

/*
 * needed by vcu_protocol.h
 * */
int BMS_GetErrStatusRelayByType( UINT8 types); //1:err, 0 OK
int BMS_GetRelayStatePositiveFeedbackConnnect(void); // 1:on, 0:off
int BMS_GetRelayStateNegtiveFeedbackConnnect(void);  // 1:on, 0:off

#endif
