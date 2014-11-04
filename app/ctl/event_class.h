/*********************************************************
 *
 * ��оԴ΢�������޹�˾(UCT)
 *
 * File Description
 *
 *   $Revision: 1813 $
 *   $Author: huhai $
 *   $Date: 2014-06-12 11:11:58 +0800 (星期�? 12 六月 2014) $
 *
 *********************************************************/
#ifndef _EVENT_CLASS_H_
#define _EVENT_CLASS_H_

#include "public/public.h"
#include "ctl/control.h"

typedef enum e_event_e
{
  EVENT_IDLE = 0, EVENT_START, EVENT_TRIGGER, EVENT_STOP
} event_e;

typedef enum cp_type
{
  SMALL = '<', BIGGER = '>', EQUALL = '=', SMALL_OR_EQUAL, BIGGER_OR_EQUAL,
} compare_type_e;

struct event_trigger
{
  enum cp_type compare_type; // '>' '<'
  UINT32 compare_value;
};

typedef struct event_class
{
  const char *event_name;
//    event_trigger *ptrigger_condition;
  UCHAR event_id;
  event_e e_state;
  event_e running_state;
  INT16
  (*event_process)(UCHAR argc, const UCHAR *argv[]);
} Cevent;

enum event_id
{
  EVENT_TEMP_LOW_CRITICAL,
  EVENT_TEMP_LOW_WARNNING,
  EVENT_TEMP_HIGH_CRITICAL,
  EVENT_TEMP_HIGH_WARNNING,
  EVENT_VOL_HIGH_CRITICAL,
  EVENT_VOL_HIGH_WARNNING,
  EVENT_VOL_LOW_CRITICAL,
  EVENT_VOL_LOW_WARNNING,
  EVENT_OVERCURRENT,
  EVENT_SOC_LOW_WARNING,
  EVENT_SOC_LOW_CRITICAL,
  EVENT_FAN_STOP,
  EVENT_HEATPANNAL_STOP
};

#define REGISTER_EVENT( EVENTID  ) {\
        #EVENTID,\
        EVENTID,\
        EVENT_IDLE,\
        EVENT_IDLE,\
        FUNC_##EVENTID\
    }

#define DECLARE_EVENT_PROCESS( EVENTID ) INT16 FUNC_##EVENTID( UCHAR argc,const UCHAR *argv[] )

DECLARE_EVENT_PROCESS( EVENT_TEMP_LOW_CRITICAL);
DECLARE_EVENT_PROCESS( EVENT_TEMP_LOW_WARNNING);
DECLARE_EVENT_PROCESS( EVENT_TEMP_HIGH_CRITICAL);
DECLARE_EVENT_PROCESS( EVENT_TEMP_HIGH_WARNNING);
DECLARE_EVENT_PROCESS( EVENT_VOL_HIGH_CRITICAL);
DECLARE_EVENT_PROCESS( EVENT_VOL_HIGH_WARNNING);
DECLARE_EVENT_PROCESS( EVENT_VOL_LOW_CRITICAL);
DECLARE_EVENT_PROCESS( EVENT_VOL_LOW_WARNNING);
DECLARE_EVENT_PROCESS( EVENT_OVERCURRENT);
DECLARE_EVENT_PROCESS( EVENT_SOC_LOW_WARNING);
DECLARE_EVENT_PROCESS( EVENT_SOC_LOW_CRITICAL);
DECLARE_EVENT_PROCESS( EVENT_FAN_STOP);
DECLARE_EVENT_PROCESS( EVENT_HEATPANNAL_STOP);

/****************************************************************************************************************
 * you can control the priority of these events by adjust the odder of the elements in this collection
 * by mgc 2014-0725
 ****************************************************************************************************************/
#define EVENTS_COLLETION   \
    REGISTER_EVENT ( EVENT_TEMP_LOW_CRITICAL )
/*,        \
    REGISTER_EVENT ( EVENT_TEMP_LOW_WARNNING ),   \
    REGISTER_EVENT ( EVENT_TEMP_HIGH_CRITICAL ),  \
    REGISTER_EVENT ( EVENT_TEMP_HIGH_WARNNING ),  \
    REGISTER_EVENT ( EVENT_VOL_LOW_CRITICAL ),    \
    REGISTER_EVENT ( EVENT_VOL_LOW_WARNNING ),    \
    REGISTER_EVENT ( EVENT_VOL_HIGH_CRITICAL ),             \
    REGISTER_EVENT ( EVENT_VOL_HIGH_WARNNING ),      \
    REGISTER_EVENT ( EVENT_OVERCURRENT ),              \
    REGISTER_EVENT ( EVENT_SOC_LOW_WARNING ),   \
    REGISTER_EVENT ( EVENT_SOC_LOW_CRITICAL ), \
    REGISTER_EVENT ( EVENT_FAN_STOP ),   \
    REGISTER_EVENT ( EVENT_HEATPANNAL_STOP )
 */

//INT16 event_add( struct event_class event );
INT16
event_set_state(enum event_id xevent, event_e state);
//INT16 event_delete( UINT8 xevent );

INT16
event_monitor(void);

#endif

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
