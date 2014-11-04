/*********************************************************
 *
 *
 * File Description
 *
 *   $Revision: 3311 $
 *   $Author: huhai $
 *   $Date: 2014-08-28 10:01:04 +0800 ( 2014) $
 *
 *********************************************************/
#include "public/public.h"
#include "ctl/control.h"
#include "temp/temperature.h"
#include "voltage/voltage.h"
#include "current/current.h"
#include "gpio/gpio.h"
#include "timer/timer.h"
#include "log/log.h"
#include "protocol/cmd_class.h"
#include "ctl/event_class.h"
#include "banlance_battery/banlancebattery.h"
#include "EEpromManage/EepromManage.h"
#include "insulation_resistance/insulation_resistance.h"
#include "ctl/charge.h"
#include "public/systemState.h"
#include "public/err.h"

UINT16 temp_warning_low_event = 0;
UINT16 temp_warning_high_event = 0;
UINT16 temp_critical_event_low = 0;
UINT16 temp_critical_event_high = 0;
UINT16 temp_edge_event = 0;
UINT16 temp_cooldown_event = 0;
UINT16 temp_heat_event = 0;
UINT8 cool_event = 0;
UINT8 heat_event = 0;

UINT32 vol_warning_low_event = 0;
UINT32 vol_warning_high_event = 0;
UINT32 vol_critical_low_event = 0;
UINT32 vol_critical_high_event = 0;

UINT32 low_current_start_time = 0;

UINT8 control_mode = BMS_CONTROL_MODE_AUTO;

static UINT8 g_vol_state;
static UINT8 g_soc_state;
static UINT8 g_cur_state;
static UINT8 g_temp_state;

UINT16 temp_warning_low_event_get(void)
{
	return temp_warning_low_event;
}

UINT16 temp_warning_high_event_get(void)
{
	return temp_warning_high_event;
}

UINT16 temp_critical_event_low_get(void)
{
	return temp_critical_event_low;
}

UINT16 temp_critical_event_high_get(void)
{
	return temp_critical_event_high;
}

UINT16 temp_edge_event_get(void)
{
	return temp_edge_event;
}

/*protect status manager*/
static UINT32 g_protect_status = 0;

void set_protect_status(UINT32 status)
{
	g_protect_status |= status;
	//MGC_SET_BIT(g_protect_status, status);
}

void reset_protect_status(UINT32 status)
{
	g_protect_status &= (~status);
	//MGC_RESET_BIT(g_protect_status, status);
}
UINT32 getProctectStatusState(void)
{
	return g_protect_status;
}

