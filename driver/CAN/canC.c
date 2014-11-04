#include "MPC5634M.h"
#include "canC.h"

static UINT32 RxCODE;                  //接收消息缓冲区的代码
static UINT32 RxID;                    //接收消息ID
static UINT32 RxTIMESTAMP;             //接收的消息的时间


/************************************************************/
/*                    初始化CAN_C：接收                     */
/************************************************************/
void initCAN_C(void)
{
	UINT8 i;
	CAN_C.MCR.R=0x5000003F;         //使能FlexCAN，启用所有64个消息缓冲区
    CAN_C.CR.R=0x04DB0006;          //Configure for 8MHz OSC, 100KHz bit time
    for(i=0;i<32;i++)
    {
    	CAN_C.BUF[i].CS.B.CODE=0;   //所有消息缓冲区为非活动态

    }
    CAN_C.BUF[4].CS.B.IDE=0;        //缓冲通道4将寻找一个标准的ID
    CAN_C.BUF[4].ID.B.STD_ID=555;   //缓冲通道4将寻找ID = 555
    CAN_C.BUF[4].CS.B.CODE=4;       //设置缓冲通道4接收脚为空
    CAN_C.RXGMASK.R=0x1FFFFFFF;     //Global acceptance mask
    SIU.PCR[87].R=0x0E2C;           //配置CAN_C为发送脚， open drain
    SIU.PCR[88].R=0x0D00;           //配置CAN_C为接收脚
    CAN_C.MCR.R=0x0000003F;         //Negate FlexCAN C halt state for  64 MB
}


/************************************************************/
/*                          CAN_A发送信息                   */
/************************************************************/
int CANC_send(UINT8 *msg, UINT8 len)
{
	if( len > 8 )
		return -1;
//	const uint8_t TxData[] = {"Hello"};  /* Transmit string*/
	CAN_C.BUF[0].CS.B.IDE=0;          //使用标准帧格式
	CAN_C.BUF[0].ID.B.STD_ID = 555;   //传递ID是555
	CAN_C.BUF[0].CS.B.RTR = 0;        //目前缓冲通道发送的是数据帧而非远程帧
	CAN_C.BUF[0].CS.B.LENGTH = len-1;
    //    CAN_A.BUF[0].CS.B.LENGTH = sizeof(TxData) -1 ;//发送数据字节的长度
    memcpy(CAN_C.BUF[0].DATA, msg, len);
//	for(i=0;i<sizeof(TxData);i++)
//	{
//	    CAN_A.BUF[0].DATA.[i] = msg[i]; //发送数据
//
//	}
    CAN_C.BUF[0].CS.B.SRR=1;             //Tx frame (not req'd for standard frame)
    CAN_C.BUF[0].CS.B.CODE =0xC;         // Activate msg. buf. to transmit data frame
    return len;
}


/************************************************************/
/*                             CAN_A接收信息     查詢           */
/* read data from cana to buff, return value:>0 len, <0 err  */
/************************************************************/
int CANC_receive(UINT8 *buff)
{
	UINT8 j;
	UINT32 dummy;
	int RxLENGTH;                //接收消息的数据字节数
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

