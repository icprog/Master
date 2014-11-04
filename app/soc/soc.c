
/*********************************************************
*
* ��оԴ΢�������޹�˾(UCT)
*
* File Description
*
*   $Revision: 3603 $
*   $Author: huhai $
*   $Date: 2014-09-10 16:15:04 +0800 (周三, 10 九月 2014) $
*
*********************************************************/
#include "soc.h"
#include "../public/public.h"
#include "string.h"
#include "ocv-soc.h"
#include "T+_soc.h"
#include "T-_soc.h"
#include "ampere-soc.h"
#include "loop-soc.h"
#include "../banlance_battery/banlancebattery.h"


#define CAL_K_USE_TABLE 1
#define DEBUG_SOC print2bank //mprintf
#define M_PRINT mprintf
//#define DEBUG_SOC bank
/*
 *��¼��һ�ε�socֵ
 *�⼸��ֵ������eeprom
 */
//#pragma pack(push)
//#pragma pack(1)

#define SOC_NO_FLOAT 1
#pragma align off

struct _soc_timer
{
    time_type idle ;
    time_type last ;
};

#pragma align off
struct s_g_values
{
#if SOC_NO_FLOAT == 1
    INT8 soc_last[MAX_BATTERYS];
    INT8 soc_last_final;
#else
    float soc_last[MAX_BATTERYS];
    float soc_last_final;
#endif
    UINT16 age_counter;
    UINT16 age_counter_down;
    UINT16 age_counter_blow;
    struct _soc_timer soc_timer;
};

//#pragma pack(pop)
struct s_g_values g_values;

#if SOC_NO_FLOAT == 1
static INT8 g_cur_soc = 0;  //��ǰsocֵ
#else
static float g_cur_soc = 0.0;  //��ǰsocֵ
#endif

#define g_soc_last g_values.soc_last
#define g_age_counter g_values.age_counter
#define g_soc_last_final g_values.soc_last_final
#define g_age_counter_down g_values.age_counter_down
#define g_age_counter_blow g_values.age_counter_blow
#define time_idle g_values.soc_timer.idle
#define time_last g_values.soc_timer.last

#if 0
void DEBUG_SOC ( const char *fmt, ... )
{
    // printf("%s\n", fmt);
    return ;
}
#endif

#if 0
static void printf_array ( const char *array, int len )
{

    int i = 0;

    for ( i = 0; i < len; i++ )
    {
        M_PRINT ( "%02x ", array[i] );
    }

    //M_PRINT( " \r\n" );
}
#endif

UINT16 SOC_DISCHARGETIMES(void)
{
    return g_values.age_counter_blow;
}

UINT16 SOC_CHARGETIMES(void)
{
    return g_values.age_counter_down;
}

void printf_g_value ( struct s_g_values *p )
{
    int i = 0 ;
    M_PRINT ( "g_soc_last:\r\n" );

    for ( i = 0; i < MAX_BATTERYS; i++ )
    {
        M_PRINT ( "[%02d]:%1.4d", i, p->soc_last[i] );

        if ( i == 9 ) M_PRINT ( "\r\n" );
    }

    M_PRINT ( "\r\ng_soc_last_final:%d ", p->soc_last_final );
    M_PRINT ( "age_counter:%d\r\n", p->age_counter );

}
#if USE_EXTERN_EE_RW == 0
void WRITE_TO_EEPROM ( UINT32 addr, const char *buff, int len )
{
    DEBUG_SOC ( "write to eeprom:addr:%04x\r\n", addr );
    DEBUG_SOC ( "%d\r\n", buff[len - 1] );
    // return 0;
}

void READ_FROM_EEPROM ( UINT32 addr, char *buff, int len )
{
    DEBUG_SOC ( "write to eeprom:addr:%04x\r\n", addr );
    DEBUG_SOC ( "%d\n", buff[len - 1] );
// return 0;
}
#endif

/*****************************************************************************
 * ��ȡ������ֵ
 * by mgc 20140627
 ****************************************************************************/
