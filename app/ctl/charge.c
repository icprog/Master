/*********************************************************
 *
 * charge.c(UCT)
 *
 * File Description
 *
 *   $Revision: 1813 $
 *   $Author: huhai $
 *   $Date: 2014-06-12 11:11:58 +0800 $
 *   implement all of the function that described by <<ZS008....20140728-V0.6.doc>>
 *   by mgc 20140809
 *********************************************************/
#include "public/public.h"
#include "ctl/charge.h"
#include "current/current.h"
#include "voltage/voltage.h"
#include "gpio/gpio.h"
#include "Gpio/gpioFuncs.h"
#include "ctl/relay.h"

/***************************MACRO FUNCTIONS*************************************/
#define SET_CHARGE_BY_CONSTCURRENT(current) set_charge_mode( C_MODE_CONSTCURRENT, 0, current )
#define SET_CHARGE_BY_CONSTVOLTAGE(voltage) set_charge_mode( C_MODE_CONSTVOLTAGE, voltage, 0 )

/*************************const value*******************************************/

/*************************private value******************************************/
#ifdef _USE_IAR
//static UINT8 g_charge_mode = C_MODE_STOP;
#endif
#ifdef _USE_CODEWARRIOR
static enum charge_mode g_charge_mode = C_MODE_STOP;
#endif

#ifdef _USE_BM01

static INT16 g_charge_current = 0;
//static UINT16 g_charge_voltage_single = 0;
static UINT32 g_charge_voltage_sum = 0;

/**************************************************************************************
 * func: set_charge_mode
 * this function is used after charge action has been started already
 * param
 * mode:
 * voltage: if mode == C_MODE_CONSTVOLTAGE, use this value
 * current: if moded ==  C_MODE_CONSTCURRENT, use this parameter
 * return < 0 err, >0 mode changed, ==0 do nothing
 * by mgc 20140809
 **************************************************************************************/
INT16 set_charge_mode ( enum charge_mode mode, UINT32 voltage, UINT16 current )
  {
    INT16 ret = 0;

    if ( g_charge_mode != mode )
      {
        g_charge_mode = mode;
        ret = 1 << 0;
      }

    if ( g_charge_voltage_sum != voltage )
      {
        g_charge_voltage_sum = voltage;
        ret |= ( 1 << 1 );
      }

    if ( g_charge_current != current )
      {
        g_charge_current = current;
        ret |= ( 1 << 2 );
      }

    if ( ret > 0 )
      {
        //do something such as telling the CAN that should infomations changed
        mprintf ( "charge mode change:mode:%d, voltage:%04u, current:%04u\r\n",
            g_charge_mode, g_charge_voltage_sum, g_charge_current );
      }

    return ret;
  }

/*******************************************************************************************************
 * charge control
 * voltage:
 * return >0 process ok
 * = 0 not process, maybe should process by other function
 * < 0 err happen
 * by mgc 20140809
 ******************************************************************************************************/
DECLARE_CHARGE_PROCESS ( voltage )
  {

    UINT16 v_min = get_vol_min();
    UINT16 v_max = get_vol_max();
    UINT32 vsum = get_vol_sum();
    UINT8 i = 0;
    UINT32 v_sum = 0;
    UINT8 condition_ok = 0;
    UINT16 voltage = 0;

    for ( i = 0; i < sizeof ( g_charge_control ) / sizeof ( struct _charge_control_param ); i++ )
      {
        /*get  voltage*/
        if ( g_charge_control[i].voltage_type == VTYPE_VMIN )
        voltage = v_min;
        else
        voltage = v_max;

        /*get relations and process*/
        switch ( g_charge_control[i].relation_type )
          {
            case R_SMALL_OR_EQUAL:
              {
                if ( voltage <= g_charge_control[i].control_voltage )
                  {
                    condition_ok = 1;
                  }
              }
            break;
            case R_SMALL:
              {
                if ( voltage < g_charge_control[i].control_voltage )
                  {
                    condition_ok = 1;
                  }
              }
            break;
            default:
            return -1;
          }

        if ( condition_ok == 1 )
          {
            SET_CHARGE_BY_CONSTCURRENT ( g_charge_control[i].control_current );
            goto out;
          }

      }

    SET_CHARGE_BY_CONSTVOLTAGE ( v_sum );

    return 0;

    out:
    return i;
  }

DECLARE_CHARGE_PROCESS ( current )
  {
    INT16 current = GET_CURRENT();

    if ( current >= charge_stop_min_current )
      {
        set_charge_mode ( C_MODE_STOP, 0, 0 );
      }

    return 1;
  }

DECLARE_CHARGE_PROCESS ( temperature )
  {
    return 1;
  }

#endif
/*
 *
 * by mgc 20140827
 */
int timer_precharge;

//INT16 precharge_process ( int argc, char**argv )
//  {
//    //discharge_ctrl ( TURN_OFF );
//    charge_ctrl ( TURN_ON );
//    precharge_ctrl_later ( TURN_OFF );
//    timer_stop ( timer_precharge );
//    return 0;
//  }
//
//int precharge_control_timer_init ( void )
//  {
////    struct timerspec ts;
////    ts.time_ms = 0;
////    ts.time_s = 10; //MINITUE_TO_S ( 3 );
////    timer_precharge = timer_get ( &ts, ( timer_process ) precharge_process, 0 );
////
////    if ( timer_precharge < 0 )
////      {
////        //err process
////        return -1;
////      }
//
//    return 0;
//  }

