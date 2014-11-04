/*********************************************************
 *
 * (UCT)
 *
 * File Description
 *
 *   $Revision: 3305 $
 *   $Author: huhai $
 *   $Date: 2014-08-27 20:04:38 +0800 (周三, 27 八月 2014) $
 *
 *********************************************************/

#ifdef _USE_CODEWARRIOR
#include "public.h"
#else
#ifdef _USE_IAR
#include "public\public.h"
#endif
#endif
#ifndef _CONTROL_H_
#define _CONTROL_H_

enum BMS_CONTROL_MODE_DEF
{
  BMS_CONTROL_MODE_AUTO = 0, BMS_CONTROL_MODE_TEST,
};

enum BMS_STATE
{
  BMS_NOMAL = 0x00, // normal
  BMS_DANGER_HI = 0x01, //overpressure or overcurrent or hyperthermy
  BMS_DANGER_LOW = 0x02, // too slow or unbelieveble
  BMS_WARN_HI = 0x03, //overpressure or overcurrent or hyperthermy
  BMS_WARN_LOW = 0x04, // too slow or unbelieveble
};

enum BMS_CONTROL_STATE
{
  BMS_CTL_NOMAL = 0x00, // normal
  BMS_CTL_HI = 0x01, //overpressure or overcurrent or hyperthermy
  BMS_CTL_LOW = 0x02, // too slow or unbelieveble
};

enum BM_PROTECT_STATUS
{
  PROTECT_HIGH_TEMPERARTURE = 1,
  PROTECT_HIGH_VOL = 1 << 1,
  PROTECT_LOW_VOL = 1 << 2,
  PROTECT_OVER_CURRENT = 1<< 3,
  PROTECT_SHORT_CIRCUIT = 1 << 4
};

union u_bms_battery_state
{
  struct s_state_bits
  {
    UINT16 s_single_vol :2;
    UINT16 s_soc :2;
    UINT16 s_current :2;
    UINT16 s_temperature :2;
    UINT16 s_insulator :2;
    UINT16 s_connector :2;
    UINT16 s_charge_allowed :2;
  } state_bits;
  UINT8 state_bytes[2];
};

//union u_bms_battery_state g_bms_battery_state;

union bms_stop_charge_err
{
  UINT16 err_bytes;
  struct err_bits
  {
    UINT16 err_insulator :2;
    UINT16 err_connector_overtemperature :2;
    UINT16 err_bms_component_overtemperature :2;
    UINT16 err_conncetor_connect :2;
    UINT16 err_batterypacks_overtemperature :2;
    UINT16 err_other :2;
  };
};

struct battery_state
{
  UINT8 s_single_vol;
  UINT8 s_soc;
  UINT8 s_current;
  UINT8 s_temperature;
  UINT8 s_insulator;
  UINT8 s_connector;
  UINT8 s_charge_allowed;
};
/*
enum flowdown_state
GetBatteryMode(void);
*/
enum UCT_ERR_CODE
temp_manage(void);
enum UCT_ERR_CODE
vol_manage(void);
enum UCT_ERR_CODE
cur_manage(void);
enum UCT_ERR_CODE
insulation_res_manager(void);

UINT8
control_mode_get(void);
enum UCT_ERR_CODE
control_mode_set(UINT8 mod);
enum UCT_ERR_CODE
device_control(UINT8 dev, UINT8 len, const UINT8 *data);
enum UCT_ERR_CODE
soc_manage(INT8 soc);
/*
UINT8
getChargeStatus(void);
*/
int
ctl_init(void);
UINT8
vol_state_get();
UINT8
soc_state_get();
UINT8
current_state_get();
UINT8
temperature_state_get();
UINT8
get_ctl_state(void);
INT16
store_ctl_param(void);

void
set_protect_status(UINT32 status);
void
reset_protect_status(UINT32 status);
UINT32
getProctectStatusState(void);
UINT8
checkPortectStatusState(UINT32 status);

#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