UINT8 checkPortectStatusState(UINT32 status)
{
	if ((g_protect_status & status) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*********************************************************
 * rewrite by mgc 20140730
 ********************************************************/
enum UCT_ERR_CODE temp_manage(void)
{
	enum UCT_ERR_CODE rv = UCT_SUCCESS;
	UINT8 count = 0, n_recover = 0;
	INT8 temperature = 0;
	static UINT8 protect_high_temp_flag = 0;
#ifdef _USE_BM01
	for ( count = 0; count < TOTAL_TEMP_NUM; count++ )
	{
		temperature = getTemperatureByIndex( count );
		/*check for hyperpyrexia first*/
		if ( temperature > temp_warning_hi_threshold_get() )
		{
			if ( temperature > temp_critical_hi_threshold_get() )
			{
				MGC_SET_BIT ( temp_critical_event_high, count );
			}
			else
			{
				MGC_RESET_BIT ( temp_critical_event_high, count );
				MGC_SET_BIT ( temp_warning_high_event, count );
			}
		}
		/*then check for whether temeperature is too low*/
		else if ( temperature < temp_warning_low_threshold_get() )
		{
			if ( temperature < temp_critical_low_threshold_get() )
			{
				MGC_SET_BIT ( temp_critical_event_low, count );
			}
			else
			{
				MGC_RESET_BIT ( temp_critical_event_low, count );
				MGC_SET_BIT ( temp_warning_low_event, count );
			}
		}
		else
		{
			MGC_RESET_BIT ( temp_critical_event_high, count );
			MGC_RESET_BIT ( temp_warning_high_event, count );
			MGC_RESET_BIT ( atemp_critical_event_low, count );
			MGC_RESET_BIT ( temp_warning_low_event, count );
		} //do not clear these flags unless the next event is triggered

		/*temperature recover from too high*/
		if ( temperature < temp_nor_hi_threshold_get() )
		{
			MGC_SET_BIT ( temp_cooldown_event, count );
		}
		else
		{
			MGC_RESET_BIT ( temp_cooldown_event, count );
		}

		/*temperature recover from too low*/
		if ( temperature > temp_nor_low_threshold_get() )
		{
			MGC_SET_BIT ( temp_heat_event, count );
		}
		else
		{
			MGC_RESET_BIT ( temp_heat_event, count );
		}

	} // end for

	/*test mode, control by pc*/
	if( control_mode_get() != BMS_CONTROL_MODE_AUTO )
	{
		return rv;
	}

	/* finished coolling down*/
	if ( temp_cooldown_event == 0x3ff )
	{
		cool1_ctrl( TURN_OFF );
		cool2_ctrl( TURN_OFF );
		g_temp_state = BMS_CTL_NOMAL;
	}

	/*finished heating*/
	if ( temp_heat_event == 0x3ff )
	{
		heat_ctrl( TURN_OFF );a
		g_temp_state = BMS_CTL_NOMAL;
	}

	/*system state check */
	if ( temp_critical_event_high != 0 )
	{
		event_set_state ( EVENT_TEMP_HIGH_CRITICAL, EVENT_TRIGGER );
		event_set_state ( EVENT_TEMP_HIGH_WARNNING, EVENT_TRIGGER );
		g_temp_state = BMS_CTL_HI;
	}
	else if ( temp_warning_high_event != 0 )
	{
		event_set_state ( EVENT_TEMP_HIGH_CRITICAL, EVENT_STOP );
		event_set_state ( EVENT_TEMP_HIGH_WARNNING, EVENT_TRIGGER );
		g_temp_state = BMS_CTL_HI;
	}
	else if ( temp_critical_event_low != 0 )
	{
		event_set_state ( EVENT_TEMP_LOW_CRITICAL, EVENT_TRIGGER );
		event_set_state ( EVENT_TEMP_HIGH_WARNNING, EVENT_TRIGGER );
		g_temp_state = BMS_CTL_LOW;
	}
	else if ( temp_warning_low_event != 0 )
	{
		event_set_state ( EVENT_TEMP_LOW_CRITICAL, EVENT_STOP );
		event_set_state ( EVENT_TEMP_LOW_WARNNING, EVENT_TRIGGER );
		g_temp_state = BMS_CTL_LOW;a
	}
	else
	{
		event_set_state ( EVENT_TEMP_HIGH_WARNNING, EVENT_STOP );
		event_set_state ( EVENT_TEMP_HIGH_CRITICAL, EVENT_STOP );
		event_set_state ( EVENT_TEMP_LOW_CRITICAL, EVENT_STOP );
		event_set_state ( EVENT_TEMP_LOW_WARNNING, EVENT_STOP );
	}

	/*process according to state*/
	if( g_temp_state == BMS_CTL_HI )
	{
		cool1_ctrl( TURN_ON );
		cool2_ctrl( TURN_ON );
		heat_ctrl( TURN_OFF );
	}
	else if( g_temp_state == BMS_CTL_LOW )
	{
		cool1_ctrl( TURN_OFF );
		cool2_ctrl( TURN_OFF );
		heat_ctrl( TURN_ON );
	}
	else
	{	PROTECT_LOW_VOL
		cool1_ctrl( TURN_OFF );
		cool2_ctrl( TURN_OFF );
		heat_ctrl( TURN_OFF );
	}
#endif

#ifdef _USE_BM03

	for (count = 0; count < TOTAL_TEMP_NUM; count++)
	{
		temperature = getTemperatureByIndex(count);
		/*check for hyperpyrexia first*/
		if (temperature > VALUE_TH_OT)
		{
			protect_high_temp_flag = 1;
			break;
		}

		if (temperature < VALUE_TH_OT_RECOVER)
		{
			n_recover++;
		}
	}

	if (n_recover >= TOTAL_TEMP_NUM )
	{
		protect_high_temp_flag = 0;
	}

	if (protect_high_temp_flag == 1)
	{
		set_protect_status(PROTECT_HIGH_TEMPERARTURE);
	}
	else
	{
		reset_protect_status(PROTECT_HIGH_TEMPERARTURE);
	}
#endif

	return (rv);
}

#define led_clear_vol_status() {\
        led_set_status(LED_SCELV,0);\
        led_set_status(LED_SLV,0);\
          led_set_status(LED_LV,0); }

//static UINT32 timer_OV[TOTAL_VOLTAGES];
//static UINT32 timer_UV[TOTAL_VOLTAGES];
//static UINT32 timer_OC[TOTAL_CURRENTS];

/*
 * sum vol err change event happen
 * this func may be called by voltage module
 * by mgc 2014-10-30
 * */
int sum_vol_state_change_update(void)
{
	int state;
	int system_state = getSystemState();
	state = getSumVoltageState();
	if (state != 0)
		alarm(2);
	switch (state)
	{
	case STATE_VOL_LOW:
		//todo;
		break;
	case STATE_VOL_LOW_LEVEL1:
		//todo
		sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);
		break;
	case STATE_VOL_LOW_LEVEL2:
		//todo
		break;
	case STATE_VOL_HIGH:
		//todo
		if (system_state == SYS_STATE_CHARGING)
		{
			sendRequestToPILS(restrict_charge_current);
		}
		break;
	case STATE_VOL_HIGH_LEVEL1:
		//TODO
		if (system_state == SYS_STATE_CHARGING)
		{
			relayOp(RELAY_CHARGE, TURN_OFF);  //shut off the charge relay module
		}
		else if (getSystemState() == SYS_STATE_DRIVING)
		{
			sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);
			//after 2 seconds, if VCU don't shutdown, system shutdown the main relay 
		}
		break;
	case STATE_VOL_HIGH_LEVEL2:
		//TODO
		if (getSystemState() == SYS_STATE_CHARGING)
			relayOp(RELAY_CHARGE, TURN_OFF);  //shut off the charge relay module
		break;
	default:
		//todo
		break;

	}
}

