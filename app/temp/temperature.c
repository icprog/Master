/*********************************************************
 *
 * (UCT)
 *
 * File Description
 *
 *   $Revision: 3405 $
 *   $Author: huhai $
 *   $Date: 2014-09-01 17:02:40 +0800 (周一, 01 九月 2014) $
 *
 *********************************************************/
#include "temp/temperature.h"
#include "string.h"
#include "public/public.h"

extern INT16 getTemperatureById( UINT8 id );

struct cell_temperature_threashold
{
	INT16 temp_low;
	INT16 temp_low_level1;
	INT16 temp_low_level2;
	INT16 temp_high;
	INT16 temp_high_level1;
	INT16 temp_high_level2;
};

struct temperature_cell_diff_threshold
{
	UINT16 temp_diff_danger1;
	UINT16 temp_diff_danger2;
};

struct temperature_const_vars
{
	struct cell_temperature_threashold cell_temp_value;
	struct temperature_cell_diff_threshold cell_diff_value;
};

struct state_temperature
{
	int state[TOTAL_TEMPERATURE_SENSORS_NUM];
	int bitState;  //each bit indicate the id of temperatures err or ok
	UINT8 sumState;  // each bit indicate the type of  type_state_temperature 
};

struct temperature_obj
{
	INT16 temp;
	UINT8 id;
};

struct temperature_vars
{
	struct state_temperature temp_state;
	struct temperature_obj temp_max;
	struct temperature_obj temp_min;
	TYPE_TEMP TempAverageValue;
};

static struct temperature_vars g_temp_var;
static const struct temperature_const_vars g_const_temp_var;


int getTemperatureState(void)
{
	return g_temp_var.temp_state.bitState;
}

//TYPE_TEMP g_UCT_temperature[TOTAL_TEMP_NUM] = {0};
//TYPE_TEMP g_simulator_temperature[TOTAL_TEMP_NUM] = {0};
//
//int set_simulator_temp( const INT8 *ptemps, UINT8 n_temps)
//{
//	memcpy( g_simulator_temperature, ptemps, n_temps );
//	return 0;
//}
#ifdef _USE_BM01
static const UINT16 r_to_temp[TOTAL_TEMP_POINT_NUM] =
{	25561, 25343, 25119, 24888, 24650, 24406, 24156, 23898,
	23636, 23367, 23092, 22811, 22524, 22239, 21944, 21644,
	21340, 21031, 20718, 20401, 20082, 19758, 19432, 19103,
	18773, 18440, 18106, 17771, 17434, 17098, 16761, 16425,
	16089, 15755, 15421, 15100, 14759, 14431, 14105, 13782,
	13463, 13147, 12834, 12525, 12220, 11918, 11621, 11329,
	11040, 10758, 10478, 10205, 9937, 9673, 9414, 9162,
	8911, 8670, 8433, 8201, 7975, 7753, 7538, 7326,
	7120, 6921, 6725, 6534, 6349, 6125, 5994, 5822,
	5655, 5495, 5336, 5183, 5035, 4890, 4749, 4613,
	4480, 4353, 4228, 4105, 3989, 3875, 3762, 3655,
	3552, 3451, 3351
};     //-10��C ~ 80��C
#endif

//void get_temperature(TYPE_TEMP *temp)
//{
//// UINT8 i = 0;
////	(void) memcpy(temp, g_UCT_temperature, TOTAL_TEMP_NUM);
//	/*
//	 for( i=0; i< TOTAL_TEMP_NUM; i++ )
//	 {
//	 temp[i] = 59 + i;
//	 }
//	 */
//	//(void)memset(temp, 67, TOTAL_TEMP_NUM);
//}
//
//int setTemperatureByIndex(UINT8 index, TYPE_TEMP temperature)
//{
//	assert(index < TOTAL_CURRENTS);
////	g_UCT_temperature[index] = temperature;
//	return 0;
//}
//
//TYPE_TEMP getTemperatureByIndex(UINT8 index)
//{
////	if (index >= TOTAL_TEMP_NUM - 1)
////		index = TOTAL_TEMP_NUM - 1;
//////	return g_UCT_temperature[index];
//	return 0;
//}
/*
 * in: cell voltage
 * out: state_voltage
 * get cell voltage state
 * by mgc 20141030
 * */
