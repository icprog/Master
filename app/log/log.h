/*********************************************************
*
* 晟芯源微电子有限公司(UCT)
*
* File Description
*
*   $Revision: 2342 $
*   $Author: huhai $
*   $Date: 2014-07-29 09:49:28 +0800 (鍛ㄤ簩, 29 涓冩湀 2014) $
*
*********************************************************/

#include "public/public.h"

#ifndef _LOG_H_
#define _LOG_H_

enum LOG_ID_DEF
{
  LOG_SYSTEM_ID = 0,
  LOG_DRV_SPI_ID = 1,
  LOG_DRV_CAN_ID = 2,
  LOG_DRV_UART_ID = 3,
  LOG_DRV_GPIO_ID =4 ,
  LOG_DRV_SCREEN_ID = 5,
  LOG_MOD_TEMP_ID = 6,
  LOG_MOD_VOLTAGE_ID = 7
};

enum LOG_STATUS_DEF {
  LOG_STATUS_OK = 0,
  LOG_STATUS_WARNING  = 1, 
  LOG_STATUS_CRITICAL = 2
};


#endif

enum UCT_ERR_CODE log_write(UINT8 mod_id, UINT8 status, UINT8 event_type, UINT8 len, UINT8 *note);
