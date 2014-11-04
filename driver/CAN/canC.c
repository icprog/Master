#include "MPC5634M.h"
#include "canC.h"

static UINT32 RxCODE;                  //������Ϣ�������Ĵ���
static UINT32 RxID;                    //������ϢID
static UINT32 RxTIMESTAMP;             //���յ���Ϣ��ʱ��


/************************************************************/
/*                    ��ʼ��CAN_C������                     */
/************************************************************/
void initCAN_C(void)
{
	UINT8 i;
	CAN_C.MCR.R=0x5000003F;         //ʹ��FlexCAN����������64����Ϣ������
    CAN_C.CR.R=0x04DB0006;          //Configure for 8MHz OSC, 100KHz bit time
    for(i=0;i<32;i++)
    {
    	CAN_C.BUF[i].CS.B.CODE=0;   //������Ϣ������Ϊ�ǻ̬

    }
    CAN_C.BUF[4].CS.B.IDE=0;        //����ͨ��4��Ѱ��һ����׼��ID
    CAN_C.BUF[4].ID.B.STD_ID=555;   //����ͨ��4��Ѱ��ID = 555
    CAN_C.BUF[4].CS.B.CODE=4;       //���û���ͨ��4���ս�Ϊ��
    CAN_C.RXGMASK.R=0x1FFFFFFF;     //Global acceptance mask
    SIU.PCR[87].R=0x0E2C;           //����CAN_CΪ���ͽţ� open drain
    SIU.PCR[88].R=0x0D00;           //����CAN_CΪ���ս�
    CAN_C.MCR.R=0x0000003F;         //Negate FlexCAN C halt state for  64 MB
}


/************************************************************/
/*                          CAN_A������Ϣ                   */
/************************************************************/
int CANC_send(UINT8 *msg, UINT8 len)
{
	if( len > 8 )
		return -1;
//	const uint8_t TxData[] = {"Hello"};  /* Transmit string*/
	CAN_C.BUF[0].CS.B.IDE=0;          //ʹ�ñ�׼֡��ʽ
	CAN_C.BUF[0].ID.B.STD_ID = 555;   //����ID��555
	CAN_C.BUF[0].CS.B.RTR = 0;        //Ŀǰ����ͨ�����͵�������֡����Զ��֡
	CAN_C.BUF[0].CS.B.LENGTH = len-1;
    //    CAN_A.BUF[0].CS.B.LENGTH = sizeof(TxData) -1 ;//���������ֽڵĳ���
    memcpy(CAN_C.BUF[0].DATA, msg, len);
//	for(i=0;i<sizeof(TxData);i++)
//	{
//	    CAN_A.BUF[0].DATA.[i] = msg[i]; //��������
//
//	}
    CAN_C.BUF[0].CS.B.SRR=1;             //Tx frame (not req'd for standard frame)
    CAN_C.BUF[0].CS.B.CODE =0xC;         // Activate msg. buf. to transmit data frame
    return len;
}


/************************************************************/
/*                             CAN_A������Ϣ     ��ԃ           */
/* read data from cana to buff, return value:>0 len, <0 err  */
/************************************************************/
int CANC_receive(UINT8 *buff)
{
	UINT8 j;
	UINT32 dummy;
	int RxLENGTH;                //������Ϣ�������ֽ���
	if( !buff )
		return -1;

	while(CAN_C.IFRL.B.BUF04I == 0) {};           // MPC555x: Wait for CAN C MB 4 flag
	RxCODE  =CAN_C.BUF[4].CS.B.CODE;              // Read CODE, ID, LENGTH, DATA, TIMESTAMP
	RxID    =CAN_C.BUF[4].ID.B.STD_ID;
	RxLENGTH=CAN_C.BUF[4].CS.B.LENGTH;
	for(j=0;j<RxLENGTH;j++)
	{
		buff[j] = CAN_C.BUF[4].DATA.B[j];
	}
	RxTIMESTAMP=CAN_C.BUF[4].CS.B.TIMESTAMP;
	dummy=CAN_C.TIMER.R;                         // Read TIMER to unlock message buffers
	CAN_C.IFRL.R=0x00000010;                     // MPC555x: Clear CAN C MB 4 flag
	return RxLENGTH;
}

