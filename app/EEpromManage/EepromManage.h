#ifndef EEPROM_H_
#define EEPROM_H_
#include "../../driver/rom/rom.h"

/*MAP*/
#define EE_SIZE_SYSTEM 1024
#define EE_SIZE_MODULE_CONFIG 1024
#define EE_SIZE_SOC  10240

#define ADDR_INIT 0
#define ADDR_SYSTEM ADDR_INIT
#define ADDR_MODULE_CONFIG (ADDR_SYSTEM + EE_SIZE_SYSTEM)
#define ADDR_SOC ( ADDR_MODULE_CONFIG + EE_SIZE_SOC )

/*eeprom interface*/

/*offered by other module*/
#define EE_READ( ADDR, DATA, LEN ) ReadEprom( ADDR, LEN, DATA )
#define EE_WRITE( ADDR, DATA, LEN ) WriteEprom( ADDR, LEN, DATA )


/*open to other module*/
INT32 store_system_data ( UINT16 addr, const UCHAR *srcdata, UINT16 len );
INT32 read_system_data ( UINT16 addr, UCHAR *dstdata, UINT16 len );
INT32 store_module_data ( UINT16 addr, const UCHAR *srcdata, UINT16 len );
INT32 read_system_data ( UINT16 addr, UCHAR *dstdata, UINT16 len );
INT32 store_soc_data ( UINT16 addr, const UCHAR *srcdata, UINT16 len );
INT32 read_soc_data ( UINT16 addr, UCHAR *dstdata, UINT16 len );
int recover_factory_settings ( void );

/*some threathold value sets*/
INT8 temp_nor_hi_threshold_get ( void );
enum UCT_ERR_CODE temp_warning_hi_threshold_set ( INT8 temp );
INT8 temp_warning_hi_threshold_get ( void );
enum UCT_ERR_CODE temp_critical_hi_threshold_set ( INT8 temp );
INT8 temp_critical_hi_threshold_get ( void );
INT8 temp_nor_low_threshold_get ( void );
enum UCT_ERR_CODE temp_warning_low_threshold_set ( INT8 temp );
INT8 temp_warning_low_threshold_get ( void );
enum UCT_ERR_CODE temp_critical_low_threshold_set ( INT8 temp );
INT8 temp_critical_low_threshold_get ( void );
UINT16 temp_warning_low_event_get ( void );
UINT16 temp_warning_high_event_get ( void );
UINT16 temp_critical_event_get ( void );
UINT16 temp_edge_event_get ( void );

/*the control threshold of volatage*/
UINT32 vol_sum_high_threshold_get ( void );
enum UCT_ERR_CODE    vol_sum_high_threshold_set ( UINT32 v );

UINT32 vol_sum_low_threshold_get ( void );
enum UCT_ERR_CODE    vol_sum_low_threshold_set ( UINT32 v ) ;

enum UCT_ERR_CODE vol_critical_hi_threshold_set ( UINT16 vol );
UINT16 vol_critical_hi_threshold_get ( void );

enum UCT_ERR_CODE vol_critical_low_threshold_set ( UINT16 vol );
UINT16 vol_critical_low_threshold_get ( void );

enum UCT_ERR_CODE vol_warning_low_threshold_set ( UINT16 vol );
UINT16 vol_warning_low_threshold_get ( void );

enum UCT_ERR_CODE vol_warning_hi_threshold_set ( UINT16 vol );
UINT16 vol_warning_hi_threshold_get ( void );


/*the control threshold of current*/
enum UCT_ERR_CODE CURRENT_CRITICAL_LOW_SET ( UINT16 current );
UINT16 CURRENT_CRITICAL_LOW_GET ( void );

enum UCT_ERR_CODE CURRENT_WARNNING_LOW_SET ( UINT16 current );
UINT16 CURRENT_WARNNING_LOW_GET ( void );


/*the threathold value  of insulation res */
enum UCT_ERR_CODE insulation_res_p_warning_threshold_set ( UINT16 vol );
UINT16 insulation_res_p_warning_threshold_get ( void );

enum UCT_ERR_CODE insulation_res_p_critical_threshold_set ( UINT16 vol );
UINT16 insulation_res_p_critical_threshold_get ( void );

enum UCT_ERR_CODE insulation_res_n_warning_threshold_set ( UINT16 vol );
UINT16 insulation_res_n_warning_threshold_get ( void );

enum UCT_ERR_CODE insulation_res_n_critical_threshold_set ( UINT16 vol );
UINT16 insulation_res_n_critical_threshold_get ( void );

enum DEV_CTRL_TYPE_DEF
{
    DEV_CTRL_TYPE_COOLING = 0,
    DEV_CTRL_TYPE_HEAT,
    DEV_CTRL_TYPE_TEMP_THRESHOLD,
    DEV_CTRL_TYPE_VOL_THRESHOLD,
    DEV_CTRL_TYPE_CHARGE_SWITCH,
    DEV_CTRL_CURRENT_CRITICAL_LOW_SET,
    DEV_CTRL_CURRENT_CRITICAL_LOW_GET,
    DEV_CONTROL_SET_V_SUM,
    DEV_CONTROL_GET_V_SUM
};


struct ctl_eerom_value
{
    INT8 temp_nor_hi;
    INT8 temp_warning_hi;
    INT8 temp_critical_hi;
    INT8 temp_nor_low;
    INT8 temp_warning_low;
    INT8 temp_critical_low;

    UINT16 vol_warning_low ;
    UINT16 vol_warning_hi;
    UINT16 vol_critical_hi ;
    UINT16 vol_critical_low;
    UINT32 vol_sum_low;
    UINT32 vol_sum_high ;

    UINT16 CURRENT_CRITICAL_LOW_value ;
    UINT16 CURRENT_WARNNING_LOW_value;

    UINT16 insulation_res_p_warnning;
    UINT16 insulation_res_p_critical;

    UINT16 insulation_res_n_warnning;
    UINT16 insulation_res_n_critical;

};


#endif

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
