/*********************************************************
 *
 * ��оԴ΢�������޹�˾(UCT)
 *
 * File Description
 *
 *   $Revision: 3403 $
 *   $Author: huhai $
 *   $Date: 2014-09-01 17:02:05 +0800 (周一, 01 九月 2014) $
 *
 *********************************************************/
#include "cmd_class.h"
#include "string.h"
#include "stdio.h"
#include "uart_protocol.h"
#include "public/public.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"
#include "banlance_battery/banlancebattery.h"
#include "../mgclib/mgcbytesorder.h"
#include "../insulation_resistance/insulation_resistance.h"

static UCHAR process_result[MAX_PARAM_ACK_LEN];

/*********************************************************
 * 输入:
 *      cmds~命令字开始的缓冲�? *
 * 输出:
 *      cmd_process_result 指向处理结果的缓�? * cmds = cmd | len1 len0 | param |
 *********************************************************/
#include "uart_protocol.h"
// extern  UCHAR g_rx_cmd_buff[MAX_CMD_PARAM_SIZE];
#define PROTOCOL_LEN_SIZE 2
INT16
ProcessCmdByWords(const UCHAR* cmds, UCHAR **cmd_process_result) //ÃüÁîŽŠÀíÀàÊµÏÖ
{
  UCHAR i = 0;
  UCHAR argc;
  const UCHAR *argv[3];
  argc = 2;

  for (i = 0; i < g_uart_cmd_len; i++)
    {
      if (cmds[0] == g_uart_cmds[i].cmd_word && g_uart_cmds[i].process)
        {
          argv[0] = (UCHAR *) g_uart_cmds[i].cmd_name; //���?          argv[1] = cmds + 1; //len

          if (BYTESORDER_S2H_16(cmds + 1) > 0)
            {
              argc++; //
              argv[2] = (UCHAR *) (*(int *) (cmds + PROTOCOL_LEN_SIZE + 1)); //命令参数
            }

          (void) mprintf("Get Cmd %s \r\n",
              (const char *) g_uart_cmds[i].cmd_name);
          //(void)mprintf (" param addr:%04x *param addr:%02x g_rx_cmd_buff addr :%04x \r\n", cmds+3, argv[2], g_rx_cmd_buff );
          *cmd_process_result = process_result;
          return g_uart_cmds[i].process(argc, argv);
        }
    }

  return -1;
}

DECLARE_FUNC ( CMD_HAND_SHAKE )
{
  (void) argv;
  (void) argc;
  process_result[0] = 0;
  return 1;
}

DECLARE_FUNC ( CMD_GET_VER )
{
  UINT16 softver = ver_software_get();
  UINT8 hardver = ver_hardware_get();
  UINT8 ver[5];
  (void) argv;
  (void) argc;
  BYTESORDER_H2S_16(softver, ver);
  ver[4] = hardver;
  (void) memcpy(process_result, ver, sizeof(ver));
  return sizeof(software_ver);
}

DECLARE_FUNC ( CMD_HEART_BEAT )
{
  (void) argv;
  (void) argc;
  process_result[0] = 0;
  return 1;
}

#ifdef USE_BM01_OLD_PROTOCAL
#pragma pack(1)
struct board_info
{
  UINT16 current;
  UCHAR leftsoc;
  UCHAR temperature[10];
  UINT16 vol[24];
  UCHAR balance_sw[3];
  UCHAR fan_state;
  UCHAR heating_applaince_state;
  UCHAR charge_state;
};
#else
/* extern  UCHAR g_rx_cmd_buff[MAX_CMD_PARAM_SIZE];
 *
 struct dev_item
 {
 UINT8 item_num;
 const void *item_value;
 };
 struct board_info
 {
 struct dev_item current;
 struct dev_item soc;
 struct dev_item soh;
 struct dev_item charge_status;
 struct dev_item temperature;
 struct dev_item voltage;
 struct dev_item balance;
 struct dev_item fan;
 struct dev_item heat;
 };*/
#endif

