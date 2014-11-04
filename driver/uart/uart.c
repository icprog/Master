#include "MPC5634M.h"
#include "gpio.h"
#include "pll/pll.h"
#include "string.h"
#include "IntcInterrupts.h"   
#include "protocol/uart_protocol.h"


UINT8 data_receive;
#define FMCLK 8*1000*1000

UINT16
GetSBRByBaundrate(UINT16 baundrate)
{
  UINT16 sbr = FMCLK / (16 * baundrate);
  return sbr;
}
void SCIA_ISAR(void);
/************************************************************/
/*                           ��ʼ��SCI_A                    */
/************************************************************/
void
initESCI_A(void)
{
  ESCI_A .CR2.R = 0x2000;      // SCI_A ģ��ʹ��
//  ESCI_A .CR1.R = 0x01A1000C;
  ESCI_A.CR1.R=0x01A1002C;  //SCI_A �ж�ʹ�� ������9600
  /* ������9600 , 8λ����, ����żУ��λ,���ͽ���ʹ��
   (3~15:110100001=417,buad=64M/(417*16))*/
//    ESCI_A.CR1.R = 0x34000C;// (GetSBRByBaundrate(9600)<<16) | 0x000C;
  SIU .PCR[89].R = 0x0400;      // ��������Ϊ��������: TxDA
  SIU .PCR[90].R = 0x0400;      // ��������Ϊ��������: RxDA
    INTC .PSR[146].R = 1;                //ʵʱ�ж����ȼ�
    INTC_InstallINTCInterruptHandler(SCIA_ISAR, 146, 1);  //�����жϺ�
}

/************************************************************/
/*                     SCI_A ����һ�ֽ�                     */
/************************************************************/
void 
SCIA_putChar(UINT8 ch)
{
  ESCI_A .DR.B.D = ch;             // д���跢�͵�����
  while (ESCI_A .SR.B.TDRE == 0)
    {
    }    // �ȴ��������
  ESCI_A .SR.R = 0x80000000;         // ���������ɱ�־λ

}


/*************************************************************/
/*                        SCI_A����һ�ֽ�                    */
/*************************************************************/
UINT8
SCIA_getChar()
{
  while (ESCI_A .SR.B.RDRF == 0)
    {
    } /* Wait for receive data reg full = 1 */
  ESCI_A .SR.R = 0X20000000; /* Clear RDRF flag */
  return ESCI_A .DR.B.D; /* Read byte of Data*/
}

int
scia_send(const char *data, UINT8 datalen)
{
  int ret = 0;
  while (datalen-- && data)
    SCIA_putChar(*data++);
  ret = datalen;
  return ret;
}

int scia_init(void)
{

  initESCI_A();
  return 0;
}


void SCIA_ISAR(void)
{
  UINT8 rec_data = 0;
  if(ESCI_A .SR.B.RDRF != 0)
   {
      ESCI_A .SR.R = 0X20000000; /* Clear RDRF flag */
      rec_data = ESCI_A .DR.B.D; /* Read byte of Data*/
      scia_send((char *)&rec_data, 1);
      protocol_rx(rec_data);
   } /* Wait for receive data reg full = 1 */
}



/************************************************************/
/*                       ������                             */
/************************************************************/
void
test_SCIA(void)
{
  static const char s[] = "hello world!\r\n";
  UINT8 flag = 0;

  Config_PLL();
  initESCI_A();
  gpio_init(LED1_PORT, GPIO_OUTPUT);
  /* Loop forever */
  for (;;)
    {
      scia_send(s, sizeof(s));
      gpio_set(LED1_PORT, flag);
      flag = 1 - flag;
      
//      data_receive=SCIA_getChar();
//      if(data_receive=='O')
//      {
//      	SCIA_putChar('Y');
//        gpio_set(LED_PORT, 0);
//      }
//      if(data_receive=='C')
//      {
//      	SCIA_putChar('V');
//        gpio_set(LED_PORT, 1);
//      }
    }
}