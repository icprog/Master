/*
 * this module should implement the protocol of the communication between
 * CPCU(charge point communication unit) and BMS(battery manage system).
 * which is described by <<GBT 27930>>
 * by mgc 20141106
 * */
#ifndef _CPCU_PROTOCOL_H_
#define _CPCU_PROTOCOL_H_


/*
 * parameter configure state
 * 
 * */
// TODO
extern UINT8 GetBMSProtocolVersion(void);// TODO
extern UINT8 GetBatteryType(void);// TODO
extern UINT16 GetBatteryChargeTimes(void);// TODO
extern UINT16 GetBatteryMaxAllowedChargeVoltage(void);// TODO
extern UINT16 GetBatteryMaxAllowedChargeCurrent(void);// TODO


/*
 * ready to charge
 * */
extern int BMS_ReadyToChargeStatus(void);  //TODO before charge , what should do.

/*
 * state:
 * phase charging
 * the next functions are needed at this state.
 * */

/*BCL*/
UINT16 B2P_CPCU_VoltageNeedGet(void);
UINT16 B2P_CPCU_CurrentNeedGet(void);
UINT16 B2P_CPCU_ChargeMode(void);

/*BCS*/
extern UINT32 BMS_GetVoltageValueBatteryTotal(void);  //mv
extern TYPE_VOLTAGE BMS_GetVoltageValueMax(VOID);  //MV
extern int BMS_GetVolValueMaxCellId(void);
extern TYPE_CURRENT BMS_GetChargeCurrentValue(void);
extern UINT8 GetSoc(void);

extern UINT16 GetChargeVoltage(void);  //TODO
extern UINT16 GetRestChargingTime(void); //TODO

/*BSM*/
extern int BMS_GetTemperatureMaxValueSensorId(void);
extern int BMS_GetTemperatureMinValueSensorId(void);
extern TYPE_TEMP BMS_GetTemperaturValueMaxValue(void);
extern TYPE_TEMP BMS_GetTemperatureValueMinValue(void);

int BMS_CPCU_GetVechleVoltageState(void); //TODO 
int BMS_CPCU_GetCellVoltageState(void); //TODO
int BMS_CPCU_GetChargeCurrentState(void); //TODO
int BMS_CPCU_GetInsulationState(void); //TODO
int BMS_CPCU_GetOutConnectorConnectStatus(void); //TODO
extern int GetChargeFuncionAllowedStatus(void);  //TODO

/*BMV*/
extern TYPE_VOLTAGE getVoltageValueByCellIndex(UINT8 index);

/*BMT*/
extern TYPE_TEMP BMS_GetTemperatureValueById(int id);

/*BSP*/
//none

/*BST*/
struct stopChargeReason
{
	UINT8 stopReason;
	UINT16 FaultReason;
	UINT8 errReason;
};
int StopCharge(struct stopChargeReason *reason); //TODO

/*CST*/
//TODO


/*
 * 
 * the next state is the end of charging state.
 * */
/*BSD*/
extern UINT8 GetSoc(void);
extern TYPE_VOLTAGE BMS_GetVoltageValueMaxCell(void); //mv
extern TYPE_VOLTAGE BMS_GetVoltageValueMinCell(void); //mv
extern TYPE_TEMP BMS_GetTemperaturValueMaxValue(void);
extern TYPE_TEMP BMS_GetTemperatureValueMinValue(void);

/*CSD*/


/*
 * errs 
 * */
/*BEM*/ 
 //TODO

#endif

