/*********************************************************
*
* ��оԴ΢�������޹�˾(UCT)
*
* File Description
*
*   $Revision: 2342 $
*   $Author: huhai $
*   $Date: 2014-07-29 09:49:28 +0800  $
*   mgctimer.c
*   this module is a soft timer 
*   ver 1.0
*   by mgc 20140730
*
*********************************************************/
#include "mgclib\mgctimer.h"
static struct timer g_timers[MAX_TIMERS];  //秒级定时�?
static struct timer g_timerms[MAX_TIMERMS];  //毫秒级定时器

/*********************************
 * name:timer_init()
 * init the timer stack 
 * you should call this before intterupt
 * by mgc 20140729
 *********************************/
int mgctimer_init(void)
{
    memset( g_timers, 0, sizeof(g_timers));
    memset(g_timerms, 0, sizeof(g_timerms));
    return 0;
}

/*********************************
 * timer_get
 * request a timer from timer stack, if success return >=0 else return < 0
 *  param: 
 *   p: the time interval that  timer trigger
 *   proc: the process function of this timer
 *   presision: if PRECISION_APP, the function will process at main loop,  else the function will process at timer isr
 * by mgc 20140729
 *********************************/

INT16 timer_get ( struct timerspec *p, timer_process proc , UINT8 presision )
{
    int i = 0;
    struct timer *ptimer = g_timerms;

    if ( p->time_ms == 0 || p->time_s > 60 ) //一分钟以上的定�?没有毫秒级的保证
        ptimer = g_timers;

    for ( i = 0; i < MAX_TIMERS; i++ )
    {
        if ( ptimer[i].flag == TIMER_IDLE )
        {
             if ( ptimer != g_timers )
               ptimer[i].time_ms =  p->time_ms + p->time_s * 1000;
             else
               ptimer[i].time_s = p->time_s;
           
            ptimer[i].proc = proc;
            ptimer[i].flag = TIMER_STOPPED;
            ptimer[i].precision = presision;

            if ( ptimer == g_timers )
            {
              i = i | 0x80;
            }

            return i;
        }
    }

    return -1;
}
 /*****************************************************************************
  * timer_add
  * add a timer to a monitor event(start this timer)
  * you should use timer_get to get the timer first
  * by mgc 20140729
  *******************************************************************************/
INT16 timer_add( int timer )
{
    struct timer *ptimer = g_timerms;

    if ( ( timer & 0x80 ) != 0 )
    {
        ptimer = g_timers;
        timer &= 0x7f;
        ptimer[timer].next_running_time = GET_SYSTEM_TIME_S() + ptimer[timer].time_s;
    }
    else
    {
        ptimer[timer].next_running_time = GET_SYSTEM_TIME_MS() + ptimer[timer].time_ms;
    }

    ptimer[timer].flag = TIMER_WAITING;
    return 0;
}

INT16 timer_stop( int timer )
{
    struct timer *ptimer = g_timerms;
    
    if ( timer >= 128  )
    {
        ptimer = g_timers;
        timer &= 0x7f;
    }

    ptimer[timer].flag = TIMER_IDLE;
    
    return 0;
}

INT16 timer_delete( int timer )
{
    struct timer *ptimer = g_timerms;

    if ( timer & 0x80 != 0 )
    {
        ptimer = g_timers;
        timer &= 0x7f;
    }
    ptimer[timer].flag = TIMER_IDLE;
    
    return 0;
}
/*******************************************************
 * TIMER_ISR_MONITOR_S
 * this function must be called by timer isr
 * this function will monitor the g_timers array per second
 * and if the timer reach it's time, the func will set this timer to waiting status
 * and if the timer precisions set running at isr, the timer's proc will run, else 
 * the proc will run when timer_monitor running
 * by mgc 20140730
 *******************************************************/
INT16 TIMER_ISR_MONITOR_S ( void )
{
    UINT8 i = 0;
    INT16 ret = 0;
    for ( i = 0; i < MAX_TIMERS; i++ )
    {
        if ( GET_SYSTEM_TIME_S() >= g_timers[i].next_running_time
                && g_timers[i].flag == TIMER_WAITING )
        {
            g_timers[i].flag = TIMER_RUNNING;

            if ( g_timers[i].precision == PRECISION_ISR )
            {
                if ( g_timers[i].proc )
                {
                    ret = g_timers[i].proc ( 0, 0 );
                }

                timer_add ( i | 0x80 );
            }
        }
    }

    return ret;
}
/*******************************************************
 * TIMER_ISR_MONITOR_S
 * this function must be called by timer isr
 * this func will monitor the g_timerms array per millisecond
 * by mgc 20140730
 *******************************************************/
