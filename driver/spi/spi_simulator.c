#include "spi/spi_simulator.h"
#include "Gpio/gpio.h"
#include "timer/timer.h"

void spi_delay(void)
{
	UINT16 j = 0, i = 0;
	for( j=0x0f;j;j--)
		for(i=0xffff;i;i--)
			;
}


/* SPI端口初始化 */
int spi_init(void) {
	set_gpio_direction(SS, OUTP);
	set_gpio_direction(SCLK, OUTP);
	set_gpio_direction(MOSI, OUTP);
	set_gpio_direction(MISO, INP);
	set_gpio_value(SCLK, 0);                    //CPOL=0 
	set_gpio_value(MOSI, 0);
	return (0);
}
/*  
 从设备使能  
 enable：为1时，使能信号有效，SS低电平  
 为0时，使能信号无效，SS高电平  
 */
void ss_enable(int enable) {
    (void)enable;
//	if (enable)
//		set_gpio_value(SS, 0);                  //SS低电平，从设备使能有效  
//	else
//		set_gpio_value(SS, 1);                  //SS高电平，从设备使能无效  
	return;
}
/* SPI字节写 */
void spi_write_byte(UINT8 b) 
{
	int i;
	for (i = 7; i >= 0; i--) {
		set_gpio_value(SCLK, 0);
		set_gpio_value(MOSI, b & (1 << i));         //从高位7到低位0进行串行写入  
		spi_delay();                            //延时  
		set_gpio_value(SCLK, 1);                // CPHA=1，在时钟的第一个跳变沿采样  
		spi_delay();
	}
}
/* SPI字节读 */
UINT8 spi_read_byte(UINT8 value) {
	int i;
	UINT8 r = 0;
	for (i = 0; i < 8; i++) {
		set_gpio_value(SCLK, 0);
		spi_delay();                            //延时  
		set_gpio_value(SCLK, 1);                // CPHA=1，在时钟的第一个跳变沿采样  
		r = (r << 1) | get_gpio_value(MISO);         //从高位7到低位0进行串行读出  
		spi_delay();
	}
}
//
//int test_spi(void)
//{
//	Config_PLL();
//	INTC_InitINTCInterrupts();
//	initPIT_RTI();
//	gpio_init(LED_PORT, GPIO_OUTPUT);
//	enableIrq();
//	spi_init();
//	while(1)
//	{
//		spi_write_byte(0x55);
//		delay_ms(10);
//	}
//}
///*  
// SPI写操作  
// buf：写缓冲区  
// len：写入字节的长度  
// */
//void spi_write(const unsigned char* buf, int len) {
//	int i;
//	spi_init();                         //初始化GPIO接口  
//	ss_enable(1);                           //从设备使能有效，通信开始  
//	spi_delay();                                //延时  
////写入数据  
//	for (i = 0; i < len; i++)
//		spi_write_byte(buf[i]);
//	spi_delay();
//	ss_enable(0);                           //从设备使能无效，通信结束  
//}
///*  
// SPI读操作  
// buf：读缓冲区  
// len：读入字节的长度  
// */
//UINT8 spi_read(unsigned char* buf, int len) {
//	int i;
//	spi_init();                         //初始化GPIO接口  
//	ss_enable(1);                           //从设备使能有效，通信开始  
//	spi_delay();                                //延时  
////读入数据  
//	for (i = 0; i < len; i++)
//		buf[i] = spi_read_byte();
//	spi_delay();
//	ss_enable(0);                           //从设备使能无效，通信结束  
//}
//
//void spi_write_read(UINT8 *TxData, UINT8 TXlen, UINT8 *rx_data, UINT8 RXlen)
//{
//	spi_write(TxData, TXlen);
//	spi_read(rx_data, RXlen);
//}