#ifdef USE_BM01_OLD_PROTOCAL
static enum UCT_ERR_CODE
bms_info_get(struct board_info *info)
{
  enum UCT_ERR_CODE rv = UCT_SUCCESS;

  UINT8 count = 0;
  UINT8 cool = 0;
  /*
  INT8 temperature[TOTAL_TEMP_NUM] =
    { 0 };
  */
  UINT16 voltage = 0;
  info->current = 0;//get_current();
  info->leftsoc = soc_read();
  for (count = 0; count < TOTAL_TEMP_NUM; count++)
    {
      info->temperature[count] = getTemperatureByIndex(count);
    }

  for (count = 0; count < TOTAL_VOLTAGES; count++)
    {
      voltage = getVoltageByIndex(count);
      BYTESORDER_H2S_16(voltage, (UCHAR *) ((UCHAR *) (info->vol) + count * 2));
    }

  battery_blance_sw_get(info->balance_sw);
  cool = cool2_get() & 0x01;
  info->fan_state = (cool << 1) | (cool1_get() & 0x01);
  info->heating_applaince_state = heat_get();
  info->charge_state = GetBatteryMode();
  return rv;
};
#endif

//#ifdef USE_BM01_OLD_PROTOCAL
//static void printf_board_info( struct board_info *pinfo )
//{
//    UCHAR i = 0;
//    mprintf( "current:%d\r\n",pinfo->current);
//    mprintf( "soc:%d\r\n",pinfo->leftsoc);
//
//    for ( i = 0; i<10; i++ )
//      mprintf( "t[%d]:%d\r\n",i,pinfo->temperature[i] );
//
//    for ( i=0; i<24; i++)
//      mprintf( "vol[%d]:%d\r\n",i,pinfo->vol[i]);
//
//    for( i=0; i<3; i++ )
//      mprintf( "banlance sw[%d]:%d\r\n",i, pinfo->balance_sw[i] );
//
//    mprintf( "fan state:%d\r\n",pinfo->fan_state );
//    mprintf( "heat applaince state:%d\r\n",pinfo->heating_applaince_state );
//    mprintf( "charge state:%d\r\n",pinfo->charge_state );
//
//}
//#endif

int
get_socs(UINT8 *data, UINT8 len)
{
  *data = soc_read();
  return (1);
}
int
get_sohs(UINT8 *data, UINT8 len)
{
	*data = 100;
  return (1);
}
int
get_chargestatus(UINT8 *data, UINT8 len)
{
  *data = getChargeStatus();
  return (1);
}
//#define TOTAL_TEMP_NUM;
int
get_temperatures(UINT8 *data, UINT8 len)
{
  UINT8 i = 0;
  //INT16 *p = (INT16 *)data;
  for( i = 0; i<len; i++ )
  {
      BYTESORDER_H2S_16(getTemperatureByIndex(i),data);
     data+=2;
  }
  //get_temperature((INT8 *) data);
  return (TOTAL_TEMP_NUM*sizeof(UINT16));
}
int
get_voltages(UINT8 *data, UINT8 len)
{
  UINT8 i =0;
 // UINT16 *pv = (UINT16 *)data;
  for( i =0; i<len; i++ )
  {
    BYTESORDER_H2S_16(getVoltageByIndex(i),data);
    data+=2;
    //get_voltage((UINT16 *) data);
  }
  return (TOTAL_VOLTAGES*sizeof(UINT16));
}
int
get_balances(UINT8 *data, UINT8 len)
{
  battery_blance_sw_get(data);
  return (3);
}
int
get_fans(UINT8 *data, UINT8 len)
{
#ifdef _USE_BM03
  return (0);
#endif

#ifdef _USE_BM01
  UCHAR cool = cool2_get() & 0x01;
  *data = ( cool << 1 ) |( cool1_get() & 0x01 );
  return 1;
#endif
}

int
get_heatpannals(UINT8 *data, UINT8 len)
{
#ifdef _USE_BM03
  return (0);
#endif

#ifdef _USE_BM01
  *data= heat_get();
  return 1;
#endif
}