static int getCellTemperatureState(int temperature)
{
	int ret;
	if (temperature < g_const_temp_var.cell_temp_value.temp_low_level1)
	{
		//todo
		ret = STATE_TEMP_LOW_LEVEL1;
	}
	else if (temperature < g_const_temp_var.cell_temp_value.temp_low_level2)
	{
		ret = STATE_TEMP_LOW_LEVEL2;
	}
	else if (temperature < g_const_temp_var.cell_temp_value.temp_low)
	{
		ret = STATE_TEMP_LOW;
	}

	if (temperature > g_const_temp_var.cell_temp_value.temp_high_level1)
	{
		ret = STATE_TEMP_HIGH_LEVEL1;
	}
	else if (temperature > g_const_temp_var.cell_temp_value.temp_high_level2)
	{
		ret = STATE_TEMP_HIGH_LEVEL2;
	}
	else if (temperature > g_const_temp_var.cell_temp_value.temp_high)
	{
		ret = STATE_TEMP_HIGH;
	}
	return ret;
}

int observerTemperatureUpdate(void)
{
	int rv = UCT_SUCCESS;
	int state;
	INT16 t_temperature;
	UINT8 temperature_sensor_id = 0;
	INT32 temp_sum = 0;
//	count = get_simulator_mode();
//	if ((count & MASK_BIT_SIMULATOR_MODE_TEMP) == 0)
//	{
//		memcpy(g_UCT_temperature, g_simulator_temperature, TOTAL_TEMP_NUM);
//	}

	for (temperature_sensor_id = 0; temperature_sensor_id < TOTAL_TEMPERATURE_SENSORS_NUM; temperature_sensor_id++)
	{
		t_temperature = getTemperatureById(temperature_sensor_id);
		temp_sum += t_temperature;
		if (g_temp_var.temp_max.temp < t_temperature)
		{
			g_temp_var.temp_max.temp = t_temperature;
			g_temp_var.temp_max.id = temperature_sensor_id;
		}

		if (g_temp_var.temp_min.temp > t_temperature)
		{
			g_temp_var.temp_min.temp = t_temperature;
			g_temp_var.temp_max.id = temperature_sensor_id;
		}

		state = getCellTemperatureState(t_temperature);
		if (state != g_temp_var.temp_state.state[temperature_sensor_id])
		{
			g_temp_var.temp_state.state[temperature_sensor_id] = state;
			//TODO
		}

	}
	g_temp_var.TempAverageValue = (TYPE_TEMP)(temp_sum/TOTAL_TEMPERATURE_SENSORS_NUM);

	return rv;
}

TYPE_TEMP BMS_GetTemperatureValueAverage(void)
{
	return g_temp_var.temp_min.temp;
}

TYPE_TEMP BMS_GetTemperaturValueMaxValue(void)
{
	return g_temp_var.temp_max.temp;
}

TYPE_TEMP BMS_GetTemperatureValueMin(void)
{
	return g_temp_var.TempAverageValue;
}

int BMS_GetTemperatureIdMaxValue(void)
{
	return g_temp_var.temp_max.id;
}

int BMS_GetTemperatureIdMinValue(void)
{
	return g_temp_var.temp_min.id;
}

/*
 * if have return 1 else 0 
 * */
int BMS_GetErrStatusTemperatureByType( UINT8 types)
{
	assert(types<STATE_TEMP_TOTAL);
	if( types == 0 )
		return (getTemperatureState()==0);
	if( ((1<<types) & getTemperatureState()) == 0)
		return 0;
	return 1;
}

