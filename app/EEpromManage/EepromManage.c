#include "EepromManage.h"
#include "../insulation_resistance/insulation_resistance.h"

struct ctl_eerom_value g_ctl_eerom_value;

//extern struct ctl_eerom_value g_ctl_eerom_value;
#define temp_nor_hi_threshold  g_ctl_eerom_value.temp_nor_hi
#define temp_warning_hi_threshold g_ctl_eerom_value.temp_warning_hi
#define temp_critical_hi_threshold g_ctl_eerom_value.temp_critical_hi
#define temp_nor_low_threshold g_ctl_eerom_value.temp_nor_low
#define temp_warning_low_threshold g_ctl_eerom_value.temp_warning_low
#define temp_critical_low_threshold g_ctl_eerom_value.temp_critical_low

#define vol_warning_low_threshold g_ctl_eerom_value.vol_warning_low
#define vol_warning_hi_threshold g_ctl_eerom_value.vol_warning_hi
#define vol_critical_hi_threshold g_ctl_eerom_value.vol_critical_hi
#define vol_critical_low_threshold g_ctl_eerom_value.vol_critical_low
#define vol_sum_low_threshold g_ctl_eerom_value.vol_sum_low
#define vol_sum_high_threshold g_ctl_eerom_value.vol_sum_high
#define CURRENT_CRITICAL_LOW g_ctl_eerom_value.CURRENT_CRITICAL_LOW_value     //400A the max  current allowed of BMS


void init_g_clt_value ( void )
{
    g_ctl_eerom_value.temp_nor_hi = 50;
    g_ctl_eerom_value.temp_warning_hi  = 60;
    g_ctl_eerom_value.temp_critical_hi  = 75;
    g_ctl_eerom_value.temp_nor_low  = 5;
    g_ctl_eerom_value.temp_warning_low  = -20;
    g_ctl_eerom_value.temp_critical_low  = -30;

    g_ctl_eerom_value.vol_critical_hi  = 42000;
    g_ctl_eerom_value.vol_critical_low  = 25000;
    g_ctl_eerom_value.vol_warning_low  = 26000;
    g_ctl_eerom_value.vol_warning_hi  = 40000;

    g_ctl_eerom_value.vol_sum_low  = 500000;
    g_ctl_eerom_value.vol_sum_high  = SUM_VOL;

    g_ctl_eerom_value.insulation_res_p_warnning = OV ( 100 );
    g_ctl_eerom_value.insulation_res_p_critical = OV ( 500 );
    g_ctl_eerom_value.insulation_res_p_warnning = OV ( 100 );
    g_ctl_eerom_value.insulation_res_p_critical = OV ( 500 );

    g_ctl_eerom_value.CURRENT_CRITICAL_LOW_value =  400;
    g_ctl_eerom_value.CURRENT_WARNNING_LOW_value =  400;
}



INT8 temp_nor_hi_threshold_get ( void )
{
    return temp_nor_hi_threshold;
}

enum UCT_ERR_CODE temp_warning_hi_threshold_set ( INT8 temp )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;

    temp_warning_hi_threshold = temp;

    return rv;
}

INT8 temp_warning_hi_threshold_get ( void )
{
    return temp_warning_hi_threshold;
}

enum UCT_ERR_CODE temp_critical_hi_threshold_set ( INT8 temp )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;

    temp_critical_hi_threshold = temp;

    return rv;
}

INT8 temp_critical_hi_threshold_get ( void )
{
    return temp_critical_hi_threshold;
}

INT8 temp_nor_low_threshold_get ( void )
{
    return temp_nor_low_threshold;
}

enum UCT_ERR_CODE temp_warning_low_threshold_set ( INT8 temp )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;

    temp_warning_low_threshold = temp;

    return rv;
}

INT8 temp_warning_low_threshold_get ( void )
{
    return temp_warning_low_threshold;
}

enum UCT_ERR_CODE temp_critical_low_threshold_set ( INT8 temp )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;

    temp_critical_low_threshold = temp;

    return rv;
}

INT8 temp_critical_low_threshold_get ( void )
{
    return temp_critical_low_threshold;
}


enum UCT_ERR_CODE vol_critical_hi_threshold_set ( UINT16 vol )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;

    vol_critical_hi_threshold = vol;

    return rv;
}

