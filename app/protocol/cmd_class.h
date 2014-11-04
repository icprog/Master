
/*********************************************************
*
* 锟斤拷??????????????????锟斤拷??????????????????(UCT)
*
* File Description
*
*   $Revision: 3403 $
*   $Author: huhai $
*   $??锟斤拷??????锟斤拷??????????????锟斤拷??????????锟斤拷??????????????????锟斤拷
*   $Date: 2014-09-01 17:02:05 +0800 (鍛ㄤ竴, 01 涔濇湀 2014) $
*
*********************************************************/
#ifndef _CMD_CLASS_H_
#define _CMD_CLASS_H_

#include "public/public.h"
#include "ctl/control.h"
#include "EEpromManage/EepromManage.h"
typedef struct cmd_class
{
    const char *cmd_name;
    UCHAR cmd_word;
    INT16 ( *process ) ( UCHAR argc, const UCHAR *argv[] );
} Ccmd;
/*
00	锟斤拷锟斤拷	-
01	锟斤拷锟斤拷	锟斤拷锟斤拷位锟斤拷锟斤拷锟斤拷锟斤拷
02	Get锟芥本锟斤拷	锟斤拷锟斤拷锟轿伙拷锟芥本锟斤拷
03	锟斤拷锟斤拷锟�	确锟较达拷锟斤拷锟斤拷锟斤拷锟角凤拷锟斤拷
04	Get锟借备锟斤拷息	锟斤拷玫锟窖癸拷锟斤拷锟斤拷锟斤拷锟绞ｏ拷锟斤拷锟斤拷锟斤拷锟铰度★拷锟斤拷锟斤拷状态锟斤拷锟斤拷锟饺帮拷状态锟斤拷锟斤拷诺锟阶刺拷锟斤拷锟较�
05	Get锟斤拷锟斤拷锟窖癸拷锟斤拷锟街�	锟斤拷玫锟角帮拷锟斤拷玫墓锟窖怪碉拷锟角费怪�
06	Get锟铰讹拷锟斤拷锟斤拷值	锟斤拷取锟斤拷前锟斤拷锟矫的癸拷锟斤拷值锟斤拷锟斤拷锟街�
07	Get锟斤拷诺锟斤拷锟斤拷	锟斤拷玫锟截的筹拷锟斤拷锟斤拷锟斤拷诺锟斤拷锟斤拷
08	Set锟斤拷锟斤拷锟窖癸拷锟斤拷锟街�	锟斤拷锟矫碉拷压锟侥癸拷压值锟斤拷欠压值
09	Set锟铰讹拷锟斤拷锟斤拷值	锟斤拷锟斤拷锟铰度的癸拷锟斤拷值锟斤拷锟斤拷锟街�
0a  	Set锟斤拷锟饺匡拷锟斤拷	锟斤拷锟狡凤拷锟饺打开伙拷囟锟�
0b	Set锟斤拷锟饺板开锟斤拷	锟斤拷锟狡硷拷锟饺帮拷蚩锟截讹拷
0c	    Set锟斤拷诺缈拷锟�	锟斤拷锟狡筹拷诺锟津开伙拷囟锟�
0d	锟斤拷锟皆匡拷锟斤拷	锟角斤拷锟斤拷锟斤拷模式锟斤拷锟角诧拷锟斤拷模式
0e	Set锟斤拷压偏锟斤拷值	锟斤拷锟矫碉拷锟斤拷锟窖蛊拷锟街�
0f  	Get锟侥达拷锟斤拷值	锟斤拷取锟缴硷拷芯片LTC6804锟斤拷锟叫匡拷锟矫寄达拷锟斤拷锟斤拷值
10	Set锟侥达拷锟斤拷值	锟斤拷锟矫采硷拷芯片LTC6804某一锟斤拷锟侥达拷锟斤拷锟斤拷值
11	Get锟斤拷锟斤拷锟斤拷锟斤拷值	锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷值
12	Set锟斤拷锟斤拷锟斤拷锟斤拷值	锟斤拷锟矫碉拷锟斤拷锟斤拷锟斤拷值
13	Get锟杰碉拷压锟斤拷锟斤拷值	锟斤拷取锟斤拷锟斤拷锟斤拷艿锟窖癸拷锟斤拷锟斤拷锟街�
14	Set锟杰碉拷压锟斤拷锟斤拷值	锟斤拷锟矫碉拷锟斤拷锟斤拷艿锟窖癸拷锟斤拷锟斤拷锟街�
15	Get锟斤拷压偏锟斤拷值	锟斤拷取锟斤拷压偏锟斤拷值
*/
enum cmd_enum
{
    CMD_HAND_SHAKE = 0x01,
    CMD_GET_VER = 0x02,
    CMD_HEART_BEAT = 0x03,
    CMD_GET_DEVINFO = 0x04,
    CMD_GET_THRESHOLD_V = 0x05,
    CMD_GET_THRESHOLD_T = 0x06,
    CMD_GET_CHAGE_TIMES = 0x07,
    CMD_SET_THRESHOLD_V = 0x08,
    CMD_SET_THRESHOLD_T = 0x09,
    CMD_SET_FAN_SW = 0x0a,
    CMD_SET_HEATING_APPLAINCE = 0x0b,
    CMD_SET_CHANGE_SW = 0x0c,
    CMD_SET_TEST_MODE = 0x0d,
    CMD_SET_VOL_DEVIATION = 0x0e,
    CMD_GET_REGISTER_VALUE = 0x0f,
    CMD_SET_REGISTER_VALUE = 0x10,
    CMD_GET_THRESHOLD_CURRENT = 0x11,
    CMD_SET_THRESHOLD_CURRENT = 0x12,
    CMD_GET_THRESHOLD_SUM_VOL = 0x13,
    CMD_SET_THRESHOLD_SUM_VOL = 0x14,
    CMD_GET_THRESHOLD_OFFSET_V = 0x15,
    CMD_GET_INSULATION_RESISTANCE = 0x16,
    CMD_EXCEPTION_TEST = 0x17,
    CMD_RECOVER_SYSTEM_SETS = 0x18// p first n last
};


