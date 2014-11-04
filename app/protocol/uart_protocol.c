/*********************************************************
 *
 * ��оԴ΢�������޹�˾(UCT)
 *
 * File Description
 *
 *   $Revision: 2422 $
 *   $Author: huhai $
 *   $Date: 2014-07-30 16:04:39 +0800 (周三, 30 七月 2014) $
 *
 *********************************************************/

#include "protocol/uart_protocol.h"
#include "stdio.h"
#include "mgclib/mgcfifo.h"
#include <string.h>

static UINT8 g_protocolinit = 0;
volatile UCHAR g_rx_frame_size = 0;
UCHAR g_rx_cmd_buff[MAX_CMD_PARAM_SIZE];
char fifo_buff[MGCFIFO_BUFF_CALCULATE(FIFO_ELEMENTS_SIZE, FIFO_SIZE)];
UCHAR *presult = NULL;
UCHAR *prxbuff = g_rx_cmd_buff;
struct mgcfifo * p_uart_rx_fifo;

int
protocol_init(void)
{
  int ret = 0;
  memset(fifo_buff, 0x55, sizeof(fifo_buff));
  memset(g_rx_cmd_buff, 0x55, sizeof(g_rx_cmd_buff));
  ret = mgcfifo_alloc(&p_uart_rx_fifo, fifo_buff, sizeof(fifo_buff),
      FIFO_ELEMENTS_SIZE);
  if (ret <= 0)
    return -1;
  g_protocolinit = 1;
  return ret;
}

static struct uart_cmd_pkg g_msg =
  { 0, 0, g_rx_cmd_buff };
/*
static int
protocol_frame_size(void)
{
  return g_rx_frame_size;
}
*/
static void
protocol_frame_clear(void)
{
  g_rx_frame_size = 0;
}

/*********************************************************
 * Э�鷢�ͺ���
 *********************************************************/
void
protocol_tx(UCHAR cmdwords, const UCHAR *pcmd, UINT16 cmd_len)
{
  UCHAR len[2];
  BYTESORDER_H2S_16(cmd_len, len);
  //PROTOCOL_PUTS(head, sizeof(head));
  (void) PROTOCOL_PUTS ( "tx:" );
  //PROTOCOL_PUTS(head);
  //(void)PROTOCOL_PUTC ( head[0] );
  //(void)PROTOCOL_PUTC ( head[1] );
  (void) PROTOCOL_PUTARRAY(head,sizeof(head));
  (void) PROTOCOL_PUTARRAY ( &cmdwords, 1 );
  //(void)PROTOCOL_PUTHEX ( cmdwords );
  (void) PROTOCOL_PUTARRAY ( len, sizeof ( len ) );

  if (pcmd)
    (void) PROTOCOL_PUTARRAY ( pcmd, cmd_len );

  (void) PROTOCOL_PUTARRAY(tail, sizeof(tail));
  //(void)PROTOCOL_PUTC ( tail[0] );
  //(void)PROTOCOL_PUTC ( tail[1] );

  (void) PROTOCOL_PUTS ( "    \r\n" );
}

/*********************************************************
 * Э����գ��ж��е���? *
 *********************************************************/
void
uart_isr_getch(UCHAR ch)
{
  if (g_protocolinit != 1)
    return;
  (void) mgcfifo_in(p_uart_rx_fifo, (char *) &ch);
}

int
uart_isr_fifo_size(void)
{
  return mgcfifo_nvalid(p_uart_rx_fifo);
}

void
protocol_rx(UCHAR ch)
{
  enum _searia_rx_frm_state
  {
    state_head = 1, state_cmd, state_len, state_cmdparam, state_tail
  };
  //UINT16  i = 0;
  //static UCHAR err_count = 0;
  static enum _searia_rx_frm_state rx_state = state_head;
  static UINT16 count = 0;

  switch (rx_state)
    {
  /*ǰ����*/
  case state_head:
    {
      if (count >= sizeof(head))
        {
          count = 0;
        }

      if (ch == head[count])
        {
          if (count == sizeof(head) - 1)
            {
              count = 0;
              rx_state = state_cmd;
              break;
            }

          count++;
        }
      else
        {
          count = 0;
        }

      break;
    }
    /*������*/
  case state_cmd:
    {
      g_msg.cmd = ch;
      rx_state = state_len;
      break;
    }
    /*����*/
  case state_len:
    {

      if (count >= 2)
        {
          count = 0;
        }

      /*�߰�λ*/
      if (count == 0)
        {
          g_msg.len = ch;
          count++;
        }
      /*�Ͱ�λ*/
      else
        {
          g_msg.len = g_msg.len * 256 + ch;
          //* ( ( UCHAR * ) &g_msg.len + 1 ) = ( ch );
          count = 0;

          if (g_msg.len == 0)
            rx_state = state_tail;

          else if (g_msg.len >= MAX_CMD_PARAM_SIZE)
            rx_state = state_head;
          else
            rx_state = state_cmdparam;
        }

      break;
    }
   case state_cmdparam:
    {
      if (count >= sizeof(g_rx_cmd_buff))
        {
          count = 0;
        }

      if (count < g_msg.len - 1) //count ��0��ʼ ���һ���ֽ���else����
        {
          g_msg.param[count] = ch;
          count++;
        }
      else
        {
          g_msg.param[count] = ch;
          rx_state = state_tail;
          count = 0;
        }

      break;
    }
  case state_tail:
    {
      if (count >= sizeof(tail))
        {
          count = 0;
        }
      if (ch == tail[count])
        {
          count++;

          if (count == sizeof(tail))
            {
              count = 0;

              //    (void)mprintf( "rx:{ < %02x %02x %02x ", g_msg.cmd & 0x0ff, g_msg.len / 0xff & 0x0ff, g_msg.len & 0x0ff );
              //    (void)printf_array_hex ( (char *)g_msg.param, g_msg.len );
              //    (void)mprintf ( "> } \r\n" );
              g_rx_frame_size = 1;
              rx_state = state_head;
              break;
            }
        }
      else
        {
          count = 0;
          rx_state = state_head;
        }

      break;
    }
  default:
    {
      count = 0;
      rx_state = state_head;
      //(void)DEBUG_UART_PRINTF( "err:serial rec unkown state!\n" );
    }
    break;
    }
}

/*********************************************************
 * ���ڴ�������
 * ����0 ��ʾδ����
 * ����<0 ��ʾ����
 * ����>0��ʾ�ɹ�
 *********************************************************/
//UCHAR tbuff[MAX_CMD_PARAM_SIZE];
INT16
process_rs232_cmd(void) //��ѭ������
{
  int len = 0;
  UCHAR ch = 0;
  struct uart_cmd_pkg *tmsg = &g_msg;

  while (uart_isr_fifo_size() > 0)
    {
      (void) mgcfifo_out(p_uart_rx_fifo, (char *) &ch);
      protocol_rx(ch);
    }
  // /*
  if (g_rx_frame_size > 0) //protocol_frame_size() > 0 )
    {
      //(void)mgcfifo_out( pfifo, (char *)tmsg);

      len = ProcessCmdByWords((UCHAR *) tmsg, &presult);
      //len = ProcessCmdByWords ( ( UCHAR * ) &g_msg, &presult );

      // g_rx_frame_size --;

      if (len >= 0)
        {

          protocol_tx(tmsg->cmd, presult, (UINT16)len);
          //   protocol_tx ( g_msg.cmd, presult, len );
        }

      protocol_frame_clear();

      return len;
    }
  //    */
  return 0;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