#if USE_SINLE_SENSOR_FUN  == 0
void GetSensorValue ( sensor *sen )
{
    UINT8 i = 0;
    //sen->vol_sum = 0;
    sen->electricity = READ_ELECTRICITY();
    READ_VOL_SONS ( sen->vol_son );

    for ( i = 0; i < MAX_BATTERYS; i++ )
    {
        sen->temperature_son[i] = GET_TEMPERATURE();
//                sen->vol_son[i] += (UINT16)25000;
        sen->Q_son[i] = SUM_Q ;
    }
}
#endif
/*****************************************************************************
 * ����ܵ�����ȡ,��λΪah
 *
 *****************************************************************************/
UINT32 GetCurQsum ( sensor sen ) //��λ a*h
{
    //TODO: �ɸ��ݴ�����ֵ�������ܵ�������ʵʱ����
    if ( SUM_Q == sen.Q_son[0] )
        return sen.Q_son[0];

    return SUM_Q;
}


/****************************************************************************
 *����ʽϵ��ͨ�ü��㺯��
 *���أ�
 *x:�Ա���
 *k:ϵ����
 *len:ϵ���鳤�ȣ��Ը��㳤��Ϊһ��λ)
 ****************************************************************************/
#if CAL_K_USE_TABLE == 0

float polynomial_cal ( const float *k, UINT8 len, float x )
{
    int i = 0;
    float j = 0.0;
    float t = 0.0;
    len = len / sizeof ( float );
    DEBUG_SOC ( "len:%d, x:%d\n", len, x );

    for ( i = 0; i < len; i++ )
    {
        j = i;
        t += ( powf ( x, j ) * k[i] );
        //DEBUG_SOC ( "calcultating t:%f,x:%f,j:%f,i:%d k[i]:%f\n ", t, x, j, i, k[i] );
    }

    return t;
}
#endif

/*һ��ϵ���У�����ϵ�������Ŵ�10000������*/
/***************************************************************************
 *��ȡ�¶�ϵ��
 *��2���¶�-������������
 * P_T = [ -0.0001    0.0018    0.9865]
 * t : �¶�-100~+100
 * ���أ�0~100
 *by mgc 20140627
 ***************************************************************************/
static const float kt[] =
{
    0.9865, 0.0018, -0.0001
};
//���뵥λΪ�¶�
float GetTk ( INT16 t )
{
#if CAL_K_USE_TABLE == 1
    unsigned char T = 0;
    DEBUG_SOC ( "T is:%d\n", t );

    if ( t >= 0 )
    {
        T = 59 - t;

        if ( T > sizeof ( T_soc ) / sizeof ( float ) )
        {
            T = sizeof ( T_soc ) / sizeof ( float );
        }

        return T_soc[T];
    }
    else
    {
        T = 0 - t;

        if ( T > sizeof ( T_N_soc ) / sizeof ( float ) )
        {
            T = sizeof ( T_N_soc ) / sizeof ( float );
        }

        return T_N_soc[T];
    }

#else
    //��λΪ���϶�
    return polynomial_cal ( kt, sizeof ( kt ), t );
#endif
}



/***************************************************************************
 *��ȡ����ϵ��
 *��3  ����-�������ӣ�����Ч�ʣ�����
 *P_I = [0.0036  -0.0158  0  0.0701  -0.0918  1.0214];
 * ���أ�0~100
 *by mgc 20140627
 ***************************************************************************/
static const float ki[] =
{
    1.0214, 0.0918, 0.0701, 0.0, -0.0158, 0.0036
};
/*
 *
 *  i ���뵥λΪ�ŵ籶��(i/c  0~1.0)
 */
