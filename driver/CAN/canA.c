/*
 * can driver
 * by mgc 2014-10-23
 * */

#include "MPC5634M.h"
#include "CAN/canA.h"

///************************************************************/
///*                    ��ʼ��CAN_A������                    */
///************************************************************/
//void initCAN_A(void)
//{
//	UINT8 i;
//	CAN_A .MCR.R = 0x5000003F;         //ʹ��FlexCAN����������64����Ϣ������
//	CAN_A .CR.R = 0x04DB0006;        //Configure for 8MHz OSC, 100KHz bit time
//	for (i = 0; i < 64; i++)
//	{
//		CAN_A .BUF[i].CS.B.CODE = 0; //������Ϣ������Ϊ�ǻ̬
//	}
//	CAN_A .BUF[0].CS.B.CODE = 8;       //���û���ͨ��0���ͽ�Ϊ�ǻ̬
//	SIU .PCR[83].R = 0x062C;         //����CAN_AΪ���ͽţ� open drain
//	SIU .PCR[84].R = 0x0500;         //����CAN_AΪ���ս�
//	CAN_A .MCR.R = 0x0000003F; /* Negate FlexCAN A halt state for 64 MB */
//}
//
///************************************************************/
///*                          CAN_A������Ϣ                   */
///************************************************************/
//int CANA_send(UINT8 *msg, UINT8 len)
//{
//	if (len > 8)
//		return -1;
////	const uint8_t TxData[] = {"Hello"};  /* Transmit string*/
//	CAN_A .BUF[0].CS.B.IDE = 0;          //ʹ�ñ�׼֡��ʽ
//	CAN_A .BUF[0].ID.B.STD_ID = 555;   //����ID��555
//	CAN_A .BUF[0].CS.B.RTR = 0;        //Ŀǰ����ͨ�����͵�������֡����Զ��֡
//	CAN_A .BUF[0].CS.B.LENGTH = len - 1;
//	//    CAN_A.BUF[0].CS.B.LENGTH = sizeof(TxData) -1 ;//���������ֽڵĳ���
//	memcpy(CAN_A .BUF[0].DATA, msg, len);
////	for(i=0;i<sizeof(TxData);i++)
////	{
////	    CAN_A.BUF[0].DATA.[i] = msg[i]; //��������
////
////	}
//	CAN_A .BUF[0].CS.B.SRR = 1;        //Tx frame (not req'd for standard frame)
//	CAN_A .BUF[0].CS.B.CODE = 0xC;  // Activate msg. buf. to transmit data frame
//	return len;
//}
//
///************************************************************/
///*                             CAN_A������Ϣ     ��ԃ           */
///* read data from cana to buff, return value:>0 len, <0 err  */
///************************************************************/
//
//UINT32 RxCODE;                  //������Ϣ�������Ĵ���
//UINT32 RxID;                    //������ϢID
//UINT8 RxDATA[8];               //������Ϣ���ݵ��ַ���
//UINT32 RxTIMESTAMP;             //���յ���Ϣ��ʱ��
//int CANA_receive(UINT8 *buff)
//{
//	UINT8 j;
//	UINT32 dummy;
//	int RxLENGTH;                //������Ϣ�������ֽ���
//	if (!buff)
//		return -1;
//
//	while (CAN_A .IFRL.B.BUF04I == 0)
//	{
//	};           // MPC555x: Wait for CAN C MB 4 flag
//	RxCODE = CAN_A .BUF[4].CS.B.CODE;  // Read CODE, ID, LENGTH, DATA, TIMESTAMP
//	RxID = CAN_A .BUF[4].ID.B.STD_ID;
//	RxLENGTH = CAN_A .BUF[4].CS.B.LENGTH;
//	for (j = 0; j < RxLENGTH; j++)
//	{
//		buff[j] = CAN_A .BUF[4].DATA.B[j];
//	}
//	RxTIMESTAMP = CAN_A .BUF[4].CS.B.TIMESTAMP;
//	dummy = CAN_A .TIMER.R;              // Read TIMER to unlock message buffers
//	CAN_A .IFRL.R = 0x00000010;                // MPC555x: Clear CAN C MB 4 flag
//	return RxLENGTH;
//}

unsigned char CAN1BufferNumber = 0;
unsigned char CAN0BufferNumber = 0;
unsigned long SAE_ID;
int j;
unsigned char dummy;

uint32_t RxCODE; /* Received message buffer code */
uint32_t RxID; /* Received message ID */
uint32_t RxLENGTH; /* Recieved message number of data bytes */
uint8_t RxDATA[8]; /* Received message data string*/
uint32_t RxTIMESTAMP; /* Received message time */

void ISR_CAN0RX(void);

