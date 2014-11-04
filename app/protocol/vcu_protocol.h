#ifndef _VCU_PROTOCOL_H_
#define _VCU_PROTOCOL_H_
/* vcu_protocol.h
 * the protocol for VCU communication
 * by mgc 2014-11-03
 * */
#include "public/types.h"
/*
 * need external module interfaces
 * */
extern UINT8 BMS_GetSOCValue(void);  //0~100
extern UINT8 BMS_GetSOHValue(void);	 //0~100

/*currents*/
extern TYPE_CURRENT BMS_GetCurrentValueBUS(void);
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableDischarge(void);
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableCharge(void);
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableFeedback(void);

/*voltage*/
extern UINT32 BMS_GetVoltageValueBatteryTotal(void);  //mv
extern TYPE_VOLTAGE BMS_GetVoltageValueMaxCell(void); //mv
extern TYPE_VOLTAGE BMS_GetVoltageValueMinCell(void); //mv

/*insulation resistance */
extern TYPE_INSRES GetInsulationResValuePositivePole(void);  //ko
extern TYPE_INSRES GetInsulationResValueNegativePole(void); //ko

/*temperature value*/
TYPE_TEMP BMS_GetTemperatureValueAverage(void);
TYPE_TEMP BMS_GetTemperaturValueMaxValue(void);
TYPE_TEMP BMS_GetTemperatureValueMin(void);
int BMS_GetTemperatureIdMaxValue(void);
int BMS_GetTemperatureIdMinValue(void);


extern int BMS_SelfCheckPass(void);  //1 pass, 0:negative
/*
 * the next 7 functions return value: 0 normal, 1:err
 * */
extern int BMS_DetectionStateCheckCurrent(void);
extern int BMS_DetectionStateCheckVoltage(void);
extern int BMS_DetectionStateCheckTemperature(void);
extern int BMS_DetectionStateCheckBalance(void);
extern int BMS_DetectionStateCheckCanCom(void);
extern int BMS_DetectionStateCheckVCULost(void);

extern UINT16 BMS_GetLoopChargeTimes(void);
extern int BMS_GetRequestPositiveRelayShutdown(void); // 1 have request, 0: no request

extern int BMS_GetChargerConnectStatus(void);    // 1,connect 0:unconnected
extern int BMS_IsChargging(void);                //1:charging, 0:uncharged

extern int BMS_GetRelayStatePositiveFeedbackConnnect(void); // 1:on, 0:off
extern int BMS_GetRelayStateNegtiveFeedbackConnnect(void);  // 1:on, 0:off

extern int BMS_GetErrStatusPrecharge(void);                 //1 err, 0 OK
extern int BMS_GetErrStatusTemperatureByType( UINT8 types);     //1:err, 0 OK
extern int BMS_GetErrStatusCurrentByType( UINT8 types);         //1:err, 0 OK
extern int BMS_GetErrStatusVoltageByType( UINT8 types);         //1:err, 0 OK
extern int BMS_GetErrStatusSOCByType( UINT8 types);             //1:err, 0 OK
extern int BMS_GetErrStatusRelayByType( UINT8 types); //1:err, 0 OK
extern int BMS_GetErrStatusResInsulationByType( UINT8 types); //1:err, 0 OK
extern int BMS_GetErrStatusBMSCheckSelf(void); //1:err, 0 OK	

/*
 * control interfaces
 * */
extern int OpenRelay(UINT8 types); //open the relay

#endif