INT16 TIMER_ISR_MONITOR_MS ( void )
{
    UINT8 i = 0;
    INT16  ret = 0;
    (void)ret;
    for ( i = 0; i < MAX_TIMERMS; i++ )
    {
        if ( GET_SYSTEM_TIME_MS() >= g_timerms[i].next_running_time
                && g_timerms[i].flag == TIMER_WAITING )
        {
            g_timerms[i].flag = TIMER_RUNNING;

            if ( g_timerms[i].precision == PRECISION_ISR )
            {
                if ( g_timerms[i].proc )
                {
                    ret = g_timerms[i].proc ( 0, 0 );
                }

                timer_add ( i );
            }
        }
    }

    return 0;
}
/*******************************************************
 * timer_monitor
 * this func should be call at main loop
 * by mgc 20140730
 *******************************************************/
INT16 timer_monitor ( void )
{
    INT16 ret = 0;
    UINT8 i = 0;
    for ( i = 0; i < MAX_TIMERS; i++ )
    {
        if ( g_timers[i].flag == TIMER_RUNNING )
        {
            if ( g_timers[i].proc )
            {
                ret = g_timers[i].proc ( 0, 0 );
            }

            timer_add ( i | 0x80 );
        }

    }

    for ( i = 0; i < MAX_TIMERMS; i++ )
    {
        if ( g_timerms[i].flag == TIMER_RUNNING )
        {
            if ( g_timerms[i].proc )
            {
                ret = g_timerms[i].proc ( 0, 0 );
            }

            timer_add ( i );
        }
    }

    return ret;
}

/*********************************************
 * reset this timer
 *********************************************/
INT16 timer_reset( int timer )
{
       
    struct timer *ptimer = g_timerms;
    //ptimer[timer].flag = TIMER_WAITING;
    if ( timer & 0x80 != 0 )
    {
        ptimer = g_timers;
        timer &= 0x7f;
        ptimer[timer].next_running_time = GET_SYSTEM_TIME_S() + ptimer[timer].time_s;
    }
    else
    {
        ptimer[timer].next_running_time = GET_SYSTEM_TIME_MS() + ptimer[timer].time_ms;
    }     
    return 0;
}

/*********************************************
 * timer test app
 *********************************************/
 
 INT16 timer_printf_system = -1; 
 
 void test_fun(void)
 {
          static UINT8 i = 0;
          i++;
          printf_sys_time();
          if( i> 10 )   
          {  
              timer_delete(  timer_printf_system );
              timer_printf_system = -1;
          }
 }
 
void test_timer(void)
{ 
   struct timerspec ts;
  	ts.time_ms = 0;
	ts.time_s = 10;
	timer_printf_system = timer_get( &ts, (timer_process)test_fun, 0 );
	if(    timer_printf_system < 0  )
	{
	    //err process 
	    return;
	}
	
	if(  timer_add(timer_printf_system) <0 )
	{
	      //err process
	}

}


/**********************************************************************
a better timer
add by mgc 20140730
not for test

static UINT8 g_max_monitor_index = 0;
static UINT8 g_monitor_timer_index[MAX_TIMERS];
void insert_monitor_timer( UINT8 monitor_index, UINT8 timer )
{
      UINT8 i = 0;
      for( i = g_max_monitor_index; i>=monitor_index; i-- )
      {
             g_timers[g_monitor_timer_index[i]].time_count -= g_timers[timer].time_count;
             g_monitor_timer_index[i+1] = g_monitor_timer_index[i];
             g_max_monitor_index ++ ;
      }
      g_monitor_timer_index[i] = timer;
}

INT16 timer_add2 ( UINT8 timer )
{
    UINT8 i = 0;
    UINT32 t = 0;
    for( i = 0; i<g_max_monitor_index; i++)
    {
         t+= g_timers[g_monit_timer_index[i]].count;
         if( g_timers[timer].time_ms < t )
         {
             t -= g_timers[g_monit_timer_index[i]].count;
             g_timers[timer].time_count = g_timers[timer].time_ms - t;
             insert_monitor_timer( i, timer ); 
             break;
         }
    }
    return 0;
}

void timer_monitor2 ( void )
{
     UINT8 i = 0;
     UINT8 index0 =  g_monitor_timer_index[0];
     g_timers[index0].count -- ;
     if( g_timers[index0].count == 0 )
     {
           for( i = 0; i<g_max_monitor_index-1; i++)
           {
                 g_monit_timer_index[i] = g_monitor_timer_index[i+1];
           }
           g_monit_timer_index[g_max_monitor-1] = -1;
           g_max_monitor--;
           if ( g_timers[index0].precision == PRECISION_ISR )
           {
                if ( g_timers[index0].proc )
                {
                    ret = g_timers[index0].proc ( 0, 0 );
                }
                timer_add2 ( index0 );
            }
           
     }
}
*************************************************************************************/
