/*********************************************************
 *
 * ºþÄÏÊ¡•NöÎÔ´ëŠ×Ó¿Æ¼¼ÓÐÏÞØŸÈÎ¹«Ë¾(UCT)
 *
 * File Description
 *
 *   $Revision: 3405 $
 *   $Author: huhai $
 *   $Date: 2014-09-01 17:02:40 +0800  $
 *
 *********************************************************/

#include "string.h"
#include "public/public.h"
#include "voltage/voltage.h"
#include "log/log.h"
#include "mgclib/mgcbytesorder.h"

extern UINT16 getVoltageById(UINT8 id);


/*types */
enum err_level
{
	err_level_none, err_level1, err_level2
};


struct cell_voltage_threashold
{
	UINT16 vol_low;
	UINT16 vol_low_level1;
	UINT16 vol_low_level2;
	UINT16 vol_high;
	UINT16 vol_high_level1;
	UINT16 vol_high_level2;
};
struct sum_voltage_threashold
{
	UINT32 vol_low;
	UINT32 vol_low_level1;
	UINT32 vol_low_level2;
	UINT32 vol_high;
	UINT32 vol_high_level1;
	UINT32 vol_high_level2;
};

struct voltage_cell_diff_threshold 
{
	UINT16 cell_diff_danger1;
	UINT16 cell_diff_danger2;
};

struct voltage_const_vars
{
	struct cell_voltage_threashold cell_vol_value;
	struct sum_voltage_threashold sum_vol_value;
	struct voltage_cell_diff_threshold cell_diff_value;
};


struct state_cell_voltage
{
	int cell[TOTAL_VOLTAGES];
};

struct state_voltage
{
	struct state_cell_voltage cell_vol_state;  //each cell state
	int sum_vol_state; // the sum of voltage state
	int cell_vol_diff_state; //the state of difference of voltages 
	int voltage_state; //each bit show the sum state of this battery as type vol state
};

struct voltage_vars
{
	UINT16 g_battery_singlevol_max;
	UINT16 g_battery_singlevol_min;
	UINT32 g_battery_vol_sum;
	/*single cell voltage state*/
	struct state_voltage g_volatage_state;
//  UINT16 g_sleep_voltage_allowed ;
//  UINT32 g_ov_status ;
//  UINT32 g_uv_status ;
//  UINT32 timer_OV[TOTAL_VOLTAGES];
//  UINT32 timer_UV[TOTAL_VOLTAGES];
//  UINT16 g_UCT_voltage[TOTAL_VOLTAGES] ;
//  UINT16 g_simulator_vols[TOTAL_VOLTAGES] ;
};

/****************************gloable varables**************************/
static const struct voltage_const_vars g_const_vol_value;
static struct voltage_vars g_voltage_vars;

//
//UINT32 getVoltageStatusOV( void )
//{
//	return (g_ov_status);
//}
//
//UINT32 getVoltageStatusUV(void)
//{
//	return (g_uv_status);
//}
//
//int set_simulator_vol(const UINT16 *pvols, UINT8 n_batterys)
//{
//	UINT8 i = 0;
//	for (i = 0; i < n_batterys; i++)
//	{
//		g_simulator_vols[i] = BYTESORDER_S2H_16((UCHAR *) pvols + i * 2);
//	}
//
//	//memcpy( g_simulator_vols, pvols, n_batterys*sizeof(UINT16) );
//	return (0);
//}
//
//void get_voltage(UINT16 *vol)
//{
//	(void) memcpy(vol, g_UCT_voltage, TOTAL_VOLTAGES * sizeof(UINT16));
//}
//
//UINT16 getVoltageByIndex(UINT8 index)
//{
//	if (index > TOTAL_VOLTAGES)
//		index = TOTAL_VOLTAGES - 1;
//	return (g_UCT_voltage[index]);
//}
//
//UINT16 setVoltageByIndex( UINT16 index, UINT16 voltage)
//{
//	if (index > TOTAL_VOLTAGES)
//		index = TOTAL_VOLTAGES - 1;
//	g_UCT_voltage[index] = voltage;
//	return 0;
//}

UINT32 BMS_GetVoltageValueBatteryTotal(void)
{
	return g_voltage_vars.g_battery_vol_sum;
//	return (g_battery_singlevol_sum);
}