//int start_precharge ( void )
//  {
//#ifdef _USE_BM01
//    precharge_ctrl_later ( TURN_ON );
//
//    if ( timer_reset ( timer_precharge ) < 0 )
//      {
//        goto timer_err;
//      }
//
//    if ( timer_start ( timer_precharge ) < 0 )
//      {
//        //err process
//        goto timer_err;
//      }
//#endif
//#ifdef _USE_BM03
//    charge_ctrl( TURN_ON );
//#endif
//
//    return 0;
//#ifdef _USE_BM01
//    timer_err:
//    return -1;
//#endif
//  }
//
//int stop_charge ( void )
//  {
//#if _USE_BM01
//    timer_stop ( timer_precharge );
//    precharge_ctrl_later ( TURN_OFF );
//#endif
//    charge_ctrl_later ( TURN_OFF );
//    return 0;
//  }
//
//int charge_init ( void )
//  {
//    return precharge_control_timer_init ( );
//  }


/*******************************************************************************************************
 * control the charge status by capture volatage , current, and temperature
 * return >=0 success, <0 err
 *******************************************************************************************************/
static const UINT32 charge_close_by_protect_status = 0;
//PROTECT_HIGH_VOL
//    | PROTECT_OVER_CURRENT | PROTECT_HIGH_TEMPERARTURE |PROTECT_SHORT_CIRCUIT ;
static const UINT32 discharge_close_by_protect_status = 0;
//    (PROTECT_HIGH_TEMPERARTURE | PROTECT_OVER_CURRENT
//        | PROTECT_LOW_VOL | PROTECT_SHORT_CIRCUIT);
INT16
charge_control(void)
{
  INT16 ret = 0;
#ifdef _USE_BM03
  if ((getProctectStatusState() & charge_close_by_protect_status) != 0)
    {
      charge_ctrl(TURN_OFF);
    }
  else
    {
      charge_ctrl(TURN_ON);
    }

  if ((getProctectStatusState() & discharge_close_by_protect_status) != 0)
    {
      discharge_ctrl(TURN_OFF);
    }
  else
    {
      discharge_ctrl(TURN_ON);
    }

#endif

#ifdef _USE_BM01
  UINT8 i = 0;
  for ( i = 0; i < g_charge_events_size; i++ )
    {
      if ( g_charge_events[i].process )
        {
          ret = g_charge_events[i].process ( 0, 0 );

          if ( ret > 0 )
          break;

          if ( ret < 0 )
            {
              //err process
              break;
            }
        }
    }
#endif
  return ret;
}

/*
 * system wake up
 * */
static int SystemWakeUp(void)
{
//	waitForKeyACCActive();
//	waitForKeyONActive();
//	waitForVCUCheckSelfOK();
	PowerOnHighVoltage();
	return 0;
}



int BMS_GetChargerConnectStatus(void)    // 1,connect 0:unconnected
{
	if(DC_CHARGER_INSERT() || AC_CHARGER_INSERT())
		return 1;
	return 0;
}

int BMS_IsChargging(void)             //1:charging, 0:uncharged
{
	//TODO 
	return 0;
}

/*
 * this  is a max allowable discharge current manager module
 * */
#define BATTERY_C 90 // A*H
static TYPE_CURRENT g_dischargeMaxAllowableCurrent = BATTERY_C;
TYPE_CURRENT BMS_GetCurrentValueMaxAllowableDischarge(void)
{
	//TODO 
	return g_dischargeMaxAllowableCurrent;
}

int SetCurrentMaxAllowableDischarge(TYPE_CURRENT current)
{
	//TODO
	return 0;
}

int ReduceCurrentMaxAllowableDischarge(TYPE_CURRENT reduceCurrent)
{
	//TODO
	if(reduceCurrent>g_dischargeMaxAllowableCurrent)
		g_dischargeMaxAllowableCurrent = 0;
	else
		g_dischargeMaxAllowableCurrent -= reduceCurrent;
	return 0;
}


/*
 * the next is the key activate and charge activate manage module
 * */
int KeyActiveCheck(void)
{
	if( KEY_TO_ACC() )
		return 1;
	return 0;
}
int DCChargeInActiveCheck(void)
{
	if( DC_CHARGER_INSERT() )
	{
		return 1;
	}
	return 0;
}

int ACChargeInActive(void)
{
	if( AC_CHARGER_INSERT())
	{
		return 1;
	}
	return 0;
}

static UINT8 gSysteActiveMode = 0;
int SetSystemActiveMode(UINT8 mode)
{
	gSysteActiveMode = mode;
	return 0;
}
UINT8 GetSystemActiveMode( void )
{
	return gSysteActiveMode;
}


int SleepModeDetection(void)
{
	if(KeyActiveCheck())
	{
		SetSystemActiveMode( SYSTEM_ACTIVE_MODE_KEY_TO_ACC );
	}
	else if(ACChargeInActive())
	{
		SetSystemActiveMode(SYSTEM_ACTIVE_MODE_ACCHARGER_IN);
	}
	else if(DCChargeInActiveCheck())
	{
		SetSystemActiveMode(SYSTEM_ACTIVE_MODE_DCCHARGER_IN);
	}
	return gSysteActiveMode;
}


int ACCharge(void)
{
	return 0;
}

