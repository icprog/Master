

#ifndef __PE_Types_H
#define __PE_Types_H

#ifdef __MISRA__
  #ifndef FALSE
    #define  FALSE  0u
  #endif
  #ifndef TRUE
    #define  TRUE   1u
  #endif
#else
  #ifndef FALSE
    #define  FALSE  0
  #endif
  #ifndef TRUE
    #define  TRUE   1
  #endif
#endif

/* Freescale types */
typedef unsigned char       VUINT8;
typedef signed char         VINT8;
typedef unsigned short int  VUINT16;
typedef signed short int    VINT16;
typedef unsigned long int   VUINT32;

/* Aditional standard ANSI C types */
#ifndef int8_t
typedef signed char int8_t;
#endif
#ifndef int16_t
typedef signed short   int16_t;
#endif
#ifndef int32_t
typedef signed long int    int32_t;
#endif

#ifndef uint8_t
typedef unsigned char       uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short  uint16_t;
#endif
#ifndef uint32_t
typedef unsigned long int   uint32_t;
#endif
#ifndef TPE_Float
typedef float TPE_Float;
#endif
#ifndef char_t
typedef char char_t;
#endif

/*Types definition*/

typedef unsigned char boolean_t;
typedef unsigned char byte;
typedef unsigned int word;
typedef unsigned long dword;
typedef unsigned long dlong[2];
typedef void (*tIntFunc)(void);
typedef uint8_t TPE_ErrCode;


#endif /* __PE_Types_H */