float GetIk ( float x )
{
#if CAL_K_USE_TABLE == 1
    INT32 index = ( INT32 ) ( x * 100 );

    if ( index > sizeof ( ampere_soc ) )
    {
        index = sizeof ( ampere_soc );
    }

    DEBUG_SOC ( "%f,%d,%f\n", x, index, ampere_soc[index] );
    return ampere_soc[index];
#else
    // DEBUG_SOC ( "Q:%f,i:%f,i/Q:%f\n", Qsum, a, a / Qsum );
    return polynomial_cal ( ki, sizeof ( ki ), x );
#endif

}

/***************************************************************************
 *��ȡѭ������ϵ��
 *��4��ѭ������-������������
 *P_a = [0.0000  -0.0000  0.0000  -0.0000  -0.0002  1.0004];
 * ���أ�0~100
 *by mgc 20140627
 ***************************************************************************/
static const float kage[] =
{
    1.0004, -0.0002
};
//���뵥λΪѭ������  0��700
float GetAgek ( UINT32 xage )
{
#if 0// CAL_K_USE_TABLE == 1

    if ( xage > sizeof ( loop_soc ) / sizeof ( float ) )
    {
        err ( "err age:%d\n", xage );
        xage = sizeof ( loop_soc ) / sizeof ( float );
    }

    return loop_soc[xage];
#else

    if ( xage >  1000 )
        xage = 1000;

    //printf("xage:%d\n", xage);
    return ( 1.0004 - ( ( float ) xage ) * 0.0002 );
    // return polynomial_cal ( kage, sizeof ( kage ), xage );
#endif

}

/***************************************************************************
 *��ȡϵ����ֵ
 * index ��ǰ���������
 *    Ki/(Kt*Kage)
 * ���أ�0~100
 *by mgc 20140627
 ***************************************************************************/
float GetK ( sensor senval, UINT32 xage, UINT8 index )
{
    float ka , ki, kt, kfinal;
    ka = GetAgek ( xage ); //��λΪ����
    ki = ( ( float ) senval.electricity ) / 10000.0;
    ki = ki / ( float ) ( SUM_Q );//ת�������ټ���Ϊ�ŵ籶��
    ki = GetIk ( ki );
    kt =  GetTk ( senval.temperature_son[index] ); //��λλ���϶�

    kfinal = ( ki ) * ( ka * kt ) ;
    DEBUG_SOC ( "ka:%2.4f,ki:%2.4f, kt:%2.4f, kfinal:%2.4f\n", ka, ki, kt, kfinal );
    return kfinal;
}

#if SOC_NO_FLOAT == 1
void set_soc_last ( INT8 soc, UINT8 index )
#else
void set_soc_last ( float soc, UINT8 index )
#endif
{
    g_soc_last[index] = soc;
//    M_PRINT("soc[%d]:%f\n", index, soc);
}

#if SOC_NO_FLOAT == 1
INT8 get_soc_last ( UINT8 index )
#else
float get_soc_last ( UINT8 index )
#endif
{
    return g_soc_last[index];
}


/****************************************************************************
 * ��ŵ����
 * by mgc 20140627
 ****************************************************************************/
UINT32 GetAge ( void )
{
    return g_age_counter;
}


/***************************************************************************
 *��·��ѹ��ȡsoc ����
 *����: soc(0~100)
 *senval:��ǰ��������Ϣ
 *��1��SOC-OCV����
 *  P_Voc_d = [10.6736 -25.6681 23.4165 -10.2319 2.2875 3.0769]��//�ŵ������
 *  P_Voc_c = [30.8077 -80.4231 79.1778 -36.1815 7.6185 2.7347]��//��������
 *  by mgc 20140627
 ***************************************************************************/
