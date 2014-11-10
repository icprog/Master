/*********************************************************
 *
 * ��оԴ΢�������޹�˾(UCT)
 *
 * File Description
 *
 *   $Revision: 2057 $
 *   $Author: huhai $
 *   $Date: 2014-07-11 14:24:25 +0800 (周五, 11 七月 2014) $
 *
 *********************************************************/

#include "banlance_battery/banlancebattery.h"
#include "voltage/voltage.h"
#include "public/public.h"
#include "mgclib/mgctimer.h"
#include <string.h>

#define DEBUG_BANLANCE mprintf
#define DEGREE
#define MV_TO_100UV( mv )  (mv)*10

static UINT16 THREASHOLD_V_BANLANCE = MV_TO_100UV(20);
#ifdef _USE_BM01
static const UINT16 V_LOWE_LIMIT = MV_TO_100UV( 2500 );
#endif

static UINT8 n_open_balance = 0;
//static int timer_balance = -1;
//static const INT8 T0 = 45;
static struct balance_t gb;
//static UINT16 v[TOTAL_VOLTAGES];
//static INT8 t[TOTAL_VOLTAGES / 2];
static UINT16 v_temp[TOTAL_VOLTAGES];
static UINT8 n;
static const INT8 T_board = 50;

void set_threshold_v(UINT16 v)
{
	THREASHOLD_V_BANLANCE = v;
}

UINT16 get_threshold_v(void)
{
	return (THREASHOLD_V_BANLANCE);
}

/*********************************************************
 *
 *********************************************************/
//
//void balance_cal(void)
//{
//	static UINT8 flag = 0;
//	if (flag == 0)
//	{
//		if (timer_add(timer_balance) < 0)
//		{
//			//err process
//			return;
//		}
//		flag = 1;
//	}
//}
static UINT8 select_max_from_array(const UINT16 *array, UINT8 len)
{
	UINT8 i = 0, ret = 0;
	UINT16 temp = 0;
	temp = array[0];
	for (i = 1; i < len; i++)
	{
		if (temp < array[i])
		{
			temp = array[i];
			ret = i;
		}
	}
	return (ret);
}
/*
 * select n max of data from array, and store to outbuff
 * */
static UINT8 select_nMaxFromArray(UINT16 *array, UINT8 len, UINT8 *outbuff,
		UINT8 n)
{
	UINT8 i = 0, ret = 0;
	for (i = 0; i < n; i++)
	{
		ret = select_max_from_array(array, len - i);
		*outbuff++ = ret;
		array[ret] = 0;
	}
	return (0);
}

#if 1
void balance_cal(void)
{
	//UINT8 iMax = 0,
	UINT8 i = 0, j = 0, validBalance[TOTAL_VOLTAGES];
	if (n_open_balance == 5)
	{
		n_open_balance = 0;
		get_voltage(v_temp);
		n = 12 - 12 * T_board / 75;
		select_nMaxFromArray(v_temp, TOTAL_VOLTAGES, validBalance, n);
		memset(&gb, 0, sizeof(gb));

		for (i = 0; i < n; i++)
		{
			if ((getVoltageValueByCellIndex(validBalance[i]) - BMS_GetVoltageValueMin())
					> THREASHOLD_V_BANLANCE)
			{
				gb.flags[validBalance[i]] = 1;
			}
		}

		for (i = 0; i < TOTAL_VOLTAGES; i++)
		{
			if (gb.flags[i])
			{
				OPEN_BANLANCE_BY_LTC6804(i);
			}
			else
			{
				CLOSE_BANLANCE_BY_LTC6804(i);
			}
		}

		battery_blance_sw_get(gb.Bytebalances);
		mprintf("balance cal result:%02x%02x%02x\r\n", gb.Bytebalances[0],
				gb.Bytebalances[1], gb.Bytebalances[2]);
	}
	n_open_balance++;
}
#else
void balance_cal1(void)
{

	UINT8 iMax = 0, iMin = 0, i = 0;
	get_voltage((UINT16 *)&v);
	get_temperature((INT8 *)&t);
	t[ TOTAL_VOLTAGES/2 -1 ] = t[ TOTAL_VOLTAGES/2 -2] = t[ TOTAL_VOLTAGES/2 -3 ];

	iMax = iMin = 0;
	(void)iMax;

	/*open*/
	for( i=0; i< TOTAL_VOLTAGES; i++ )
	{
		//    if( v[i] > v[iMax] )
		//      iMax = i;

		if( v[i] < v[iMin] )
		iMin = i;
	}

	/*
	 if(v[iMax] - v[iMin] > THREASHOLD_V_BANLANCE )
	 {
	 if( t[iMax/2] > T0 &&  gb.flags[iMax] != 1 )
	 {
	 gb.flags[iMax] = 1;
	 OPEN_BANLANCE_BY_LTC6804(iMax);
	 DEBUG_BANLANCE("��⿪��?��%d�ڵ��?\r\n",i);
	 }
	 }
	 */

	/* open and close*/
	for( i=0; i< TOTAL_VOLTAGES; i++ )
	{
		if( gb.flags[i] == 1 )
		{
			// if( v[i] >= vol_critical_low_threshold_get() )
			{
				//do nothing
			}
			//else
			if( v[i] - v[iMin] <= THREASHOLD_V_BANLANCE )
			{
				gb.flags[i] = 0;
				DEBUG_BANLANCE("���ر�:%d�ڵ��?\r\n", i );
				CLOSE_BANLANCE_BY_LTC6804(i);
			}
		}
		else
		{
			if( v[i] - v[iMin] > THREASHOLD_V_BANLANCE )
			{
				if( v[iMin] > V_LOWE_LIMIT )
				{
					gb.flags[i] = 1;
					DEBUG_BANLANCE("balance %d open!\r\n", i );
					OPEN_BANLANCE_BY_LTC6804(i);
				}
			}
		}
	}
}
#endif

