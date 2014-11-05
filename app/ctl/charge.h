#ifndef _CHARGE_H_
#define _CHARGE_H_

/*****************************************includs******************************************/

#include "public/public.h"
#include "voltage/voltage.h"
#include "current/current.h"


/*****************************************types******************************************/
typedef INT16
(*charge_events)(UCHAR argc, const UCHAR *argv[]);
struct _charge_events
{
  const char *name;
  charge_events process;
};

enum vol_type
{
  VTYPE_VMIN, VTYPE_VMAX, VTYPE_VSUM
};

enum realtions_c
{
  R_SMALL = '<',
  R_BIGGER = '>',
  R_EQUAL = 'E',
  R_SMALL_OR_EQUAL = '0',
  R_BIGGER_OR_EQUAL = '1'
};
struct _charge_control_param
{
  UINT16 control_voltage;
  UINT16 control_current;
  enum vol_type voltage_type;
  enum realtions_c relation_type;
};
enum charge_mode
{
  C_MODE_CONSTCURRENT, C_MODE_CONSTVOLTAGE, C_MODE_STOP
};

/*****************************************MACROS******************************************/
#define MAX_BATTERY_C 100
#define GET_CURRENT_BY_C(x) ((x)*MAX_BATTERY_C)
#define REGISTER_CHARGE_EVENTS( NAME  ) {\
        #NAME,\
        CHARGE_CTL_##NAME\
    }
#define DECLARE_CHARGE_PROCESS( NAME ) INT16 CHARGE_CTL_##NAME( UCHAR argc,const UCHAR *argv[] )

/*****************************************function declaration******************************************/
DECLARE_CHARGE_PROCESS( temperature);
DECLARE_CHARGE_PROCESS( voltage);
DECLARE_CHARGE_PROCESS( current);

/*****************************************constant value******************************************/
static const INT8 charge_max_temperature = 50;
static const INT8 charge_min_temperature = 0;
static const UINT16 charge_const_voltage = 42000; //V_TO_100UV ( 4.2 );
static const INT16 charge_stop_min_current = GET_CURRENT_BY_C ( 0.05 );
static const struct _charge_events g_charge_events[] =
  { REGISTER_CHARGE_EVENTS ( temperature ), REGISTER_CHARGE_EVENTS ( voltage ),
  REGISTER_CHARGE_EVENTS ( current ) };

static const UINT8 g_charge_events_size = sizeof(g_charge_events)
    / sizeof(struct _charge_events);
static const struct _charge_control_param g_charge_control[] =
  {
    { 25000, GET_CURRENT_BY_C ( 0.1 ), VTYPE_VMIN, R_SMALL_OR_EQUAL },
    { 38000, GET_CURRENT_BY_C ( 0.8 ), VTYPE_VMAX, R_SMALL_OR_EQUAL },
    { 40000, GET_CURRENT_BY_C ( 0.5 ), VTYPE_VMAX, R_SMALL_OR_EQUAL },
    { 42000, GET_CURRENT_BY_C ( 0.2 ), VTYPE_VMAX, R_SMALL }
  /*
   { V_TO_100UV ( 2.5 ), GET_CURRENT_BY_C ( 0.1 ), VTYPE_VMIN, R_SMALL_OR_EQUAL  },
   { V_TO_100UV ( 3.8 ), GET_CURRENT_BY_C ( 0.8 ), VTYPE_VMAX, R_SMALL_OR_EQUAL  },
   { V_TO_100UV ( 4.0 ), GET_CURRENT_BY_C ( 0.5 ), VTYPE_VMAX, R_SMALL_OR_EQUAL  },
   { V_TO_100UV ( 4.2 ), GET_CURRENT_BY_C ( 0.2 ), VTYPE_VMAX, R_SMALL  }
   */
  };

/*
 * 
 * */
/****************************************external interface needs******************************************/
#define GET_CURRENT_V_SUM() BMS_GetVoltageValueBatteryTotal()  // ret UINT32
#define GET_CURRENT_V_MIN() BMS_GetVoltageValueMinCell() //ret UINT16
#define GET_CURRENT_V_MAX() BMS_GetVoltageValueMaxCell() //ret UINT16
#define GET_CURRENT() 0//get_current() //ret UINT16
#define GET_TEMPERATURE_MAX() BMS_GetTemperaturValueMaxValue()// ret INT16
#define GET_TEMPERATURE_MIN() BMS_GetTemperaturValueMinValue()// ret INT16

/****************************************for external interface******************************************/
INT16 charge_control(void); //return >=0 success
int charge_init(void);
int start_precharge(void);
int stop_charge(void);

enum TYPE_BMS_POWER_ON
{
	POWER_ON_RUNNING,
	POWER_ON_OVER
};


int BMS_GetChargerConnectStatus(void);    // 1,connect 0:unconnected
int BMS_IsChargging(void);                //1:charging, 0:uncharged
int BMS_PowerOn(void);
int BMS_GetPowerOnStatus(void); //enum TYPE_BMS_POWER_ON
int BMS_PowerOff(void);
TYPE_CURRENT BMS_GetCurrentValueMaxAllowableDischarge(void);

int BMS_SelfCheckPass(void);  //1 pass, 0:negative

int SetSystemActiveMode(UINT8 mode);
UINT8 GetSystemActiveMode( void );
int SleepModeDetection(void);
#endif

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