static const float Kv[] =
{
    2.7347, 7.6185, -36.1815, 79.1778, -80.4231, 30.8077
};
//   3.0769, 2.2875, -10.2319, 23.4165, -25.6681, 10.6736
#if SOC_NO_FLOAT == 1
INT8 GetSOCByOCVSingle ( sensor sen , UINT8 index )
#else
float GetSOCByOCVSingle ( sensor sen , UINT8 index )
#endif
{
#if SOC_NO_FLOAT == 0
    float v = sen.vol_son[index]; //�ɼ���ѹΪ100uV Ҫת��v
    float soc = 0.0;
#else
    UINT32 v = sen.vol_son[index];
    INT8 soc = 0;
#endif
    UINT8 i = 0;
    UINT8 size_type = sizeof ( UINT16 );

#if SOC_NO_FLOAT == 0
    v = v / 10000.0;  //ת��Ϊv
#endif
#if CAL_K_USE_TABLE == 1
#if SOC_NO_FLOAT == 1

   // if ( sen.vol_son[index] < 32347 ) v = 32347;

   // if ( sen.vol_son[index] > 41854 ) v = 41854;

#else

    if ( sen.vol_son[index] < 32347 ) v = 3.2347;

    if ( sen.vol_son[index] > 41854 ) v = 4.1854;

#endif
    
    
     for ( i = 0; i < ( sizeof ( ocv_soc ) / size_type ) ; i++)
    {
    //    mprintf("compare i:%d\r\n",  i );
    //    mprintf("compare v:%d\r\n",v ); 
    //    mprintf("compare ocv_soc:%d\r\n" , ocv_soc[i] );
        
        if ( v < ocv_soc[i] )
        {
            break;
        }

       // i = i + 1;
    }

#if SOC_NO_FLOAT == 1
    soc = i;
#else
    soc = ( float ) i / 100.0;
#endif
//    M_PRINT("v:%f->soc:%f\r\n",v, soc);
    set_soc_last ( soc, index );

#else
    soc = polynomial_cal ( Kv, sizeof ( Kv ),  v );
    DEBUG_SOC ( "OCV-SOC:%2.4lf - %2.4lf\n", v, soc );
#endif
    return soc;
}

/***************************************************************************
 *��·��ѹ��ȡsoc
 *����: soc(0~100)
 *senval:��ǰ��������Ϣ
 *  by mgc 20140627
 ***************************************************************************/
#if SOC_NO_FLOAT == 1
INT8 GetSOCByOCV ( sensor sen )
#else
float GetSOCByOCV ( sensor sen )
#endif
{
    UINT8 i = MAX_BATTERYS;
#if SOC_NO_FLOAT == 1
    INT8 soc = 0;
    INT8 soc_final = 100;
#else
    float soc = 0.0;
    float soc_final = 1.0;
#endif

    for ( i = 0; i < MAX_BATTERYS; i++ )
    {
        soc = GetSOCByOCVSingle ( sen , i );
#if SOC_NO_FLOAT == 1

        if ( soc < soc_final ) soc_final = soc;

#else
        soc_final = FLOAT_MIN ( soc, soc_final );
#endif
    }

    //M_PRINT_g_value( &g_values);
#if SOC_NO_FLOAT == 1
    //M_PRINT ( "get soc by v:%dmv->%d\r\n", sen.vol_son[0] / 10, soc_final );
#else
   // M_PRINT ( "get soc by v:%dmv->%1.2f\n", sen.vol_son[0] / 10, soc_final );
#endif

    return soc_final;
}




/****************************************************************************
 * ��ʱ���ַ�����soc
 * time ��λΪs
 * ������λΪmA
 * �¶ȵ�λλ���϶�
 * by mgc20140627
 ****************************************************************************/
