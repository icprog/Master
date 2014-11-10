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
/*
 * sum vol err change event happen
 * this func may be called by voltage module
 * by mgc 2014-10-30
 * */
/*
 * process for fault of voltage
 * 
 * sum high vol  fault lever1:
 * cell high vol fault lever1:
 * 持续2S报警，充电时强制切断充电继电器；行驶时请求切断主正继电器，接收到VCU断电指令后，切断主正继电器。
 * 
 * cell low voltage fault lever1:
 * sum low voltage fault lever1:
 * 持续2S报警，行驶时请求切断主正继电器，接收到VCU断电指令后，切断主正继电器。
 * 
 * cell high vol fault lever2:
 * sum high vol fault lever2:
 * 持续2S报警，若充电则通过CAN关闭充电机。
 * 
 * sum high vol fault:
 * cell high vol fualt:
 * 持续2S报警，若充电则限制充电电流。
 * sum low voltage fault:
 * cell low voltage fault:
 * 持续2S报警 不显示故障
 * 
 * cell low voltage fault lever2:
 * sum low voltage fault lever2:
 * cell voltage difference too bigger lever2:
 * 持续2S报警
 * 
 * cell voltage differnce too bigger lever1:
 * 持续2S报警，请求切断主正继电器，收到VCU断电指令后，切断主正继电器。
 * 
 * */
int VolStateUpdate(int state)
{
	static int laststate = state;
	//int system_state = getSystemState();
	//state = getSumVoltageState();
	if (state != STATE_VOL_NORMAL)
		alarm(2);
	
	switch (state)
	{
	case STATE_VOL_NORMAL:
	case STATE_SUM_VOL_LOW:
		//todo;
		break;
		
	case STATE_SUM_VOL_LOW_LEVEL1:
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
		alarm(2);  //报警2s
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

int AlarmUpdateVoltage(void);	
int AlarmUpdateTemperature(void);	
int AlarmUpdateCurrent(void);	
int AlarmUpdateSoc(void);	
int AlarmUpdateInsulutionResistance(void);	
int AlarmUpdateRelayModule(void);	
int AlarmUpdateBmsSelfCheck(void);	

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