void initSIU(void)
{
	SIU .PCR[188].R = 0x0200;
	SIU .PCR[189].R = 0x0200;
	SIU .PCR[190].R = 0x0200;
	SIU .PCR[191].R = 0x0200;
	SIU .GPDO[188].R = 1;
	SIU .GPDO[189].R = 1;
	SIU .GPDO[190].R = 1;
	SIU .GPDO[191].R = 1;
}
enum CAN_BANDRATE
{
	K100 = 1, K250 = 2, K500 = 3,
};

void CAN0Init(enum CAN_BANDRATE bandrate)
{
	uint8_t i;

	CAN_A .MCR.B.MDIS = 0;     //CAN0CTL1_CANE=1;		//CAN0ģ��ʹ��
	CAN_A .MCR.B.FRZ = 1;	  // CAN0CTL0_INITRQ=1;		//�����ʼ��ģʽ
	CAN_A .MCR.B.HALT = 1;
	CAN_A .MCR.B.MAXMB = 64;

	CAN_A.CR.R = 0;
	switch (bandrate)
	{
	case (K250)://���speed=2;baud=250KHz   //********************��ֲʱҪע��***********//
		CAN_A .CR.B.PRESDIV = 1;   //baud=250KHz
		CAN_A .CR.B.PSEG1 = 3;
		CAN_A .CR.B.PSEG2 = 3;
		CAN_A .CR.B.PROPSEG = 6;  //TSEG2��3 ,TSEG1=3 ,PROPSEG=6;
		break;
	case (K500): //���speed=3,baud=500KHz  //********************��ֲʱҪע��***********//
		CAN_A .CR.B.PRESDIV = 0;   //baud=500KHz
		CAN_A .CR.B.PSEG1 = 3;
		CAN_A .CR.B.PSEG2 = 3;
		CAN_A .CR.B.PROPSEG = 6;  //TSEG2��3 ,TSEG1=3 ,PROPSEG=6;
		break;
	default:	    //���speed�Ȳ���2Ҳ����3,��Ĭ��Ϊbaud=250;
		CAN_A .CR.B.PRESDIV = 1;   //baud=250KHz
		CAN_A .CR.B.PSEG1 = 3;
		CAN_A .CR.B.PSEG2 = 3;
		CAN_A .CR.B.PROPSEG = 6; //TSEG2��3 ,TSEG1=3 ,PROPSEG=6;
		break;
	}

//	CAN_A .CR.B.RJW = 0;
//	CAN_A .CR.B.BOFFMSK = 0;    // <URM>BOFF_MSK</URM> //
//	CAN_A .CR.B.ERRMSK = 0;     // <URM>ERR_MSK</URM> //
//	CAN_A .CR.B.CLKSRC = 0;     // <URM>CLK_SRC</URM> //
//	CAN_A .CR.B.LPB = 0;
//	CAN_A .CR.B.TWRNMSK = 0;    // <URM>TWRN_MSK</URM> //
//	CAN_A .CR.B.RWRNMSK = 0;    // <URM>RWRN_MSK</URM> //
//	CAN_A .CR.B.SMP = 0;
//	CAN_A .CR.B.BOFFREC = 0;    // <URM>BOFF_REC</URM> //
//	CAN_A .CR.B.TSYN = 0;
//	CAN_A .CR.B.LBUF = 0;
//	CAN_A .CR.B.LOM = 0;

	/**************************��������ΪCAN_A����***********************/
	/* MPC555x: Configure pad as CNTXA, open drain */
	SIU .PCR[83].R =0x062C;
	/* MPC555x: Configure pad as CNRXA */
	SIU .PCR[84].R = 0x0500;

////////////////////////////////////////////////////////////
	for (i = 0; i < 64; i++)
	{
		CAN_A .BUF[i].CS.B.CODE = 0; /* Inactivate all message buffers */
	}

	/*************************START CAN_A***********************/

	CAN_A .MCR.R = 0x0000003F;

	/*CAN_A.IMRL.B.BUF04M=0;     //CANA BUF[4] ID ����,ID DONT CARE
	 //CAN_A.IMRL.B.BUF03M=1;
	 //CAN_A.IMRL.B.BUF02M=1;
	 //CAN_A.IMRL.B.BUF01M=1;
	 //CAN_A.IMRL.B.BUF00M=1;

	 CAN_A.IFRL.B.BUF04I=1;     //CANA �ж�ʹ��MB[0:4]
	 CAN_A.IFRL.B.BUF03I=1;
	 CAN_A.IFRL.B.BUF02I=1;
	 CAN_A.IFRL.B.BUF01I=1;
	 CAN_A.IFRL.B.BUF00I=1;

	 CAN_A.IMRL.B.BUF04M=1;
	 CAN_A.IMRL.B.BUF03M=1;    //CANA �ж�ʹ��MB[0:3]
	 CAN_A.IMRL.B.BUF02M=1;
	 CAN_A.IMRL.B.BUF01M=1;
	 CAN_A.IMRL.B.BUF00M=1;
	 CAN_A.IFRL.B.BUF04I=1;
	 CAN_A.IFRL.B.BUF03I=1;
	 CAN_A.IFRL.B.BUF02I=1;
	 CAN_A.IFRL.B.BUF01I=1;
	 CAN_A.IFRL.B.BUF00I=1;

	 */

	for (i = 0; i < 4; i++)
		CAN_A .BUF[i].CS.B.CODE = 8; /* Message Buffer 0-3 set to Tx INACTIVE */
	CAN_A .BUF[4].CS.B.CODE = 4; /* Message Buffer 4 set to Rx INACTIVE */
	CAN_A .RXGMASK.R = 0x00000000; /* Global acceptance mask */
	//CAN_A.RXGMASK.R = 0x1FFFFFFF;   /* Global acceptance mask */
	//�ȴ�CAN_A׼����
	INTC_InstallINTCInterruptHandler(ISR_CAN0RX, 159, 10);
	CAN_A .IMRL.B.BUF04M = 1;        // Enable MB4 interrupt;
	while (CAN_A .MCR.B.NOTRDY == 1)
	{
	}
	INTC .CPR.B.PRI = 0; /* MPC555x: Lower INTC's current priority */

	return;
}