int
get_currents(UINT8 *data, UINT8 len)
{
#ifdef _USE_BM01
  UINT16 current = 0;//get_current();
  BYTESORDER_H2S_16( current, data);
  return sizeof(current );
#endif
#ifdef _USE_BM03
  UINT8 i = 0;
  UINT16 current = 0;
  for (i = 0; i < TOTAL_CURRENTS; i++)
    {
      current = getCurrentById(i);
      BYTESORDER_H2S_16(current, data);
      data += sizeof(current);
    }
  return (TOTAL_CURRENTS * sizeof(current));
#endif

}

DECLARE_FUNC ( CMD_GET_DEVINFO )
{
  int ret = 0;
  UCHAR count = 0;

#ifdef USE_BM01_OLD_PROTOCAL	
  struct board_info *p_info = (struct board_info *) process_result;
  (void) argv;
  (void) argc;
  (void) bms_info_get(p_info);
  //intf_board_info( &m_info );	
//		(void)memcpy( process_result, (char *)&m_info, sizeof ( struct board_info ) );
  // printf_board_info( (struct board_info *)process_result ); 
  return sizeof(struct board_info);
#else
  /*get currents*/
  (void)argv;
  (void)argc;
  process_result[count++] = TOTAL_CURRENTS; //
  ret = get_currents( process_result+count, TOTAL_CURRENTS );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+=ret;
  /*get soc s*/
  process_result[count++] = TOTAL_SOC_NUM;
  ret = get_socs( process_result+count, TOTAL_SOC_NUM );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+= ret;
  /*get sohs*/
  process_result[count++] = TOTAL_SOH_NUM;
  ret = get_sohs( process_result+count, TOTAL_SOH_NUM );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+= ret;
  /*get chargestatus*/
  process_result[count++] = TOTAL_CHARGE_STATUS_NUM;
  ret = get_chargestatus( process_result+count, TOTAL_CHARGE_STATUS_NUM );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+= ret;
  /*get tempraturs*/
  process_result[count++] = TOTAL_TEMP_NUM;
  ret = get_temperatures( process_result+count, TOTAL_TEMP_NUM );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+= ret;
  /*get voltages*/
  process_result[count++] = TOTAL_VOLTAGES;
  ret = get_voltages( process_result+count, TOTAL_VOLTAGES );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+= ret;
  /*get balances*/
  process_result[count++] = TOTAL_BALANCES;
  ret = get_balances( process_result+count, TOTAL_BALANCES );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+= ret;
  /*get fans*/
  process_result[count++] = TOTAL_FANS_NUM;
  ret = get_fans( process_result+count, TOTAL_FANS_NUM );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+= ret;
  /*get heatpannals*/
  process_result[count++] = TOTAL_HEATPANNALS_NUM;
  ret = get_heatpannals( process_result+count, TOTAL_HEATPANNALS_NUM );
  if( ret < 0 )
    {
      goto err_end;
    }
  count+= ret;

  return (count);
  err_end:
  return (1);
#endif	
}

DECLARE_FUNC ( CMD_GET_THRESHOLD_V )
{
  struct threshold_v tv;
  (void) argv;
  (void) argc;
  tv.undervoltage = vol_warning_low_threshold_get();
  tv.overpressure = vol_warning_hi_threshold_get();
  tv.overpressure_critical = vol_critical_hi_threshold_get();
  tv.undervoltage_critical = vol_critical_low_threshold_get();
  mprintf("low v:%d high v:%d \r\n", tv.undervoltage, tv.overpressure);
  (void) memcpy(process_result, &tv, sizeof(struct threshold_v));
  return sizeof(struct threshold_v);
}

DECLARE_FUNC ( CMD_GET_THRESHOLD_T )
{
  struct threshold_t tt;
  (void) argv;
  (void) argc;
  tt.lowtemperature = temp_warning_low_threshold_get();
  tt.hightemperature = temp_warning_hi_threshold_get();
  tt.hightemperature_critical = temp_critical_hi_threshold_get();
  tt.lowtemperature_critical = temp_critical_low_threshold_get();
  //mprintf("low temp:%d high temp:%d \r\n", tt.lowtemperature, tt.hightemperature );
  (void) memcpy(process_result, &tt, sizeof(struct threshold_t));
  return sizeof(struct threshold_t);
}