#if SOC_NO_FLOAT == 1
INT8 GetSocBymAhByIndex ( sensor sen, time_type time, UINT8 index )
#else
float GetSocBymAhByIndex ( sensor sen, time_type time, UINT8 index )
#endif
{

#if SOC_NO_FLOAT == 1
    INT8 ret = 0,  last_soc,  dsoc ;
    INT16 a = 0, ab = 0;
    INT64 Qt = 0,Qsum = 0;
    float nfix;
#else
    float ret = 0.0, Qsum, a, nfix, last_soc,  dsoc ;
#endif
    const struct balance_t *pb = balance_read(); 
    enum flowdown_state state;
    DEBUG_SOC ( "\nindex:[%d]\n", index );
    /*��ȡ�ܵ���*/
    Qsum = GetCurQsum ( sen ); //��λ a*h
    Qsum = H_TO_S ( Qsum ); //��λa*s
    //Qsum = Qsum/MAX_BATTERYS; //ÿ�ڵ�ص���
    DEBUG_SOC ( "sum q:%4.5d AS\n", Qsum );

    /*��ȡ���� ת��Ϊa*/
    a = sen.electricity ;
    DEBUG_SOC ( "A:%dA\n", a );

    /*��ȡ��������ϵ��*/
    nfix = GetK ( sen, GetAge(), index );
    DEBUG_SOC ( "getk:%2.4f\n", nfix );


    /*��ȡ��һ��socֵ*/
    last_soc = get_soc_last ( index );
    DEBUG_SOC ( "last_soc:%d\n", last_soc );

    /*�ﲻ����������  ����Ϊ��λ ����soc*/
    DEBUG_SOC ( "time:%d\n", time );
    
    if( pb->flags[index] == 1 )
    {
        ab = get_balance_current_by_index( index );
    }
    
    if( a<0 )
    {
        mprintf("err �����������!\r\n");
    }
    state = GetBatteryMode();
    if( state == STATE_DOWN )  
    {      
      a = a * (-1);
    }
    a =  a - ab ;
    Qt = (INT64)( (float)(a*(INT32)time )/(nfix) ); //��ǰ����soc
    
    if(Qt>Qsum)
    {      
      Qt == Qsum;
      mprintf("���ֵ������س��� ����!\r\n");
    }
    
    dsoc = (INT8) ( Qt*(INT8)100/(INT64)Qsum );
//Qt = ( time  ) * ( nfix * 100 ) ; //��ǰ���ֵ���

#if SOC_NO_FLOAT == 1
//dsoc = ( INT8 ) ( Qt / Qsum );
    DEBUG_SOC ( "time:%d, a:%d qsum:%d qt:%d dsoc:%d\n", time, a, Qsum, Qt, dsoc );

    /*�ﲻ���������� ֱ�ӷ��ش���*/
    if ( dsoc == 0  )
    {
        DEBUG_SOC ( " err dsoc < 0\n " );
        return -1;
    }

#else
    dsoc = dsoc * nfix;
#endif

    ret = last_soc + dsoc ;

    /*�����ѹ�ﵽ��ߵ�ѹ��Ĭ��Ϊ�ѳ����磬�޸�SOCΪ1.0*/
    if ( sen.vol_son[index] >= MAX_SON_VOL )
    {
#if SOC_NO_FLOAT == 1
        ret = 100;
#else
        M_PRINT ( "fix soc from %f to 1.0! reason: son v:%d, MAX v:%d\r\n", ret, sen.vol_son[index], MAX_SON_VOL );
        ret = 1.0;
#endif
    }
    
    if(ret<0) 
      ret = 0;
    
    if(ret>100) 
      ret = 100;
    
    //DEBUG_SOC ( "soc is %1.4f\n", ret );
    set_soc_last ( ret, index );
   
    return ret;
}



#if 0
    DEBUG_SOC ( "state:%d\n", state );

    switch ( state )
    {
        case STATE_BLOW :
        {
            ret = last_soc + dsoc;
#if SOC_NO_FLOAT == 1

            if ( ret > 100 || ret < 0 )
                ret = 100;

#else
            DEBUG_SOC ( "ret:%f dsoc:%f last_soc:%f\n", ret, dsoc, last_soc );
            ret = FLOAT_MIN ( ret, 1.0 );
#endif
            break;
        }
        case STATE_DOWN :
        {
            /*
             if ( FLOAT_MAX ( dsoc, last_soc ) == dsoc )
             {
                 // err ( "last soc:%f dsoc:%f soc err FLOAT_IS_BIGGER!!!\n",last_soc, dsoc );
             }
             */
            if ( dsoc > last_soc )
                ret = 0;
            else
                ret = last_soc - dsoc;

#if SOC_NO_FLOAT == 1

            if ( ret < 0 ) ret = 0;

#else
            ret = FLOAT_MAX ( ret, 0.0 );
#endif
            break;
        }
    }
 
