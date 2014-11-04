#ifndef _SYSTEM_STATE_H_
#define _SYSTEM_STATE_H_

int getSystemState(void);

struct s_state_check
{
	int state;
	int (*update)(void);
	int (*notify)(void);
	int (*getState)(void);
};

enum type_soc_state
{
	STATE_SOC_LOW,
	STATE_SOC_LOW_LEVER1
};

#endif