/*
 * cell vol err change event happen
 * this func may be called by voltage module
 * by mgc 2014-10-30
 * */
int cell_vol_state_change_update(void)
{
	static int state = 0;
	int new_state;
	new_state = getCellVoltageState();
	switch (new_state)
	{
	case STATE_VOL_LOW:
		alarm(2);  //±¨¾¯2s
		//todo;
		break;
	case STATE_VOL_LOW_LEVEL1:

		//todo
		break;
	case STATE_VOL_LOW_LEVEL2:
		//todo
		break;
	case STATE_VOL_HIGH:
		//todo
		break;
	case STATE_VOL_HIGH_LEVEL1:
		//TODO
		break;
	case STATE_VOL_HIGH_LEVEL2:
		//TODO
		break;
	default:
		//TODO
		break;
	}
	state = new_state;
	return 0;
}

enum UCT_ERR_CODE vol_manage(void)
{
	enum UCT_ERR_CODE rv = UCT_SUCCESS;

	return (rv);
}

enum UCT_ERR_CODE cur_manage(void)
{
	enum UCT_ERR_CODE rv = UCT_SUCCESS;
	UINT8 i = 0;
//	UINT8 t_oc_protect = 0; // over current protect trigger flag
//	UINT8 t_sc_protect = 0; // short circuit protect trigger flag
//  UINT16 current = 0;
//  UINT32 time = 0;
//  UINT32 stop_time = 0;
//
//  current = get_current();
//
//  if (current > CURRENT_CRITICAL_LOW_GET())
//    {
//      event_set_state(EVENT_OVERCURRENT, EVENT_TRIGGER);
//      g_cur_state = BMS_CTL_HI;
////		led_set_status(LED_OCI, 1);
//    }
//  else
//    {
//      event_set_state(EVENT_OVERCURRENT, EVENT_STOP);
//      g_cur_state = BMS_CTL_NOMAL;
////		led_set_status(LED_OCI, 0);
//    }
	/*over current check!*/
//	if (getCurrentById(CURRENT_ID_CHARGE) >= VALUE_TH_OC)
//	{
//		if (timer_OC[CURRENT_ID_CHARGE] > VALUE_TH_TIME_FILTER_OC)
//			t_oc_protect = 1;
//		else
//			timer_OC[CURRENT_ID_CHARGE]++;
//	}
//	else
//	{
//		timer_OC[CURRENT_ID_CHARGE] = 0;
//	}
//
//	if (getCurrentById(CURRENT_ID_DISCHARGE) >= VALUE_TH_OC)
//	{
//		if (timer_OC[CURRENT_ID_DISCHARGE] > VALUE_TH_TIME_FILTER_OC)
//			t_oc_protect = 1;
//		else
//			timer_OC[CURRENT_ID_DISCHARGE]++;
//	}
//	else
//		timer_OC[CURRENT_ID_DISCHARGE] = 0;
//
//	/*short circuit check*/
//	if (getCurrentById(CURRENT_ID_CHARGE_OC) >= VALUE_TH_SHORT_CIRCUIT)
//	{
//		if (timer_OC[CURRENT_ID_CHARGE_OC] > VALUE_TH_TIME_FILTER_SC)
//		{
//			t_sc_protect = 1;
//		}
//		else
//		{
//			timer_OC[CURRENT_ID_CHARGE_OC]++;
//		}
//	}
//	else
//		timer_OC[CURRENT_ID_CHARGE_OC] = 0;
//	if (getCurrentById(CURRENT_ID_DISCHARGE_OC) >= VALUE_TH_SHORT_CIRCUIT)
//	{
//		if (timer_OC[CURRENT_ID_DISCHARGE_OC] > VALUE_TH_TIME_FILTER_SC)
//			t_sc_protect = 1;
//		else
//			timer_OC[CURRENT_ID_DISCHARGE_OC]++;
//	}
//	else
//		timer_OC[CURRENT_ID_DISCHARGE_OC] = 0;
//
//	/*don't clear this two status until system reset or mast command*/
//	if (t_sc_protect != 0 || t_oc_protect != 0 )
//	{
//		set_protect_status(PROTECT_OVER_CURRENT);
//	}
#ifdef _USE_BM03
	for( i = 0; i<TOTAL_CURRENTS; i++ )
	{
		if( getCurrentStatusById(i) != 0 )
		{
			/*don't clear this two status until system reset or mast command*/
			set_protect_status(PROTECT_OVER_CURRENT);
			break;
		}
	}
#endif