#endif

/****************************************************************************
 *INT8 GetSocBymAh
 *if soc <0 means ignore the soc this time;
 ****************************************************************************/
#if SOC_NO_FLOAT == 1
INT8 GetSocBymAh ( sensor sen, time_type time )
#else
//float GetSocBymAh ( sensor sen, time_type time )
#endif
{
    UINT8 i = 0;
#if SOC_NO_FLOAT == 1
    INT8 soc = 0;
#else
    float soc = 0.0;// soc_final = 1.0;
#endif
    //M_PRINT ( "soc_last:%f\n", soc_last ( i ) );
    //M_PRINT ( "get soc by ah\n" );
//return 0;
    for ( i = 0; i < MAX_BATTERYS; i++ )
    {
        soc = GetSocBymAhByIndex ( sen, time, i );
#if SOC_NO_FLOAT == 1

        if ( soc > 100 )
            soc = 100;

        if ( soc == -1 )
            break;

#else
        soc = FLOAT_MIN ( soc, 1.0 );
#endif
    }

    return soc;
}
/****************************************************************************
 *
 * ��ŵ��������
 * ���������ͨ��socά��һ����¼��ŵ�����ļ����� g_age_conter
 ****************************************************************************/
#if SOC_NO_FLOAT == 1
#define CHAGE_THRESHOLD_BOTTOM 20
#define CHAGE_THRESHOLD_TOP 80
#else
#define CHAGE_THRESHOLD_BOTTOM 0.2
#define CHAGE_THRESHOLD_TOP 0.8
#endif

#if SOC_NO_FLOAT == 1
void charge_times_manage ( float soc )
#else
void charge_times_manage ( INT8 soc )
#endif
{
    enum _flag
    {
        FLAG_IDLE = 0,
        FLAG_BLOW = 1,
        FLAG_DOWN = 2
    };
    static UINT8 flag = FLAG_IDLE;

    //M_PRINT("flag:%d\n",flag);
    switch ( flag )
    {
        case FLAG_IDLE:

            if ( soc > CHAGE_THRESHOLD_TOP )
            {
                flag = FLAG_DOWN;
                M_PRINT ( "look forward down\r\n" );
            }

            if ( soc < CHAGE_THRESHOLD_BOTTOM )
            {
                flag = FLAG_BLOW;
                M_PRINT ( "look forward up\r\n" );
            }

            break;
        case FLAG_BLOW:

            if ( soc > CHAGE_THRESHOLD_TOP )
            {
                M_PRINT ( "up ++\r\n" );
                g_age_counter++;
                g_age_counter_down++;
                flag = FLAG_DOWN;
            }

            break;
        case FLAG_DOWN:

            if ( soc < CHAGE_THRESHOLD_BOTTOM )
            {
                M_PRINT ( "down ++\r\n" );
                g_age_counter_blow++;
                g_age_counter++;
                flag = FLAG_BLOW;
            }

            break;
        default:
            break;
    }

}
/****************************************************************************
 * ��ӡ��������Ϣ������
 ****************************************************************************/
void printf_sensor ( sensor *sen )
{
    int i = 0;
    M_PRINT ( "V:\r\n" );

    for ( i = 0; i < MAX_BATTERYS; i++ )
    {
        M_PRINT ( "[%02d]:%dmv ", i, sen->vol_son[i] / 10 );

        if ( i == 9 ) M_PRINT ( "\r\n" );
    }

    M_PRINT ( "\nelec:%dA  ", sen->electricity );
    M_PRINT ( "Qsum:%dAH\r\n", sen->Q_son[0] );

}

/****************************************************************************
 * soc ��ʼ���������ϵ�ʱ����
 ****************************************************************************/
