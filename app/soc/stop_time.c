
/*********************************************************
*
* 晟芯源微电子有限公司(UCT)
*
* File Description
*
*   $Revision: 2057 $
*   $Author: huhai $
*   $Date: 2014-07-21 14:24:25 +0800  $
*
*********************************************************/

#include "stop_time.h"
#include "timer.h"
#include "current.h"
#include "../banlance_battery/banlancebattery.h"
#include "../ctl/control.h"
#define STOP_CURRENT_THRESHOLD 0

static UINT32 g_stop_time_s = 0;
static UINT32 g_stop_time_last = 0;

/*********************************************************
 * 当所有开关关闭时(均衡开关和总开关)
 * 开始开始计算停车时间
 *********************************************************/
UINT32 get_stop_time(void)
{
    UINT32 tnow = 0;
    UINT8 i = 0, sw_flag = 0;
    const struct balance_t *gb = balance_read();
    for(i=0; i<TOTAL_VOLTAGE_NUM; i++)
    {
      if( gb->flags[i] == 0 )
      {
           sw_flag = 1;
      }        
    }
    
    if( sw_flag == 0 && GetBatteryMode() == 0 )
    {
        tnow = GetSysTimeBySec();
        g_stop_time_s += ( TIME_TRHESHOLD_CAL(tnow, g_stop_time_last) );
        g_stop_time_last = tnow; 
    }
    else
    {
        g_stop_time_s = 0;
    }                     
    
    return g_stop_time_s;
}