	return (rv);
}
/*******************************************************************************
 * SOC manage
 * by mgc 20140725
 *******************************************************************************/
enum UCT_ERR_CODE soc_manage(INT8 soc)
{
//  UCHAR status_charge = 0;
//  if (soc < 10)
//    {
//      event_set_state(EVENT_SOC_LOW_WARNING, EVENT_TRIGGER);
//      event_set_state(EVENT_SOC_LOW_CRITICAL, EVENT_TRIGGER);
//      g_soc_state = BMS_CTL_LOW;
//    }
//  else if (soc < 20)
//    {
//      event_set_state(EVENT_SOC_LOW_WARNING, EVENT_TRIGGER);
//      event_set_state(EVENT_SOC_LOW_CRITICAL, EVENT_STOP);
//      g_soc_state = BMS_CTL_LOW;
//    }
//  else
//    {
//      event_set_state(EVENT_SOC_LOW_WARNING, EVENT_STOP);
//      event_set_state(EVENT_SOC_LOW_CRITICAL, EVENT_STOP);
//      g_soc_state = BMS_CTL_NOMAL;
//    }
////	status_charge = gpio_get_charge();
//  if (gpio_get_charge() == 0 && gpio_get_discharge() == 0)
//    {
//      // to do   not charge or discharge
//      return UCT_SUCCESS;
//    }
////
////	if (soc <= 6) {
////		if (status_charge) {
////			//led_set_status(LED_CHARGE_6,1);
////		} else {
////			led_clear_soc_status();
////			led_set_status(LED_DISCHARGE_6, 1);
////		}
////	} else if (soc <= 25) {
////		led_clear_soc_status();
////		if (status_charge) {
////			led_set_status(LED_CHARGE_25, 1);
////		} else {
////			led_set_status(LED_DISCHARGE_25, 1);
////		}
////
////	} else if (soc <= 50) {
////		led_clear_soc_status();
////		if (status_charge) {
////			led_set_status(LED_CHARGE_50, 1);
////		} else {
////			led_set_status(LED_DISCHARGE_50, 1);
////		}
////	} else if (soc <= 75) {
////		led_clear_soc_status();
////		if (status_charge) {
////			led_set_status(LED_CHARGE_75, 1);
////		} else {
////			led_set_status(LED_DISCHARGE_75, 1);
////		}
////	} else if (soc < 100) {
////		led_clear_soc_status();
////		led_set_status(LED_CHARGE_100, 1);
////	} else { //>=100?
////		led_clear_soc_status();
////		led_set_status(LED_CHARGE_FULL, 1);
////	}
//
//  if ( soc >= 100 )
//    {
//      //SendStopCharge();
//      stop_charge();
////      led_clear_soc_status();
////		led_set_status(LED_WAIT, 1);
//    }

