
#ifndef _TYPES_H_
#define _TYPES_H_

typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef signed int INT32;
typedef unsigned long long UINT64;
typedef signed long long INT64;
typedef unsigned char UCHAR;
typedef char byte;
typedef volatile unsigned long long VUINT64;
typedef volatile unsigned int VUINT32;
typedef UINT16 CURRENT_TYPE;
typedef INT16 TYPE_TEMP;
typedef UINT16 TYPE_CURRENT;
typedef UINT16 TYPE_VOLTAGE;
typedef UINT16 TYPE_INSRES;

enum TYPE_CHECK
{
	CHECK_NG = 0,
	CHECK_PASS = 1
};

enum TYPE_DEVICE_CONNECT
{
	DEV_DISCONNECT = 0,
	DEV_CONNECTED = 1
};

enum TYPE_REALY_SW
{
	RELAY_OFF = 0,
	RELAY_ON = 1
};

enum TYPE_ERRO
{
	CHECK_ERR_NONE = 0,
	CHECK_ERR = 1
};

#endif
