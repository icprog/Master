#include "stdlib.h"
#include "../public/public.h"
#ifndef _MGCFIFO_H_
#define _MGCFIFO_H_
/*
 * 最大容量UCHAR 个元�? * 每个元素最大为UCHAR字节
 */
struct mgcfifo
{
    UCHAR fifo_size;    
    UCHAR element_size; 
    UCHAR volatile mutex;
    UCHAR volatile head;  
    UCHAR volatile tail; 
    UCHAR volatile nvalid;
    UCHAR *buff;  
};

/*计算需要申请的空间大小*/
#define MGCFIFO_BUFF_CALCULATE(elements_size, fifo_size) (sizeof(struct mgcfifo) + (fifo_size)*(elements_size) )

/********************************************************************************
 * mgcfifo_alloc()
 * ret��if >0 fifo size((buffsize-sizeof(struct mgcfifo)/elements), else <=0 err;
 * pfifo: if success,  point to the fifo, else NULL;
 * buff: the buff, where user must alloc themsleves;
 * buffsize: sizeof buff
 * elementsize: sizeof elements
 * you must finish this func before use the fifo, and before interrupt
 * by mgc 20140708
 ********************************************************************************/
int
mgcfifo_alloc(struct mgcfifo **pfifo, char *buff, UINT16 buffsize,
    UCHAR elementsize);

int
mgcfifo_in(struct mgcfifo *pfifo, const char *srcbuff); //��������Ϊһ��elements
int
mgcfifo_out(struct mgcfifo *pfifo, char *destbuff); //��������Ϊһ�� elements
int
mgcfifo_size(struct mgcfifo *pfifo);
int
mgcfifo_nvalid(struct mgcfifo *pfifo);

#endif

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