	return UCT_SUCCESS;
}

enum UCT_ERR_CODE control_mode_set(UINT8 mod)
{
	enum UCT_ERR_CODE rv = UCT_SUCCESS;

	if (BMS_CONTROL_MODE_TEST == mod)
	{
		control_mode = BMS_CONTROL_MODE_TEST;
	}
	else
	{
		control_mode = BMS_CONTROL_MODE_AUTO;
	}

	return (rv);
}

UINT8 control_mode_get(void)
{
	return (control_mode);
}

enum UCT_ERR_CODE insulation_res_manager(void)
{

//	UINT32 rp = get_insulation_res_p();
//	UINT32 rv = get_insulation_res_n();
//
//	if (rp < insulation_res_p_critical_threshold_get())
//	{
//		stop_charge();
//	}
//	else if (rp < insulation_res_p_warning_threshold_get())
//	{
//		// todo alarm
//	}
//	else
//	{
//
//	}
//
//	if (rv < insulation_res_n_critical_threshold_get())
//	{
//		stop_charge();
//	}
//	else if (rv < insulation_res_n_warning_threshold_get())
//	{
//		//todo alarm
//	}
//	else
//	{
//
//	}

	return UCT_SUCCESS;

}

/************************************************************************************************
 enum DEV_CTRL_TYPE_DEF {
 DEV_CTRL_TYPE_COOLING = 0,
 DEV_CTRL_TYPE_HEAT,
 DEV_CTRL_TYPE_TEMP_THRESHOLD,
 DEV_CTRL_TYPE_VOL_THRESHOLD,
 DEV_CTRL_TYPE_CHARGE_SWITCH
 };

 enum UCT_ERR_CODE temp_warning_hi_threshold_set(INT8 temp);
 INT8 temp_warning_hi_threshold_get(void);
 enum UCT_ERR_CODE temp_critical_hi_threshold_set(INT8 temp);
 INT8 temp_critical_hi_threshold_get(void);
 INT8 temp_nor_low_threshold_get(void);
 enum UCT_ERR_CODE temp_warning_low_threshold_set(INT8 temp);
 INT8 temp_warning_low_threshold_get(void);
 enum UCT_ERR_CODE temp_critical_low_threshold_set(INT8 temp);
 INT8 temp_critical_low_threshold_get(void);
 enum UCT_ERR_CODE vol_critical_hi_threshold_set (UINT16 vol);
 UINT16 vol_critical_hi_threshold_get(void);
 enum UCT_ERR_CODE vol_critical_low_threshold_set (UINT16 vol);
 UINT16 vol_critical_low_threshold_get(void);
 enum UCT_ERR_CODE vol_warning_low_threshold_set (UINT16 vol);
 *************************************************************************************************/