//int balance_timer_init(void)
//{
//	struct timerspec ts;
//	ts.time_ms = 0;
//	ts.time_s = 5;
//	timer_balance = timer_get(&ts, (timer_process) balance_cal1, 0);
//	return (timer_balance);
//}

struct balance_t * balance_read(void)
{
	return ((struct balance_t *) &gb);
}

UINT16 get_balance_current_by_index(UINT8 index)
{
	return ((UINT16) (getVoltageValueByCellIndex(index)/ V_2_MV(R_BANLANCE)));
}

/********************************************************
 * set balance sw by ptr[3]
 * by mgc 20140725
 ********************************************************/
static enum UCT_ERR_CODE battery_balance_sw_set(const UINT8 *balance_ptr)
{
	UINT8 i, j;
	enum UCT_ERR_CODE rv = UCT_SUCCESS;
	struct balance_t * const gb = balance_read();

	for (j = 0; j < 3; j++)
	{
		for (i = 0; i < 8; i++)
		{
			gb->flags[i + 8 * j] = ((balance_ptr[j] >> i) & 0x01);
		}
	}

	return (rv);
}

/********************************************************
 * read balance status to ptr
 * by mgc 20140725
 ********************************************************/
enum UCT_ERR_CODE battery_blance_sw_get(UINT8 *balance_ptr)
{
	UINT8 i, j;
	enum UCT_ERR_CODE rv = UCT_SUCCESS;
	const struct balance_t * gb = balance_read();

	for (j = 0; j < 3; j++)
	{
		balance_ptr[j] = 0;

		for (i = 0; i < 8; i++)
		{
			balance_ptr[j] |= gb->flags[i + 8 * j] << i;
		}
	}

	//balance_ptr[0] = 0;
	//balance_ptr[1] = 0;
	//balance_ptr[2] = 0;

	return (rv);
}

int balance_init(void)
{
	memset(&gb, 0, sizeof(struct balance_t));
	return (0);	// balance_timer_init();
}

#if 0

#pragma align off
union bit32_u
{
	UINT32 b32;
	struct bits
	{
		UINT32 b24 : 1;
		UINT32 b25 : 1;
		UINT32 b26 : 1;
		UINT32 b27 : 1;
		UINT32 b28 : 1;
		UINT32 b29 : 1;
		UINT32 b30 : 1;
		UINT32 b31 : 1;
		UINT32 b16 : 1;
		UINT32 b17 : 1;
		UINT32 b18 : 1;
		UINT32 b19 : 1;
		UINT32 b20 : 1;
		UINT32 b21: 1;
		UINT32 b22: 1;
		UINT32 b23: 1;
		UINT32 b8: 1;
		UINT32 b9:1;
		UINT32 b10: 1;
		UINT32 b11: 1;
		UINT32 b12: 1;
		UINT32 b13:1;
		UINT32 b14:1;
		UINT32 b15:1;
		UINT32 b0: 1;
		UINT32 b1: 1;
		UINT32 b2: 1;
		UINT32 b3: 1;
		UINT32 b4: 1;
		UINT32 b5: 1;
		UINT32 b6: 1;
		UINT32 b7:1;
	}bbit;
};

void balance_manager(void)
{

}

#pragma align off
union ba_u
{
	struct ba_t
	{
		UINT8 bank;
		UINT8 b[3];
	}balance_flags;
	struct _bits
	{
		UINT32 b0: 1;
		UINT32 b1: 1;
		UINT32 b2: 1;
		UINT32 b3: 1;
		UINT32 b4: 1;
		UINT32 b5: 1;
		UINT32 b6: 1;
		UINT32 b7:1;
		UINT32 b8: 1;
		UINT32 b9:1;
		UINT32 b10: 1;
		UINT32 b11: 1;
		UINT32 b12: 1;
		UINT32 b13:1;
		UINT32 b14:1;
		UINT32 b15:1;
		UINT32 b16 : 1;
		UINT32 b17 : 1;
		UINT32 b18 : 1;
		UINT32 b19 : 1;
		UINT32 b20 : 1;
		UINT32 b21: 1;
		UINT32 b22: 1;
		UINT32 b23: 1;
	}bbits;
};

void test_union(void)
{
	union bit32_u bu;
	// set_bit( &bu );
	bu.b32 = 0;
	bu.bbit.b1 = 1;
	bu.bbit.b2 = 1;
	bu.bbit.b4 = 1;

	bu.bbit.b11 = 1;
	bu.bbit.b12 = 0;
	bu.bbit.b21 = 1;
	bu.bbit.b31 = 1;
	bu.bbit.b17 = 1;

	mprintf("B32:%lX\r\n",bu.b32);
	/*
	 bu.b[0] = 0x12;
	 bu.b[1] = 0x34;
	 bu.b[2] = 0x56;
	 bu.b[3] = 0x78;

	 mprintf("b3[%d],b4%d],b5%d],b11[%d],b12[%d],b13[%d] , b21[%d],b23[%d],b24[%d]\r\n",
	 bu.bbit.b3, bu.bbit.b4, bu.bbit.b5,bu.bbit.b11, bu.bbit.b12, bu.bbit.b13,bu.bbit.b21, bu.bbit.b23, bu.bbit.b24);

	 mprintf("B32:%lX\r\n",bu.b32);
	 */
}
#endif