UINT16 vol_critical_hi_threshold_get ( void )
{
    return vol_critical_hi_threshold;
}

enum UCT_ERR_CODE vol_critical_low_threshold_set ( UINT16 vol )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;

    vol_critical_low_threshold = vol;

    return rv;
}

UINT16 vol_critical_low_threshold_get ( void )
{
    return vol_critical_low_threshold;
}

enum UCT_ERR_CODE vol_warning_low_threshold_set ( UINT16 vol )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;

    vol_warning_low_threshold = vol;

    return rv;
}

UINT16 vol_warning_low_threshold_get ( void )
{
    return vol_warning_low_threshold;
}



enum UCT_ERR_CODE vol_warning_hi_threshold_set ( UINT16 vol )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;

    vol_warning_hi_threshold = vol;

    return rv;
}

UINT16 vol_warning_hi_threshold_get ( void )
{
    return vol_warning_hi_threshold;
}

UINT32 vol_sum_high_threshold_get ( void )
{
    return  vol_sum_high_threshold;
}

enum UCT_ERR_CODE    vol_sum_high_threshold_set ( UINT32 v )
{
    vol_sum_high_threshold = v;
    return UCT_SUCCESS;
}


UINT32 vol_sum_low_threshold_get ( void )
{
    return  vol_sum_low_threshold;
}

enum UCT_ERR_CODE    vol_sum_low_threshold_set ( UINT32 v )
{
    vol_sum_low_threshold = v;
    return UCT_SUCCESS;
}


enum UCT_ERR_CODE CURRENT_CRITICAL_LOW_SET ( UINT16 current )
{
    CURRENT_CRITICAL_LOW = current;
    return UCT_SUCCESS;
}

UINT16 CURRENT_CRITICAL_LOW_GET ( void )
{
    return CURRENT_CRITICAL_LOW;
}


enum UCT_ERR_CODE insulation_res_p_warning_threshold_set ( UINT16 ov )
{
    g_ctl_eerom_value.insulation_res_p_warnning = OV ( ov );
    return UCT_SUCCESS;
}
UINT16 insulation_res_p_warning_threshold_get ( void )
{
    return g_ctl_eerom_value.insulation_res_p_warnning;
}

enum UCT_ERR_CODE insulation_res_p_critical_threshold_set ( UINT16 ov )
{
    g_ctl_eerom_value.insulation_res_p_critical = OV ( ov );
    return UCT_SUCCESS;
}
UINT16 insulation_res_p_critical_threshold_get ( void )
{
    return g_ctl_eerom_value.insulation_res_p_critical;
}

enum UCT_ERR_CODE insulation_res_n_warning_threshold_set ( UINT16 ov )
{
    g_ctl_eerom_value.insulation_res_n_warnning = OV ( ov );
    return UCT_SUCCESS;
}
UINT16 insulation_res_n_warning_threshold_get ( void )
{
    return g_ctl_eerom_value.insulation_res_n_warnning;
}

enum UCT_ERR_CODE insulation_res_n_critical_threshold_set ( UINT16 ov )
{
    g_ctl_eerom_value.insulation_res_n_critical = OV ( ov );
    return UCT_SUCCESS;
}
UINT16 insulation_res_n_critical_threshold_get ( void )
{
    return g_ctl_eerom_value.insulation_res_n_critical;
}


INT32 store_system_data ( UINT16 addr, const UCHAR *srcdata, UINT16 len )
{
    UINT32 real_addr = ADDR_SYSTEM;

    if ( addr + len > EE_SIZE_SYSTEM )
        return -1;

    EE_WRITE ( addr + ADDR_SYSTEM, srcdata, len );
    return 0;
}

INT32 read_system_data ( UINT16 addr, UCHAR *dstdata, UCHAR len )
{
    if ( addr + len > EE_SIZE_SYSTEM )
        return -1;

    EE_READ ( addr + ADDR_SYSTEM, dstdata, len );
    return 0;
}

INT32 store_module_config_data ( UINT16 addr, const UCHAR *srcdata, UCHAR len )
{
    if ( len + addr > EE_SIZE_MODULE_CONFIG )
        return -1;

    EE_WRITE ( addr + ADDR_MODULE_CONFIG, srcdata, len );
    return 0;
}

