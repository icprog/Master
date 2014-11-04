/*********************************************************
 *
 * ��оԴ΢�������޹�˾(UCT)
 *
 * File Description
 *
 *   $Revision: 2342 $
 *   $Author: huhai $
 *   $Date: 2014-07-29 09:49:28 +0800 (星期�? 29 七月 2014) $
 *
 *********************************************************/
#include "mgclib\mgcfifo.h"
#include <string.h>
#include "mgclib\mgcstring.h"
#include "stdlib.h"
#include "stdio.h"
struct mgcfifo m_fifo;
#define EXIT_CRITICAL()
#define ENTER_CRITICAL()

#define DEBUG_FIFO print2bank //mprintf // print2bank
//#define DEBUG_FIFO_ARRAY printf_array_hex
//#pragma INLINE
///*
static void
DEBUG_FIFO_ARRAY(UCHAR* array, UCHAR len)
{
  (void) array;
  (void) len;
}
//*/
#define MUTEX_LOCK(  mutex) /*do\
    {\
        while( (mutex) != 0 );\
        ENTER_CRITICAL();\
        (mutex) = 1;\
        EXIT_CRITICAL();\
    }while(0)                 */

#define MUTEX_UNLOCK(  mutex) /*do\
    {\
        ENTER_CRITICAL();\
        (mutex) = 0;\
        EXIT_CRITICAL();\
    }while(0)                   */

int
mgcfifo_alloc(struct mgcfifo **pfifo, char *buff, UINT16 buffsize,
    UCHAR elementsize)
{
  if (buffsize < elementsize + sizeof(struct mgcfifo))
    {
      pfifo = NULL;
      return 0;
    }

  (void) memset(buff, 0x55, buffsize);
  *pfifo = (struct mgcfifo *) buff;
  (*pfifo)->buff = (UCHAR *) &(buff[sizeof(struct mgcfifo)]);
  (*pfifo)->element_size = elementsize;
  (*pfifo)->fifo_size = (UCHAR) ((buffsize - sizeof(struct mgcfifo))
      / elementsize);
  (*pfifo)->head = 0;
  (*pfifo)->tail = 0;
  (*pfifo)->mutex = 0;

  return (*pfifo)->fifo_size;
}

static void
mgcfifo_headinc(struct mgcfifo *pfifo)
{
  pfifo->mutex = 1;
  pfifo->head += pfifo->element_size;
  if (pfifo->head == pfifo->fifo_size)
    {
      pfifo->head = 0;
    }

}

static void
mgcfifo_tailinc(struct mgcfifo *pfifo)
{
  pfifo->mutex = 1;
  pfifo->tail += pfifo->element_size;
  if (pfifo->tail == pfifo->fifo_size)
    {
      pfifo->tail += pfifo->element_size;
      pfifo->tail = 0;
    }
}

int
mgcfifo_in(struct mgcfifo *pfifo, const char *srcbuff) //拷贝长度为一个elements
{
  int ret = 0;
  static UINT16 i = 0;
  if (pfifo->fifo_size == 0)
    {
      while (1)
        mprintf("err fifosize == 0!\r\n");
    }

  MUTEX_LOCK ( pfifo->mutex );
  if (pfifo->nvalid != 0)
    {
      mgcfifo_headinc(pfifo);
    }
  (void) memcpy(pfifo->head + pfifo->buff, srcbuff, pfifo->element_size);

  if (pfifo->nvalid == pfifo->fifo_size)
    {
      mgcfifo_tailinc(pfifo);
    }
  else
    {
      pfifo->nvalid++;
    }

  DEBUG_FIFO("fifo in%d:", i++);
  (void) DEBUG_FIFO_ARRAY(pfifo->head + pfifo->buff, pfifo->element_size);
  DEBUG_FIFO("\r\n");
  ret = pfifo->nvalid;
  MUTEX_UNLOCK ( pfifo->mutex );
  return ret;
}

int
mgcfifo_out(struct mgcfifo *pfifo, char *destbuff) //拷贝长度为一�?elements
{
  int ret = 0;
  static UINT16 i = 0;

  if (pfifo->fifo_size == 0)
    {
      while (1)
        mprintf("err fifosize == 0!\r\n");
    }

  MUTEX_LOCK ( pfifo->mutex );

  if (pfifo->nvalid == 0)
    {
      ret = -1;
      goto out;
    }

  DEBUG_FIFO("fifo out %d:", i++);
  (void) DEBUG_FIFO_ARRAY(pfifo->tail + pfifo->buff, pfifo->element_size);
  DEBUG_FIFO("\r\n");
  (void) memcpy(destbuff, pfifo->tail + pfifo->buff, pfifo->element_size);
//  if( pfifo->nvalid > 1)
  if (pfifo->tail != pfifo->head)
    {
      mgcfifo_tailinc(pfifo);
    }

  pfifo->nvalid--;

  ret = pfifo->nvalid;
  out: MUTEX_UNLOCK ( pfifo->mutex );
  return ret;
}

int
mgcfifo_size(struct mgcfifo *pfifo)
{
  return pfifo->fifo_size;
}

int
mgcfifo_nvalid(struct mgcfifo *pfifo)
{
  UCHAR ret = 0;
  MUTEX_LOCK ( pfifo->mutex );
  ret = pfifo->nvalid;
  MUTEX_UNLOCK ( pfifo->mutex );
  return ret;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