#ifdef _USE_IAR
#pragma pack(push)
#pragma pack(1)
#endif

#ifdef _USE_CODEWARRIOR
#pragma align off
#endif




struct threshold_v
{
    UINT16 overpressure;
    UINT16 undervoltage;
    UINT16 overpressure_critical;
    UINT16 undervoltage_critical; 
};
struct threshold_t
{
    UINT8 hightemperature;
    UINT8 lowtemperature;
    UINT8 hightemperature_critical;
    UINT8 lowtemperature_critical;
};
#ifdef _USE_IAR
#pragma pack(pop)
#endif

#define DECLARE_CMD( CMD ) { #CMD, CMD, FUNC_##CMD }
#define DECLARE_FUNC( CMD ) INT16 FUNC_##CMD( UCHAR argc,const UCHAR *argv[] )

static const UINT16 SVNCODE  = 12345;
static const UCHAR PLATFORMCODE = 0 ;
static const UCHAR CLIENTCODE = 1;
static const UCHAR HARDWARECODE = 0;
static  UCHAR software_ver[] = {1, 2, 3, 4, 5};

DECLARE_FUNC ( CMD_HAND_SHAKE );
DECLARE_FUNC ( CMD_GET_VER );
DECLARE_FUNC ( CMD_HEART_BEAT );
DECLARE_FUNC ( CMD_GET_DEVINFO );
DECLARE_FUNC ( CMD_GET_THRESHOLD_V );
DECLARE_FUNC ( CMD_GET_THRESHOLD_T );
DECLARE_FUNC ( CMD_GET_CHAGE_TIMES );
DECLARE_FUNC ( CMD_SET_THRESHOLD_V );
DECLARE_FUNC ( CMD_SET_THRESHOLD_T );
DECLARE_FUNC ( CMD_SET_FAN_SW );
DECLARE_FUNC ( CMD_SET_HEATING_APPLAINCE );
DECLARE_FUNC ( CMD_SET_CHANGE_SW );
DECLARE_FUNC ( CMD_SET_TEST_MODE );
DECLARE_FUNC ( CMD_SET_VOL_DEVIATION );
DECLARE_FUNC ( CMD_GET_REGISTER_VALUE );
DECLARE_FUNC ( CMD_SET_REGISTER_VALUE );
DECLARE_FUNC ( CMD_GET_THRESHOLD_CURRENT );
DECLARE_FUNC ( CMD_SET_THRESHOLD_CURRENT );
DECLARE_FUNC ( CMD_GET_THRESHOLD_SUM_VOL );
DECLARE_FUNC ( CMD_SET_THRESHOLD_SUM_VOL );
DECLARE_FUNC ( CMD_GET_THRESHOLD_OFFSET_V );
DECLARE_FUNC ( CMD_GET_INSULATION_RESISTANCE );
DECLARE_FUNC ( CMD_EXCEPTION_TEST );
DECLARE_FUNC ( CMD_RECOVER_SYSTEM_SETS );
static const Ccmd g_uart_cmds[] =
{
    DECLARE_CMD ( CMD_HAND_SHAKE ),
    DECLARE_CMD ( CMD_GET_VER ),
    DECLARE_CMD ( CMD_HEART_BEAT ),
    DECLARE_CMD ( CMD_GET_DEVINFO ),
    DECLARE_CMD ( CMD_GET_THRESHOLD_V ),
    DECLARE_CMD ( CMD_GET_THRESHOLD_T ),
    DECLARE_CMD ( CMD_GET_CHAGE_TIMES ),
    DECLARE_CMD ( CMD_SET_THRESHOLD_V ),
    DECLARE_CMD ( CMD_SET_THRESHOLD_T ),
    DECLARE_CMD ( CMD_SET_FAN_SW ),
    DECLARE_CMD ( CMD_SET_HEATING_APPLAINCE ),
    DECLARE_CMD ( CMD_SET_CHANGE_SW ),  
    DECLARE_CMD ( CMD_SET_TEST_MODE ),
    DECLARE_CMD ( CMD_SET_VOL_DEVIATION ),
    DECLARE_CMD ( CMD_GET_REGISTER_VALUE ),
    DECLARE_CMD ( CMD_SET_REGISTER_VALUE ),
    DECLARE_CMD ( CMD_GET_THRESHOLD_CURRENT ),
    DECLARE_CMD ( CMD_SET_THRESHOLD_CURRENT ),
    DECLARE_CMD ( CMD_GET_THRESHOLD_SUM_VOL ),
    DECLARE_CMD ( CMD_SET_THRESHOLD_SUM_VOL ),
    DECLARE_CMD ( CMD_GET_THRESHOLD_OFFSET_V ),
    DECLARE_CMD ( CMD_GET_INSULATION_RESISTANCE ),
    DECLARE_CMD ( CMD_EXCEPTION_TEST ),
    DECLARE_CMD ( CMD_RECOVER_SYSTEM_SETS )
};

static const UCHAR g_uart_cmd_len = sizeof ( g_uart_cmds ) / sizeof ( Ccmd );

/*external funcs*/
#define SET_FAN( sw ) //set the switch of the fan( 0锟截憋拷 1锟斤拷) 
#define GET_CUR_DISCHARGETIMES() SOC_DISCHARGETIMES()//1
#define GET_CUR_CHARGETIMES() SOC_CHARGETIMES()  //1
extern enum UCT_ERR_CODE device_control(UINT8 dev, UINT8 len, const UINT8 *data);





#endif



// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
