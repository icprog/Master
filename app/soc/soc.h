#ifndef _SOC_H_
#define _SOC_H_
#include <stdio.h>
#include "public/public.h"
#include "current/current.h"
//#include "ltc6804.h"



#if 0
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef short INT16;
typedef char INT8;
#endif

typedef UINT32 time_type;

#define mA
#define mV
#define AH
#define uV100

#define PRECISION 0.00001              //浮点精度

#define MAX_BATTERYS   24 // LTC6804_IC_NUM * LTC6804_MAX_VOL                    //电池包数
#define H_TO_S_f(h) ((h)*60.0*60.0)  //小时转化为秒，浮点
#define H_TO_S(h) ((h)*60*60)  //小时转化为秒，浮点

#define FLOAT_IS_BIGGER(fa,fb) (( (fa)-(fb)>PRECISION )?1:0) //浮点比较
#define FLOAT_MIN( a, b)\
    ((a)-(b) > PRECISION ? (b):(a))

#define FLOAT_MAX( a, b)\
    ((a)-(b) > PRECISION ?(a):(b))

#define FLOAT_EQUAL(a,b)\
    ( (a) - (b) < PRECISION && (a) - (b) > -PRECISION )

#define FLOAT_IS_ZERO(a)\
    ( (a) < PRECISION && (a) > -PRECISION )

#define H_TO_S(h) ((h)*60*60)
#define M_TO_S(m) ((m)*60)
#define SUM_Q 100 //电池总电量， 单位为安时，厂家提供
#define BATTARY_STABLE_TIME H_TO_S(1) //电池稳定时间 用以开路电压的修正 单位为S

#define MAX_SON_VOL 42000 //单节电池最大电压  用以修复soc值 单位 0.1mv
#define SUM_VOL   MAX_SON_VOL*MAX_BATTERYS
#define SOC_NO_FLOAT 1 //不起用浮点，将此置1 否则为0
#define err mprintf

typedef struct _sensors
{
    UINT16 vol_son[MAX_BATTERYS]; //单位100uV
    INT16 temperature_son[MAX_BATTERYS]; // C
    UINT16 Q_son[MAX_BATTERYS];  //单位为AH
    UINT16 electricity; //单位 安培
} sensor;
//
///*冲放电状态*/
//enum flowdown_state
//{
//    STATE_BLOW = 1,
//    STATE_DOWN = 3,
//    STATE_IDLE = 0,
//    STATE_SETUP = 2,
//};

/*供外部调用函数*/
INT16 soc_init ( void );  //soc 上电初始化
#if SOC_NO_FLOAT == 1
INT8 soc_read ( void );
#else
float soc_read ( void );  //读取soc值
#endif

UINT16 SOC_DISCHARGETIMES ( void );
UINT16 SOC_CHARGETIMES ( void );

/*外部需提供函数*/

extern UINT32 GetSysTimeBySec ( void ); //获取系统时间，单位为 S
extern enum flowdown_state GetBatteryMode ( void ); //读取电池工作模式(充电1/放电-1/空闲0) 外部提供函数

#define USE_SINLE_SENSOR_FUN 0  //是否使用一个函数采集所有传感器值
#if USE_SINLE_SENSOR_FUN == 0

#define READ_ELECTRICITY()  get_current()//2000 //读取总电流，单位100uA 外部提供函数
#define READ_VOL_SONS( dst_addr )  get_voltage( dst_addr )//读取每个电池电压值 外部提供函数
#define GET_TEMPERATURE() (25)  //读取温度
extern void get_voltage ( UINT16 *vol ); //读取电压 每节电池电压，vol为一个数组 单位为 0.1mV

#else

void GetSensorValue ( sensor *sen );
#endif

#define USE_EXTERN_EE_RW 1
#if USE_EXTERN_EE_RW == 1
#define WRITE_TO_EEPROM  (void)store_soc_data
#define READ_FROM_EEPROM   (void)read_soc_data

//#define WRITE_TO_EEPROM ( addr, buff, len ) (void)store_soc_data ( addr, (const char*)(buff), len )
//#define READ_FROM_EEPROM ( addr, buff, len ) (void)read_soc_data ( addr, (char *)(buff), len )

//extern int WRITE_TO_EEPROM ( UINT32 addr, const char *buff, int len );
//extern int READ_FROM_EEPROM ( UINT32 addr, char *buff, int len );
#endif



#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