TYPE_VOLTAGE BMS_GetVoltageValueMinCell(void)
{
	return g_voltage_vars.g_battery_singlevol_min;
//	return (g_battery_singlevol_min);
}
TYPE_VOLTAGE BMS_GetVoltageValueMaxCell(void)
{
	return g_voltage_vars.g_battery_singlevol_max;
//	return (g_battery_singlevol_max);
}
/*
 * get the BMS batteries voltage state
 * state:
	bit0:STATE_VOL_NORMAL = 0,
		STATE_CELL_VOL_LOW,
		STATE_CELL_VOL_LOW_LEVEL1,
		STATE_CELL_VOL_LOW_LEVEL2,
		STATE_CELL_VOL_HIGH,
	bit5:STATE_CELL_VOL_HIGH_LEVEL1 = 5,
	bit6:STATE_CELL_VOL_HIGH_LEVEL2,
		STATE_SUM_VOL_LOW,
		STATE_SUM_VOL_LOW_LEVEL1,
		STATE_SUM_VOL_LOW_LEVEL2,
		STATE_SUM_VOL_HIGH, // =10
	bit11:STATE_SUM_VOL_HIGH_LEVEL1 = 11,
		STATE_SUM_VOL_HIGH_LEVEL2,
		STATE_VOL_CELL_DIFF_BIG_LEVEL1,
		STATE_VOL_CELL_DIFF_BIG_LEVER2,
 * by mgc-2014-10-31
 * */
int getVoltageState(void)
{
	return g_voltage_vars.g_volatage_state.voltage_state;
}
//
//UINT8 getVoltageStatusSleepCondition(void)
//{
//	return (g_sleep_voltage_allowed);
//}

/*
 * in: cell voltage
 * out: state_voltage
 * get cell voltage state
 * by mgc 20141030
 * */
static int checkCellVoltageState(UINT16 cell_voltage)
{
	int ret = 0;
	if (cell_voltage < g_const_vol_value.cell_vol_value.vol_low_level1)
	{
		//todo
		ret = STATE_CELL_VOL_LOW_LEVEL1;
	}
	else if (cell_voltage < g_const_vol_value.cell_vol_value.vol_low_level2)
	{
		ret = STATE_CELL_VOL_LOW_LEVEL2;
	}
	else if (cell_voltage < g_const_vol_value.cell_vol_value.vol_low)
	{
		ret = STATE_CELL_VOL_LOW;
	}

	if (cell_voltage > g_const_vol_value.cell_vol_value.vol_high_level1)
	{
		ret = STATE_CELL_VOL_HIGH_LEVEL1;
	}
	else if (cell_voltage > g_const_vol_value.cell_vol_value.vol_high_level2)
	{
		ret = STATE_CELL_VOL_HIGH_LEVEL2;
	}
	else if (cell_voltage > g_const_vol_value.cell_vol_value.vol_high)
	{
		ret = STATE_CELL_VOL_HIGH;
	}
	return ret;
}
/*
 * get the state of sum voltage
 * by mgc 20141030
 * */
static int checkSumVoltageState(UINT32 sum_voltage)
{
	int ret = 0;
	if (sum_voltage < g_const_vol_value.sum_vol_value.vol_low_level1)
	{
		//todo
		ret = STATE_SUM_VOL_LOW_LEVEL1;
	}
	else if (sum_voltage < g_const_vol_value.sum_vol_value.vol_low_level2)
	{
		ret = STATE_SUM_VOL_LOW_LEVEL2;
	}
	else if (sum_voltage < g_const_vol_value.sum_vol_value.vol_low)
	{
		ret = STATE_SUM_VOL_LOW;
	}

	if (sum_voltage > g_const_vol_value.sum_vol_value.vol_high_level1)
	{
		ret = STATE_SUM_VOL_HIGH_LEVEL1;
	}
	else if (sum_voltage > g_const_vol_value.sum_vol_value.vol_high_level2)
	{
		ret = STATE_SUM_VOL_HIGH_LEVEL2;
	}
	else if (sum_voltage > g_const_vol_value.sum_vol_value.vol_high)
	{
		ret = STATE_SUM_VOL_HIGH;
	}
	return ret;
}

static int checkVoltageDiffState(UINT16 min_vol, UINT16 max_vol)
{
	int ret = STATE_VOL_NORMAL;
	UINT16 diff = max_vol-min_vol ;
	if( diff > g_const_vol_value.cell_diff_value.cell_diff_danger1 )
	{
		ret = STATE_VOL_CELL_DIFF_BIG_LEVEL1;
	}
	else if(diff > g_const_vol_value.cell_diff_value.cell_diff_danger2)
	{
		ret = STATE_VOL_CELL_DIFF_BIG_LEVER2;
	}
	return ret;
}
/*
 * update for ltc68042 object
 * if it read new voltages from chips this update should be executed
 * by mgc 20141030
 * */
