
/*********************************************************
 *
 * ��оԴ΢�������޹�˾(UCT)
 *
 * File Description
 *
 *   $Revision: 2342 $
 *   $Author: huhai $
 *   $Date: 2014-07-29 09:49:28 +0800 $
 *
 *********************************************************/

#ifndef _MGC_TIMER_H_
#define _MGC_TIMER_H_

#ifdef _USE_CODEWARRIOR
#include "public.h"
#include "timer.h"
#endif
#ifdef _USE_IAR
#include "public\public.h"
#include "timer\timer.h"
#endif

#define MAX_TIMERS 5
#define MAX_TIMERMS 3

struct timerspec
{
  UINT16 time_ms;
  UINT16 time_s;
};

typedef INT16
(*timer_process)(int argc, char**argv);

struct timer
{
  UINT8 volatile flag;
  UINT16 time_ms;
  UINT16 time_s;
  timer_process proc;
  UINT64 next_running_time;
  UINT8 precision;
};

enum _timer_state
{
  TIMER_IDLE = 0, TIMER_STOPPED, TIMER_WAITING, TIMER_RUNNING
};

enum _timer_precision
{
  PRECISION_APP, // 主循环的层次  处理在主循环线程�?    
  PRECISION_ISR, //达到ISR的精�? 处理将在中断中跑
};

#define GET_SYSTEM_TIME_S()   GetSysTimeBySec()
#define GET_SYSTEM_TIME_MS()   GetSysTimeByMsec()

/*these functions should be called by timer isr*/
INT16
TIMER_ISR_MONITOR_MS(void); //you should call this func per millisecond in isr
INT16
TIMER_ISR_MONITOR_S(void); // you should call this func per second in isr

/*below is for user*/
int
mgctimer_init(void); // call this before interrupt open
INT16
timer_get(struct timerspec *p, timer_process proc, UINT8 presision);
INT16
timer_add(int timer); // if <0 failed
#define timer_start timer_add
//INT16 timer_start( int timer );  //start a timer, equal timer add
INT16
timer_delete(int timer);
INT16
timer_stop(int timer);
INT16
timer_reset(int timer); //reset this timer
INT16
timer_monitor(void); //you should call this function in a main loop
void
test_timer(void);

#endif

