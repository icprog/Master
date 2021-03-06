#ifndef _TIMER_H_
#define _TIMER_H_

enum timer_state
{
	RUNNING, STOP
};

typedef void (*type_isr_pit)(void);
/************************************************************/
void initPIT_RTI(void);
UINT64 getSystemTimeMs(void);
UINT64 getSystemTimeS(void);
int test_timer_RTI(void);
int test_timer_pit(void);
int timer_add(int id, type_isr_pit isr_func, UINT32 timems_interval);
int timer_stop(int id);
int timer_continue(int id);
void enableIrq(void);

void delay_us(UINT32 us);
void delay_ms(UINT32 ms);
void delay_s(UINT32 s);

static UINT32
TIME_TRHESHOLD_CAL(UINT32 tnow, UINT32 tlast)
{

  return (
      (tnow) >= (tlast) ? ((tnow) - (tlast)) : (0xFFFFFFFF - (tnow) + (tlast)));
}
#endif