INT16 soc_init ( void )
{
#if SOC_NO_FLOAT == 1
    INT8 soc_initial = 100;
#else
    float soc_initial = 1.0;
#endif
    sensor sen;
    UINT8 i = 0;
    M_PRINT ( "soc init start:\n" );
    //memset( &g_values, 0, sizeof(g_values));
    READ_FROM_EEPROM ( 0, ( char * ) &g_values, sizeof ( g_values ) );
    //M_PRINT ( "read from eeprom:last soc is:%d\n", g_soc_last_final );
    printf_g_value ( &g_values );
    GetSensorValue ( &sen );
    printf_sensor ( &sen );

    /*setup the battery*/
    if ( GetBatteryMode()  == STATE_SETUP )
    {
        g_soc_last_final = 0;
    }

#if SOC_NO_FLOAT == 1

    if ( g_soc_last_final == 0 )
#else
    if ( FLOAT_IS_ZERO ( g_soc_last_final ) )
#endif
    {
        //  printf("get soc by ocv!\r\n");
        soc_initial = GetSOCByOCV ( sen );
    }
    else
    {
        soc_initial = g_soc_last_final;
    }

    if ( soc_initial < 0 )
    {
        //  printf("err!\n");
        soc_initial = 0;
    }

    g_cur_soc = soc_initial;
    printf_g_value ( &g_values );
    M_PRINT ( "soc init over!\r\n" );
    DEBUG_SOC ( "soc init:%d\r\n", soc_initial );
    /*
        for ( i = 0; i < MAX_BATTERYS; i++ )
        {
            set_soc_last ( soc_initial, i );
        }

    */
    return 0;
}

/****************************************************************************
 * ��ȡsocֵ
 * soc_read(void)
 * by mgc 20140627
 ****************************************************************************/             
    sensor sen;
 #include "timer.h"
#if SOC_NO_FLOAT == 1
INT8 soc_read ( void )
#else
float soc_read ( void )
#endif
{
#if SOC_NO_FLOAT == 1
    INT8 soc = 0;
#else
    float soc = 0.0;
#endif
    time_type tnow = 0;
//    static time_type time_idle = 0;
//    static time_type time_last = 0;
    GetSensorValue ( &sen );
    tnow = GetSysTimeBySec();
  //   return  0;
    if ( GetBatteryMode() == STATE_IDLE  )
    {
        if ( get_stop_time() > BATTARY_STABLE_TIME )
        {
            soc = GetSOCByOCV ( sen );
        }
        else
        {
            soc = g_cur_soc;
        }
        time_last = tnow;
    }
  
    else if( GetBatteryMode()  == STATE_SETUP )
    {
        soc = GetSOCByOCV ( sen );   
        g_age_counter_down    = 0;
        g_age_counter_blow      = 0;
        
        time_last = tnow;
    }
    else
    {
        if ( tnow != time_last )
        {
            //M_PRINT("tnow:%d, tlast:%d, tnow-tlast:%d", tnow, time_last, tnow-time_last);
            time_idle = TIME_TRHESHOLD_CAL( tnow, time_last );
            soc = GetSocBymAh ( sen, time_idle );
           
            if ( soc == -1 ) //�ﲻ��soc�������� ������ʱ��
                soc = g_cur_soc;
            else
                time_last = tnow;
        }
        else
        {
            soc = g_cur_soc;
        }
        
         time_idle = tnow;

    }

    g_soc_last_final = g_cur_soc;
    DEBUG_SOC ( "last soc is:%d\r\n", g_soc_last_final );
    g_cur_soc = soc;
    charge_times_manage ( soc );
    if( g_soc_last_final != soc )
    {       
      M_PRINT ( "soc:%d->%d\n", g_soc_last_final, soc );
    }
    //printf_sys_time();
    //M_PRINT("write to eeprom!");      
    g_age_counter_down    =   g_age_counter_blow ;
    WRITE_TO_EEPROM ( 0, ( char * ) &g_values, sizeof ( g_values ) );

    return soc;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
