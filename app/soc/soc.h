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

#define PRECISION 0.00001              //���㾫��

#define MAX_BATTERYS   24 // LTC6804_IC_NUM * LTC6804_MAX_VOL                    //��ذ���
#define H_TO_S_f(h) ((h)*60.0*60.0)  //Сʱת��Ϊ�룬����
#define H_TO_S(h) ((h)*60*60)  //Сʱת��Ϊ�룬����

#define FLOAT_IS_BIGGER(fa,fb) (( (fa)-(fb)>PRECISION )?1:0) //����Ƚ�
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
#define SUM_Q 100 //����ܵ����� ��λΪ��ʱ�������ṩ
#define BATTARY_STABLE_TIME H_TO_S(1) //����ȶ�ʱ�� ���Կ�·��ѹ������ ��λΪS

#define MAX_SON_VOL 42000 //���ڵ������ѹ  �����޸�socֵ ��λ 0.1mv
#define SUM_VOL   MAX_SON_VOL*MAX_BATTERYS
#define SOC_NO_FLOAT 1 //�����ø��㣬������1 ����Ϊ0
#define err mprintf

typedef struct _sensors
{
    UINT16 vol_son[MAX_BATTERYS]; //��λ100uV
    INT16 temperature_son[MAX_BATTERYS]; // C
    UINT16 Q_son[MAX_BATTERYS];  //��λΪAH
    UINT16 electricity; //��λ ����
} sensor;
//
///*��ŵ�״̬*/
//enum flowdown_state
//{
//    STATE_BLOW = 1,
//    STATE_DOWN = 3,
//    STATE_IDLE = 0,
//    STATE_SETUP = 2,
//};

/*���ⲿ���ú���*/
INT16 soc_init ( void );  //soc �ϵ��ʼ��
#if SOC_NO_FLOAT == 1
INT8 soc_read ( void );
#else
float soc_read ( void );  //��ȡsocֵ
#endif

UINT16 SOC_DISCHARGETIMES ( void );
UINT16 SOC_CHARGETIMES ( void );

/*�ⲿ���ṩ����*/

extern UINT32 GetSysTimeBySec ( void ); //��ȡϵͳʱ�䣬��λΪ S
extern enum flowdown_state GetBatteryMode ( void ); //��ȡ��ع���ģʽ(���1/�ŵ�-1/����0) �ⲿ�ṩ����

#define USE_SINLE_SENSOR_FUN 0  //�Ƿ�ʹ��һ�������ɼ����д�����ֵ
#if USE_SINLE_SENSOR_FUN == 0

#define READ_ELECTRICITY()  get_current()//2000 //��ȡ�ܵ�������λ100uA �ⲿ�ṩ����
#define READ_VOL_SONS( dst_addr )  get_voltage( dst_addr )//��ȡÿ����ص�ѹֵ �ⲿ�ṩ����
#define GET_TEMPERATURE() (25)  //��ȡ�¶�
extern void get_voltage ( UINT16 *vol ); //��ȡ��ѹ ÿ�ڵ�ص�ѹ��volΪһ������ ��λΪ 0.1mV

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
