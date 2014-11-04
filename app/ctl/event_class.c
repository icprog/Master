/*********************************************************
 *
 * ��оԴ΢�������޹�˾(UCT)
 *
 * File Description
 *
 *   $Revision: 1813 $
 *   $Author: huhai $
 *   $Date: 2014-06-12 11:11:58 +0800 (星期�? 12 六月 2014) $
 */
#include "ctl\event_class.h"
#include "string.h"
#include "stdio.h"
#include "public\public.h"
#include "ctl\control.h"
#include "temp\temperature.h"
#include "voltage\voltage.h"
#include "current\current.h"
#include "gpio\gpio.h"
#include "timer\timer.h"
#include "log\log.h"
#include "protocol\cmd_class.h"
#include "ctl/event_class.h"
#include "banlance_battery/banlancebattery.h"

#define events_num  ( sizeof ( g_events ) / sizeof ( Cevent  ) )

INT16
event_add(struct event_class event)
{
  return 0;
}

static struct event_class g_events[] =
  { EVENTS_COLLETION };

INT16
event_set_state(enum event_id xevent, event_e state)
{
  UCHAR i = 0, nprocess = 0;

  for (i = 0; i < events_num; i++)
    {
      if (g_events[i].event_id == xevent && g_events[i].e_state != state)
        {
          // mprintf ( "%s state change from %d to %d !\r\n", g_events[i].event_name, g_events[i].e_state, state );
          g_events[i].e_state = state;
          nprocess++;
          break;
        }
    }

  return nprocess;
}

/*********************************************************
 *
 *********************************************************/
INT16
event_monitor(void) //ÃüÁîŽŠÀíÀàÊµÏÖ
{
  UCHAR i = 0, nprocess = 0;
  UINT8 *argv[1];

  for (i = 0; i < events_num; i++)
    {
      /*
       * now i set the condition that events only trigger once!!!
       * if change this condition, we can monitor all the states that any events change!!
       * by mgc 20140730
       */
      if (g_events[i].e_state == EVENT_TRIGGER) //&& g_events[i].running_state != EVENT_TRIGGER )
        {
          mprintf("%s trigger!\r\n", g_events[i].event_name);
          argv[0] = (UINT8*) &g_events[i];
          g_events[i].running_state = EVENT_TRIGGER;
          //g_events[i].event_process ( 1, argv );
          nprocess++;
        }
      else if (g_events[i].e_state != EVENT_TRIGGER)
        {
          g_events[i].running_state = g_events[i].e_state;
        }
    }
    (void)argv;
  return nprocess;
}

DECLARE_EVENT_PROCESS ( EVENT_TEMP_LOW_CRITICAL )
{
  log_write(LOG_MOD_TEMP_ID, LOG_STATUS_CRITICAL, TEMP_EVENT_LOW_TEMP, 0, 0); // ( UINT8 * ) &temp_critical_event );
  discharge_ctrl_later(TURN_OFF);
  return 0;
}

DECLARE_EVENT_PROCESS ( EVENT_TEMP_LOW_WARNNING )
{
  heat_ctrl_later(TURN_ON);
  cool1_ctrl_later(TURN_OFF);
  cool2_ctrl_later(TURN_OFF);
  //    cool_event = TURN_OFF;
  bms_overtemp_ctrl(TURN_OFF);
  log_write(LOG_MOD_TEMP_ID, LOG_STATUS_WARNING, TEMP_EVENT_LOW_TEMP, 0, 0); //2, ( UINT8 * ) &temp_critical_event );
  return 0;
}

DECLARE_EVENT_PROCESS ( EVENT_TEMP_HIGH_CRITICAL )
{
  log_write(LOG_MOD_TEMP_ID, LOG_STATUS_CRITICAL, TEMP_EVENT_HIGH_TEMP, 0, 0); //( UINT8 * ) &temp_critical_event );
  discharge_ctrl_later(TURN_OFF);
  return 0;
}
DECLARE_EVENT_PROCESS ( EVENT_TEMP_HIGH_WARNNING )
{
  cool1_ctrl_later(TURN_ON);
  cool2_ctrl_later(TURN_ON);
  bms_overtemp_ctrl(TURN_ON);
  heat_ctrl_later(TURN_OFF);
  log_write(LOG_MOD_TEMP_ID, LOG_STATUS_WARNING, TEMP_EVENT_HIGH_TEMP, 0, 0); // ( UINT8 * ) &temp_critical_event );
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_VOL_HIGH_CRITICAL )
{
  charge_ctrl_later(TURN_OFF);
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_VOL_HIGH_WARNNING )
{
  charge_ctrl_later(TURN_OFF);
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_VOL_LOW_CRITICAL )
{
  log_write(LOG_MOD_VOLTAGE_ID, LOG_STATUS_CRITICAL, VOL_EVENT_LOW_VOL, 0, 0); //4, ( UINT8 * ) &vol_critical_low_event );
  discharge_ctrl_later(TURN_OFF);
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_VOL_LOW_WARNNING )
{
  log_write(LOG_MOD_VOLTAGE_ID, LOG_STATUS_WARNING, VOL_EVENT_LOW_VOL, 0, 0); // 4, ( UINT8 * ) &vol_warning_low_event );
  bms_undervol_ctrl(TURN_ON);
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_OVERCURRENT )
{
//  Cevent *pthis = (Cevent *) argv[0];

  discharge_ctrl_later(TURN_OFF);
  charge_ctrl_later(TURN_OFF);
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_SOC_LOW_WARNING )
{
  mprintf("������;��棡����\r\n");
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_SOC_LOW_CRITICAL )
{
  mprintf("�������ع�;���?!!!\r\n");
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_FAN_STOP )
{
  cool1_ctrl_later(TURN_OFF);
  cool2_ctrl_later(TURN_OFF);
  bms_overtemp_ctrl(TURN_OFF);
  log_write(LOG_MOD_TEMP_ID, LOG_STATUS_OK, TEMP_EVENT_OK, 0, NULL);
  event_set_state(EVENT_FAN_STOP, EVENT_STOP);
  return 0;
}
;
DECLARE_EVENT_PROCESS ( EVENT_HEATPANNAL_STOP )
{
  heat_ctrl_later(TURN_OFF);
  log_write(LOG_MOD_TEMP_ID, LOG_STATUS_OK, TEMP_EVENT_OK, 0, NULL);
  event_set_state(EVENT_HEATPANNAL_STOP, EVENT_STOP);
  return 0;
}
;

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