INT32 read_module_config_data ( UINT16 addr, UCHAR *dstdata, UCHAR len )
{
    if ( addr + len > EE_SIZE_MODULE_CONFIG )
        return -1;

    EE_READ ( addr + ADDR_MODULE_CONFIG, dstdata, len );
    return 0;
}

INT32 store_soc_data ( UINT16 addr, const UCHAR *srcdata, UCHAR len )
{
    if ( len + addr > EE_SIZE_SOC )
        return -1;

    EE_WRITE ( addr + ADDR_SOC, srcdata, len );
    return 0;
}

INT32 read_soc_data ( UINT16 addr, UCHAR *dstdata, UCHAR len )
{
    if ( addr + len > EE_SIZE_SOC )
        return -1;

    EE_READ ( addr + ADDR_SOC, dstdata, len );
    return 0;
}


void printf_ctl_param ( void )
{
    mprintf ( "g_ctl_eerom_value.temp_nor_hi = %d\r\n", g_ctl_eerom_value.temp_nor_hi ); // = 50;
    mprintf ( "g_ctl_eerom_value.temp_warning_hi = %d\r\n", g_ctl_eerom_value.temp_warning_hi ); // = 60;
    mprintf ( "g_ctl_eerom_value.temp_critical_hi  = %d\r\n", g_ctl_eerom_value.temp_critical_hi ); //  = 75;
    mprintf ( "g_ctl_eerom_value.temp_nor_low  = %d\r\n", g_ctl_eerom_value.temp_nor_low ); // =5;
    mprintf ( "g_ctl_eerom_value.temp_warning_low  = %d\r\n", g_ctl_eerom_value.temp_warning_low ); // = -20;
    mprintf ( "g_ctl_eerom_value.temp_critical_low  = %d\r\n", g_ctl_eerom_value.temp_critical_low ); //;

    mprintf ( "g_ctl_eerom_value.vol_critical_hi  = %d\r\n", g_ctl_eerom_value.vol_critical_hi ); // 42000;
    mprintf ( "g_ctl_eerom_value.vol_warning_low  = %d\r\n", g_ctl_eerom_value.vol_warning_low ); // 30000;
    mprintf ( "g_ctl_eerom_value.vol_critical_low  = %d\r\n", g_ctl_eerom_value.vol_critical_low ); // 25000;
    mprintf ( "g_ctl_eerom_value.vol_sum_low  = %d\r\n",  g_ctl_eerom_value.vol_sum_low ); // 50000;
    mprintf ( "g_ctl_eerom_value.vol_sum_high  = %d\r\n", g_ctl_eerom_value.vol_sum_high ); //  84000;

    mprintf ( "g_ctl_eerom_value.CURRENT_CRITICAL_LOW_value = %d\r\n", g_ctl_eerom_value.CURRENT_CRITICAL_LOW_value ); // 400;
}



#define ADDR_WRITE_FLAG 0
#define ADDR_PARAM 1
INT16 store_ctl_param ( void )
{
    return store_system_data ( ADDR_PARAM, &g_ctl_eerom_value, sizeof ( g_ctl_eerom_value ) );
}

int recover_factory_settings ( void )
{
    int ret = 0;
    UINT8 flag = 0xab;
    init_g_clt_value();
    mprintf ( "get a empty eeprom,ready write initial control param to eeprom!\r\n" );
    ret = store_system_data ( ADDR_WRITE_FLAG, &flag, 1 );
    ret = store_ctl_param();
    return ret;
}

/*****************************************************************
 * init control param
 * read from eeprom
 *****************************************************************/
INT16 ctl_init ( void )
{
    UINT8 flag = 0;
    INT16 ret = 0;
    read_system_data ( ADDR_WRITE_FLAG, &flag, 1 );

    if ( flag != 0xab )
    {
        flag = 0xab;
        init_g_clt_value();
        mprintf ( "get a empty eeprom,ready write initial control param to eeprom!\r\n" );
        ret = store_system_data ( ADDR_WRITE_FLAG, &flag, 1 );
        ret = store_ctl_param();
        mprintf ( "write over!\r\n" );

    }

    read_system_data ( ADDR_PARAM, &g_ctl_eerom_value, sizeof ( g_ctl_eerom_value ) );
    printf_ctl_param();
      
    return 0;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