DECLARE_FUNC ( CMD_GET_CHAGE_TIMES )
{
  UINT16 charge_times = GET_CUR_CHARGETIMES();
  UINT16 discharge_times = GET_CUR_DISCHARGETIMES();
  BYTESORDER_H2S_16(charge_times, process_result);
  BYTESORDER_H2S_16(discharge_times, process_result + 2);
  (void) argv;
  (void) argc;
  return sizeof(charge_times) + sizeof(discharge_times);
}

DECLARE_FUNC ( CMD_SET_THRESHOLD_V )
{

  struct threshold_v tv;
  (void) argv;
  (void) argc;
  process_result[0] = 1;

#if 0  
  if ( argc < 3 || argv[1][0] < 4 )
    {
      return 1;
    }
#endif

  (void) memcpy((char *) &tv, argv[2], sizeof(struct threshold_v));
  if (vol_critical_hi_threshold_set(tv.overpressure_critical) != UCT_SUCCESS)
    goto err;
  if (vol_critical_low_threshold_set(tv.undervoltage_critical) != UCT_SUCCESS)
    goto err;
  if (vol_warning_hi_threshold_set(tv.overpressure) != UCT_SUCCESS)
    goto err;
  if (vol_warning_low_threshold_set(tv.undervoltage) != UCT_SUCCESS)
    goto err;

  /*
   if ( device_control ( DEV_CTRL_TYPE_VOL_THRESHOLD , argv[1][0], argv[2] ) != UCT_SUCCESS )
   {
   return 1;
   }
   */
  process_result[0] = 0;
  err: return 1;
}

DECLARE_FUNC ( CMD_SET_THRESHOLD_T )
{
  struct threshold_t tt;
  process_result[0] = 1;
#if 0  
  if ( argc < 3 || argv[1][0] < 2 )
    {
      process_result[0] = 1;
      return 1;
    }
#endif
  (void) memcpy((char *) &tt, argv[2], sizeof(struct threshold_t));
  if (temp_critical_hi_threshold_set(tt.hightemperature_critical)
      != UCT_SUCCESS)
    goto err;
  if (temp_critical_low_threshold_set(tt.lowtemperature_critical)
      != UCT_SUCCESS)
    goto err;
  if (temp_warning_hi_threshold_set(tt.hightemperature) != UCT_SUCCESS)
    goto err;
  if (temp_warning_low_threshold_set(tt.lowtemperature) != UCT_SUCCESS)
    goto err;
  /*
   if ( device_control ( DEV_CTRL_TYPE_TEMP_THRESHOLD , argv[1][0], argv[2] ) != UCT_SUCCESS )
   {
   process_result[0] = 1;
   }
   */
  process_result[0] = 0;
  err: return 1;
}

extern void
print_gpio_status(void);
DECLARE_FUNC ( CMD_SET_FAN_SW )
{
  process_result[0] = 0;
#if 0
  if ( argc < 3 || argv[1][0] < 2 )
    {
      return 1;
    }
#endif
  if (device_control(DEV_CTRL_TYPE_COOLING, argv[1][0], argv[2]) != UCT_SUCCESS)
    {
      process_result[0] = 1;
    }
  print_gpio_status();
  return 1;
}

DECLARE_FUNC ( CMD_SET_HEATING_APPLAINCE )
{
  process_result[0] = 0;
#if 0
  if ( argc < 3 || argv[1][0] < 1 )
    {
      process_result[0] = 1;
      return 1;
    }
#endif
  if (device_control(DEV_CTRL_TYPE_HEAT, argv[1][0], argv[2]) != UCT_SUCCESS)
    {
      process_result[0] = 1;
    }

  return 1;
}