int observerVoltageUpdate(void)
{
	int rv = 0;
	UINT8 i = 0, j = 0, k = 0, temp_sleep = 0;
	UINT16 voltage = 0;
	UINT32 temp_sum = 0;
	struct state_voltage *pvstate = NULL;
	int state;
	int t_vol_state = 0;
	UINT8 flag_cell_state_change = 0;
#ifdef _USE_BM03
	bq536_volt_read(g_UCT_voltage);
	/*abandon the invalid cells
	 for( i=0; i<NUMBER_OF_BQ_DEVICES; i++)
	 {
	 for( j=0; j<NUMBER_OF_CELLS_OF_PACK[i]; j++ )
	 {
	 g_UCT_voltage[k++] = g_UCT_voltage[(i+1)*j];
	 }
	 }*/
#endif

#ifdef _USE_BM01
	for ( ic_num = 0; ic_num < LTC6804_IC_NUM; ic_num ++ )
	{
		rv = ltc6804_get_voltage ( ic_num, buff );

		if ( rv == UCT_SUCCESS )
		{
			( void ) memcpy ( &g_UCT_voltage[ic_num * LTC6804_MAX_VOL], buff, LTC6804_MAX_VOL * 2 );
		}
		else
		{
			//ï¿½ì³£ï¿½ï¿½ï¿½ï¿½
			//mprintf("ï¿½ï¿½Ñ¹ï¿½É¼ï¿½ï¿½ï¿½ï¿½ï¿½!\r\n");
			//rv = UCT_ERR_OPT_FAIL;
			continue;
		}
	}
#endif
	/*the next simulator should move to driver layer*/
//	i = get_simulator_mode();
//	if ((i & MASK_BIT_SIMULATOR_MODE_VOL) == 0)
	{
//		memcpy(g_UCT_voltage, g_simulator_vols,
//				TOTAL_VOLTAGES * sizeof(TYPE_VOLTAGE));
	}

	g_voltage_vars.g_battery_singlevol_max = g_voltage_vars.g_battery_singlevol_min = getVoltageById(0);
	pvstate = &g_voltage_vars.g_volatage_state ;

	
	for (i = 0; i < TOTAL_VOLTAGES; i++)
	{
		voltage = getVoltageById(i);

		if (g_voltage_vars.g_battery_singlevol_max < voltage)
		{
			g_voltage_vars.g_battery_singlevol_max = voltage;
		}

		if ( g_voltage_vars.g_battery_singlevol_min > voltage)
		{
			 g_voltage_vars.g_battery_singlevol_min = voltage;
		}

		/*cell voltage err state check*/
		state = checkCellVoltageState(voltage);
		if (pvstate->cell_vol_state.cell[i] != state)
		{
			pvstate->cell_vol_state.cell[i] = state;
			MGC_SET_BIT(t_vol_state, 1<<state);
			flag_cell_state_change = 1;
			//todo
		}
//		if (voltage > VALUE_TH_OV)
//		{
//			timer_OV[i]++;
//			if (timer_OV[i] > VALUE_TH_TIME_FILTER_OV)
//				MGC_SET_BIT( g_ov_status, i);
//		}
//		else if (voltage < VALUE_TH_OV_RECOVER)
//		{
//			MGC_RESET_BIT( g_ov_status, i);
//			timer_OV[i] = 0;
//		}
//
//		if (voltage < VALUE_TH_UV)
//		{
//			timer_UV[i]++;
//			if (timer_UV[i] > VALUE_TH_TIME_FILTER_UV)
//				MGC_SET_BIT( g_uv_status, i);
//		}
//		else if (voltage > VALUE_TH_UV_RECOVER)
//		{
//			MGC_RESET_BIT( g_uv_status, i);
//			timer_UV[i] = 0;
//		}

		temp_sum += voltage;
	}
	/*observe object1 cell voltage state*/
	if (flag_cell_state_change)
	{
		//todo update()
	}
	
	/*observe object2 sum voltage state */
	state = checkSumVoltageState(temp_sum);
	if (state !=  pvstate->sum_vol_state)
	{
		pvstate->sum_vol_state = state;
		MGC_SET_BIT(t_vol_state, 1<<state);
		//todo update()
	}
	
	/*observer object3 voltage differences state*/
	state  = checkVoltageDiffState(g_voltage_vars.g_battery_singlevol_max, g_voltage_vars.g_battery_singlevol_min);
	if(state  != pvstate->cell_vol_diff_state)
	{
		pvstate->cell_vol_diff_state = state ;
		MGC_SET_BIT(t_vol_state, 1<<state);
		//todo update()
	}
	
	
//	if (temp_sleep >= TOTAL_VOLTAGES || getVoltageStatusUV() != 0)
//	{
//		g_sleep_voltage_allowed = 1;
//	}
//	else
//	{
//		g_sleep_voltage_allowed = 0;
//	}

	g_voltage_vars.g_battery_vol_sum = temp_sum;
	g_voltage_vars.g_volatage_state.voltage_state = t_vol_state;
	return (rv);

}

/*
 * check the voltage state  by state as ttype_state_voltage
 * in : state 		type_state_voltage
 * out: 
 * return 0 none, 1:yes
 * */
int BMS_GetErrStatusVoltageByType( UINT8 types)
{
	assert(types<STATE_VOL_TOTAL);
	if( types == 0 )
		return (getVoltageState()==0);
	if( ((1<<types) & getVoltageState()) == 0)
		return 0;
	return 1;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