enum UCT_ERR_CODE device_control(UINT8 dev, UINT8 len, const UINT8 *data)
{
	enum UCT_ERR_CODE rv = UCT_SUCCESS;
	//UINT16 temp_16 = 0;

	switch (dev)
	{

	case DEV_CTRL_TYPE_COOLING:
#ifdef _USE_BM01
		if ((data[0] & 0x03) == 0x03)
		{
			cool1_ctrl(TURN_ON);
			cool2_ctrl(TURN_ON);
		}
		else if ((data[0] & 0x03) == 0x02)
		{
			cool1_ctrl(TURN_OFF);
			cool2_ctrl(TURN_ON);
		}
		else if ((data[0] & 0x03) == 0x01)
		{
			cool1_ctrl(TURN_ON);
			cool2_ctrl(TURN_OFF);
		}
		else
		{
			cool1_ctrl(TURN_OFF);
			cool2_ctrl(TURN_OFF);
		}
#endif

		break;

	case DEV_CTRL_TYPE_HEAT:
#ifdef _USE_BM01
		if (data[0] != 0)
		{
			heat_ctrl(TURN_ON);
		}
		else
		{
			heat_ctrl(TURN_OFF);
		}
#endif

		break;

	case DEV_CTRL_TYPE_TEMP_THRESHOLD:
#ifdef _USE_BM01
		if ((INT8) (*(data + 1)) > temp_nor_low_threshold_get())
		return UCT_ERR_INVALID_PARAMETER;

		if ((INT8) (*data) < temp_nor_hi_threshold_get())
		return UCT_ERR_INVALID_PARAMETER;

		rv = temp_warning_low_threshold_set(*(data + 1));
		rv = temp_warning_hi_threshold_set(*(data));
		store_ctl_param();
#endif
		break;

	case DEV_CTRL_TYPE_VOL_THRESHOLD:
#ifdef _USE_BM01
		temp_16 = (data[0] << 8) | data[1];
		vol_critical_hi_threshold_set(temp_16);
		temp_16 = (data[2] << 8) | data[3];
		vol_critical_low_threshold_set(temp_16);
		store_ctl_param();
#endif
		break;
		/*
		 case DEV_CTRL_TYPE_CHARGE_SWITCH:

		 if (data[0] > 3)
		 return UCT_ERR_OPT_FAIL;

		 set_charge_sw_state(data[0]);
		 break;
		 */
	}

	return rv;
}

UINT8 vol_state_get()
{
	return g_vol_state;
}
UINT8 soc_state_get()
{

	return g_soc_state;
}
UINT8 current_state_get()
{
	return g_cur_state;
}

UINT8 temperature_state_get()
{
	return g_temp_state;
}

UINT8 get_ctl_state(void)
{
	UINT8 ret = 0;
	ret = vol_state_get();
	ret |= soc_state_get() << 2;
	ret |= current_state_get() << 4;
	ret |= temperature_state_get() << 6;
	return ret;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