enum UCT_ERR_CODE
set_charge_sw_state( UINT8 state)
{

  enum UCT_ERR_CODE rv = UCT_SUCCESS;

  //if ( g_charge_status == state )
  //  return UCT_SUCCESS;
  if(state&0x01)
  {
	  charge_ctrl(TURN_ON);
  }
  else
  {
	  charge_ctrl(TURN_OFF);
  }

  if(state&0x02)
  {
	  discharge_ctrl(TURN_ON);
  }
  else
  {
	  discharge_ctrl(TURN_OFF);
  }
//  switch (state)
//    {
//  case STATE_BLOW: // = 1,
//    start_precharge();
//    break;
//
//  case STATE_DOWN: // = -1,
//    //stop_charge (  );
//    discharge_ctrl(TURN_ON);
//    break;
//
//  case STATE_IDLE: // = 0,
//    stop_charge();
//    discharge_ctrl(TURN_OFF);
//    //charge_ctrl ( TURN_OFF );
//    break;
//
//  case STATE_SETUP: // = 2,  recover factory settings
//    recover_factory_settings();
//    //todo reset system
//    MCU_Reset();
//    break;
//
//  default:
//    rv = UCT_ERR_INVALID_PARAMETER;
//    break;
//    }
//
//  if (rv == UCT_SUCCESS)
//    {
//      g_charge_status = state;
//    }

  return (rv);
}
DECLARE_FUNC ( CMD_SET_CHANGE_SW )
{
  UINT8 sw_value = argv[2][0];
  process_result[0] = 0;
#if 0
  if ( argc < 3 || argv[1][0] < 1 )
    {
      process_result[0] = 1;
      return 1;
    }
#endif
  ;

  if (set_charge_sw_state(sw_value)
      != UCT_SUCCESS)
    {
      process_result[0] = 1;
    }
  return (1);
}

DECLARE_FUNC ( CMD_GET_REGISTER_VALUE )
{
  (void) argv;
  (void) argc;
  process_result[0] = 1;
  return (1);
}

DECLARE_FUNC ( CMD_SET_REGISTER_VALUE )
{
  (void) argv;
  (void) argc;
  process_result[0] = 1;
  return (1);
}

DECLARE_FUNC ( CMD_SET_TEST_MODE )
{
  (void) argc;
  process_result[0] = 0;
  if (control_mode_set(argv[2][0]) != UCT_SUCCESS)
    process_result[0] = 1;
  else
    process_result[0] = control_mode_get();
  return 1;
}

DECLARE_FUNC ( CMD_SET_VOL_DEVIATION )
{
  UINT16 v = 0;
  process_result[0] = 1;
  if (argv[2] != 0)
    {
      v = BYTESORDER_S2H_16(argv[2]);
      set_threshold_v(v);
      process_result[0] = 0;
    }
  return 1;
}

DECLARE_FUNC ( CMD_GET_THRESHOLD_CURRENT )
{
  UINT16 i = CURRENT_CRITICAL_LOW_GET();
  BYTESORDER_H2S_16(i, process_result);
  return 2;
}

DECLARE_FUNC ( CMD_SET_THRESHOLD_CURRENT )
{
  UINT16 a = 0;
  (void) argc;
  process_result[0] = 0;
  a = BYTESORDER_S2H_16(argv[2]);
  if (CURRENT_CRITICAL_LOW_SET(a) != UCT_SUCCESS)
    {
      process_result[0] = 1;
    }
  return 1;
}

DECLARE_FUNC ( CMD_GET_THRESHOLD_SUM_VOL )
{
  UINT32 v = vol_sum_low_threshold_get();
  BYTSORDER_H2S_32(v, process_result + 4);
  v = vol_sum_high_threshold_get();
  BYTSORDER_H2S_32(v, process_result);
  (void) argc;
  (void) argv;
  return 8;
}