void PDU_ID2CAN0_ID(unsigned long ID_LW)            //CAN0 ��ID�Ĵ���
{
	CAN_A .BUF[CAN0BufferNumber].ID.R = ID_LW;
}

void CAN0TX(unsigned long ID, unsigned char * pTxData, unsigned char nbytes)
{
	uint8_t i;
	if (CAN0BufferNumber == 4)
		CAN0BufferNumber = 0;     //��MB0-MB3��������

	CAN_A .BUF[CAN0BufferNumber].CS.B.IDE = 1; /* Use standard ID length */
	/*******************************���PDU_ID2CAN0_ID()�����Ǹ�ʲô�õ�******************************/
	PDU_ID2CAN0_ID(ID); //����ID  	CAN_A.BUF[CAN0BufferNumber].ID.B.EXT_ID =ID;
	/* Transmit ID is 555 */
	/*******************************���PDU_ID2CAN0_ID()�����Ǹ�ʲô�õ�******************************/
	CAN_A .BUF[CAN0BufferNumber].CS.B.RTR =	0;
	/* Data frame, not remote Tx request frame */
	CAN_A .BUF[CAN0BufferNumber].CS.B.LENGTH =nbytes;
	/* # bytes to transmit w/o null */
	for (i = 0; i < nbytes; i++)
	{
		CAN_A .BUF[CAN0BufferNumber].DATA.B[i] = pTxData[i]; /* Data to be transmitted */
	}
	CAN_A .BUF[CAN0BufferNumber].CS.B.SRR = 1; /* Tx frame (not req'd for standard frame)*/
	CAN_A .BUF[CAN0BufferNumber].CS.B.CODE = 0xC; /* Activate msg. buf. to transmit data frame */
	CAN0BufferNumber++;
}

void test_canA(void)
{

	int k = 0;
	UINT8 Tx[8] =
	{ 1, 2, 3, 4, 5, 6, 7, 8 };
	CAN0Ini(K100);
	initSIU();
	INTC_InitINTCInterrupts();
	CAN0TX(0x1fffffff, Tx, 8);
	while (1)
	{
		k++;
	}
}

/****************************************************************************************/

void ISR_CAN0RX(void)      //CAN0 �Ľ���
{

//  uint8_t j;
	uint32_t dummy;

//  while (CAN_A.IFRL.B.BUF04I == 0) {};    /* MPC555x: Wait for CAN C MB 4 flag */
//  RxCODE   = CAN_A.BUF[4].CS.B.CODE;      /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */

	RxID = CAN_A .BUF[4].ID.R;
	SAE_ID = CAN_A .BUF[4].ID.R;
//  RxLENGTH = CAN_A.BUF[4].CS.B.LENGTH;
//  for (j=0; j<RxLENGTH; j++) {
//    RxDATA[j] = CAN_A.BUF[4].DATA.B[j];
//  }
//  RxTIMESTAMP = CAN_A.BUF[4].CS.B.TIMESTAMP;
	dummy = CAN_A .TIMER.R; /* Read TIMER to unlock message buffers */
	SIU .GPDO[188].R = ~SIU .GPDO[188].R;
	CAN_A .IFRL.R = 0x00000010; /* MPC555x: Clear CAN C MB 4 flag */
}

