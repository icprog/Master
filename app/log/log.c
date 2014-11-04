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


#include "public.h"
#include "timer.h"
#include "log.h"

enum UCT_ERR_CODE log_write(UINT8 mod_id, UINT8 status, UINT8 event_type, UINT8 len, UINT8 *note) 
{
  enum UCT_ERR_CODE rv = UCT_SUCCESS;
  UINT8 count = 0;
  UINT8 log_data[33] = {0};
  UINT8 temp_8 = 0;
  UINT32 time = 0;
  UINT16 temp_16 = 0;
  if( len+7 > 32 )
  {
    mprintf("err log.c len too long\r\n");
    return rv;
  }
  (void)memset( log_data, 0, sizeof(log_data) );
  time = get_time();
  log_data[count++] = (time >> 24) & 0xFF;
  log_data[count++] = (time >> 16) & 0xFF;
  log_data[count++] = (time >> 8) & 0xFF;
  log_data[count++] = time & 0xFF;
  log_data[count++] = mod_id;
  log_data[count++] = status;
  log_data[count++] = event_type;
  for (temp_8 = 0; temp_8 < len; temp_8++)
  {
    log_data[count++] = note[temp_8]; 
  }
  
  DisableInterrupts;
  // Call SPI driver to write data to SD card
  WriteLog(log_data);
 // mprintf("LOG:");
 // printf_array_hex(log_data, len+7);
 // mprintf("\r\n");
  EnableInterrupts;  
	
	return rv;
}