DECLARE_FUNC ( CMD_SET_THRESHOLD_SUM_VOL )
{
  UINT32 v_low = 0, v_high = 0;
  process_result[0] = 0;
  if (!argv[2])
    {
      process_result[0] = 1;
      goto out;
    }
  v_high = BYTSORDER_S2H_32(argv[2]);
  if (vol_sum_high_threshold_set(v_high) != UCT_SUCCESS)
    {
      process_result[0] = 1;
      goto out;
    }
  v_low = BYTSORDER_S2H_32(argv[2] + 4);
  if (vol_sum_low_threshold_set(v_low) != UCT_SUCCESS)
    {
      process_result[0] = 1;
      goto out;
    }
  out: return (1);
}

DECLARE_FUNC ( CMD_GET_THRESHOLD_OFFSET_V )
{
  UINT16 v = get_threshold_v();
  BYTESORDER_H2S_16(v, process_result);
  return 2;
}

DECLARE_FUNC ( CMD_GET_INSULATION_RESISTANCE )
{
#ifdef _USE_BM01
  UINT16 rp = get_insulation_res_p();
  UINT16 rn = get_insulation_res_n();
  BYTESORDER_H2S_16( rp, process_result );
  BYTESORDER_H2S_16( rn, process_result );
  return 4;
#endif
#ifdef _USE_BM03
  process_result[0] = 1;
  return 1;
#endif
}

#pragma pack(push)
#pragma  pack(1)
struct t_exception_cmd
{
  UCHAR cmd;
  union exeception_param
  {
    UCHAR mode;
    struct t_excetpion_vol
    {
      UCHAR n_battery;
      UINT16 battarys[MAX_BATTERYS];
    } m_vol;
    struct t_excetpion_temp
    {
      UCHAR n_temp_sensors;
      INT8 temperature[TOTAL_TEMP_NUM];
    } m_temp;
    struct t_exception_current
    {
      UINT8 discharge_current[2];
      UINT8 charge_current[2];
    }m_current;
  } param;
};
#pragma pack(pop)

enum e_exception_cmd
{
  CMD_EXCEPTION_SET_MODE = 1,
  CMD_EXCEPTION_SET_VOL = 2,
  CMD_EXCEPTION_SET_TEMP = 3,
  CMD_EXCEPTION_SET_CURRENTS = 6
};
/*
 #define set_simulator_mode(mode)
 #define set_simulator_vol( pvols, n_batterys)
 #define set_simulator_vol( ptemps, n_temps)
 */
DECLARE_FUNC ( CMD_EXCEPTION_TEST )
{
  struct t_exception_cmd *m_exception_cmd = (struct t_exception_cmd *) argv[2];
//  UINT32 battery_start_index = 3;//(UINT32)(((struct t_exception_cmd *)0)->param.m_vol.battarys);
  process_result[0] = m_exception_cmd->cmd;
  process_result[1] = 0;
  UINT16 charge_current = 0;
  UINT16 discharge_current = 0;
  switch (m_exception_cmd->cmd)
    {
  case CMD_EXCEPTION_SET_MODE:
    set_simulator_mode(m_exception_cmd->param.mode);
    break;
  case CMD_EXCEPTION_SET_TEMP:
    set_simulator_temp(m_exception_cmd->param.m_temp.temperature,
        m_exception_cmd->param.m_temp.n_temp_sensors);
    break;
  case CMD_EXCEPTION_SET_VOL:
    /*set_simulator_vol((UINT16 *)&argv[2][battery_start_index],
        m_exception_cmd->param.m_vol.n_battery);
    */
    set_simulator_vol(m_exception_cmd->param.m_vol.battarys,
        m_exception_cmd->param.m_vol.n_battery);
    //*/
    break;

  case CMD_EXCEPTION_SET_CURRENTS:
	  discharge_current = BYTESORDER_S2H_16(m_exception_cmd->param.m_current.discharge_current);
	  charge_current = BYTESORDER_S2H_16( m_exception_cmd->param.m_current.charge_current);
	  set_simulator_current(charge_current, discharge_current);
	break;

  default:
    process_result[1] = 1;
    break;
    }
  return (2);
}



 DECLARE_FUNC ( CMD_RECOVER_SYSTEM_SETS )
 {
	 recover_factory_settings();
	 MCU_Reset();
	 process_result[0] = 0;
	 return (1);
 }

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
