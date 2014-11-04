/*
 * can driver
 * by mgc 2014-10-23
 * */

#include "MPC5634M.h"
#include "CAN/canA.h"

///************************************************************/
///*                    初始化CAN_A：发送                    */
///************************************************************/
//void initCAN_A(void)
//{
//	UINT8 i;
//	CAN_A .MCR.R = 0x5000003F;         //使能FlexCAN，启用所有64个消息缓冲区
//	CAN_A .CR.R = 0x04DB0006;        //Configure for 8MHz OSC, 100KHz bit time
//	for (i = 0; i < 64; i++)
//	{
//		CAN_A .BUF[i].CS.B.CODE = 0; //所有消息缓冲区为非活动态
//	}
//	CAN_A .BUF[0].CS.B.CODE = 8;       //设置缓冲通道0发送脚为非活动态
//	SIU .PCR[83].R = 0x062C;         //配置CAN_A为发送脚， open drain
//	SIU .PCR[84].R = 0x0500;         //配置CAN_A为接收脚
//	CAN_A .MCR.R = 0x0000003F; /* Negate FlexCAN A halt state for 64 MB */
//}
//
///************************************************************/
///*                          CAN_A发送信息                   */
///************************************************************/
//int CANA_send(UINT8 *msg, UINT8 len)
//{
//	if (len > 8)
//		return -1;
////	const uint8_t TxData[] = {"Hello"};  /* Transmit string*/
//	CAN_A .BUF[0].CS.B.IDE = 0;          //使用标准帧格式
//	CAN_A .BUF[0].ID.B.STD_ID = 555;   //传递ID是555
//	CAN_A .BUF[0].CS.B.RTR = 0;        //目前缓冲通道发送的是数据帧而非远程帧
//	CAN_A .BUF[0].CS.B.LENGTH = len - 1;
//	//    CAN_A.BUF[0].CS.B.LENGTH = sizeof(TxData) -1 ;//发送数据字节的长度
//	memcpy(CAN_A .BUF[0].DATA, msg, len);
////	for(i=0;i<sizeof(TxData);i++)
////	{
////	    CAN_A.BUF[0].DATA.[i] = msg[i]; //发送数据
////
////	}
//	CAN_A .BUF[0].CS.B.SRR = 1;        //Tx frame (not req'd for standard frame)
//	CAN_A .BUF[0].CS.B.CODE = 0xC;  // Activate msg. buf. to transmit data frame
//	return len;
//}
//
///************************************************************/
///*                             CAN_A接收信息     查詢           */
///* read data from cana to buff, return value:>0 len, <0 err  */
///************************************************************/
//
//UINT32 RxCODE;                  //接收消息缓冲区的代码
//UINT32 RxID;                    //接收消息ID
//UINT8 RxDATA[8];               //接收消息数据的字符串
//UINT32 RxTIMESTAMP;             //接收的消息的时间
//int CANA_receive(UINT8 *buff)
//{
//	UINT8 j;
//	UINT32 dummy;
//	int RxLENGTH;                //接收消息的数据字节数
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

	CAN_A .MCR.B.MDIS = 0;     //CAN0CTL1_CANE=1;		//CAN0模块使能
	CAN_A .MCR.B.FRZ = 1;	  // CAN0CTL0_INITRQ=1;		//进入初始化模式
	CAN_A .MCR.B.HALT = 1;
	CAN_A .MCR.B.MAXMB = 64;

	CAN_A.CR.R = 0;
	switch (bandrate)
	{
	case (K250)://如果speed=2;baud=250KHz   //********************移植时要注意***********//
		CAN_A .CR.B.PRESDIV = 1;   //baud=250KHz
		CAN_A .CR.B.PSEG1 = 3;
		CAN_A .CR.B.PSEG2 = 3;
		CAN_A .CR.B.PROPSEG = 6;  //TSEG2＝3 ,TSEG1=3 ,PROPSEG=6;
		break;
	case (K500): //如果speed=3,baud=500KHz  //********************移植时要注意***********//
		CAN_A .CR.B.PRESDIV = 0;   //baud=500KHz
		CAN_A .CR.B.PSEG1 = 3;
		CAN_A .CR.B.PSEG2 = 3;
		CAN_A .CR.B.PROPSEG = 6;  //TSEG2＝3 ,TSEG1=3 ,PROPSEG=6;
		break;
	default:	    //如果speed既不是2也不是3,就默认为baud=250;
		CAN_A .CR.B.PRESDIV = 1;   //baud=250KHz
		CAN_A .CR.B.PSEG1 = 3;
		CAN_A .CR.B.PSEG2 = 3;
		CAN_A .CR.B.PROPSEG = 6; //TSEG2＝3 ,TSEG1=3 ,PROPSEG=6;
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

	/**************************设置引脚为CAN_A功能***********************/
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

	/*CAN_A.IMRL.B.BUF04M=0;     //CANA BUF[4] ID 屏蔽,ID DONT CARE
	 //CAN_A.IMRL.B.BUF03M=1;
	 //CAN_A.IMRL.B.BUF02M=1;
	 //CAN_A.IMRL.B.BUF01M=1;
	 //CAN_A.IMRL.B.BUF00M=1;

	 CAN_A.IFRL.B.BUF04I=1;     //CANA 中断使能MB[0:4]
	 CAN_A.IFRL.B.BUF03I=1;
	 CAN_A.IFRL.B.BUF02I=1;
	 CAN_A.IFRL.B.BUF01I=1;
	 CAN_A.IFRL.B.BUF00I=1;

	 CAN_A.IMRL.B.BUF04M=1;
	 CAN_A.IMRL.B.BUF03M=1;    //CANA 中断使能MB[0:3]
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
	//等待CAN_A准备好
	INTC_InstallINTCInterruptHandler(ISR_CAN0RX, 159, 10);
	CAN_A .IMRL.B.BUF04M = 1;        // Enable MB4 interrupt;
	while (CAN_A .MCR.B.NOTRDY == 1)
	{
	}
	INTC .CPR.B.PRI = 0; /* MPC555x: Lower INTC's current priority */

	return;
}

void PDU_ID2CAN0_ID(unsigned long ID_LW)            //CAN0 对ID的处理
{
	CAN_A .BUF[CAN0BufferNumber].ID.R = ID_LW;
}

void CAN0TX(unsigned long ID, unsigned char * pTxData, unsigned char nbytes)
{
	uint8_t i;
	if (CAN0BufferNumber == 4)
		CAN0BufferNumber = 0;     //用MB0-MB3发送数据

	CAN_A .BUF[CAN0BufferNumber].CS.B.IDE = 1; /* Use standard ID length */
	/*******************************这个PDU_ID2CAN0_ID()到底是干什么用的******************************/
	PDU_ID2CAN0_ID(ID); //传送ID  	CAN_A.BUF[CAN0BufferNumber].ID.B.EXT_ID =ID;
	/* Transmit ID is 555 */
	/*******************************这个PDU_ID2CAN0_ID()到底是干什么用的******************************/
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

void ISR_CAN0RX(void)      //CAN0 的接收
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

