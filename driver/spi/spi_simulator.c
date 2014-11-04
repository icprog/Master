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


/* SPI�˿ڳ�ʼ�� */
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
 ���豸ʹ��  
 enable��Ϊ1ʱ��ʹ���ź���Ч��SS�͵�ƽ  
 Ϊ0ʱ��ʹ���ź���Ч��SS�ߵ�ƽ  
 */
void ss_enable(int enable) {
    (void)enable;
//	if (enable)
//		set_gpio_value(SS, 0);                  //SS�͵�ƽ�����豸ʹ����Ч  
//	else
//		set_gpio_value(SS, 1);                  //SS�ߵ�ƽ�����豸ʹ����Ч  
	return;
}
/* SPI�ֽ�д */
void spi_write_byte(UINT8 b) 
{
	int i;
	for (i = 7; i >= 0; i--) {
		set_gpio_value(SCLK, 0);
		set_gpio_value(MOSI, b & (1 << i));         //�Ӹ�λ7����λ0���д���д��  
		spi_delay();                            //��ʱ  
		set_gpio_value(SCLK, 1);                // CPHA=1����ʱ�ӵĵ�һ�������ز���  
		spi_delay();
	}
}
/* SPI�ֽڶ� */
UINT8 spi_read_byte(UINT8 value) {
	int i;
	UINT8 r = 0;
	for (i = 0; i < 8; i++) {
		set_gpio_value(SCLK, 0);
		spi_delay();                            //��ʱ  
		set_gpio_value(SCLK, 1);                // CPHA=1����ʱ�ӵĵ�һ�������ز���  
		r = (r << 1) | get_gpio_value(MISO);         //�Ӹ�λ7����λ0���д��ж���  
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
// SPIд����  
// buf��д������  
// len��д���ֽڵĳ���  
// */
//void spi_write(const unsigned char* buf, int len) {
//	int i;
//	spi_init();                         //��ʼ��GPIO�ӿ�  
//	ss_enable(1);                           //���豸ʹ����Ч��ͨ�ſ�ʼ  
//	spi_delay();                                //��ʱ  
////д������  
//	for (i = 0; i < len; i++)
//		spi_write_byte(buf[i]);
//	spi_delay();
//	ss_enable(0);                           //���豸ʹ����Ч��ͨ�Ž���  
//}
///*  
// SPI������  
// buf����������  
// len�������ֽڵĳ���  
// */
//UINT8 spi_read(unsigned char* buf, int len) {
//	int i;
//	spi_init();                         //��ʼ��GPIO�ӿ�  
//	ss_enable(1);                           //���豸ʹ����Ч��ͨ�ſ�ʼ  
//	spi_delay();                                //��ʱ  
////��������  
//	for (i = 0; i < len; i++)
//		buf[i] = spi_read_byte();
//	spi_delay();
//	ss_enable(0);                           //���豸ʹ����Ч��ͨ�Ž���  
//}
//
//void spi_write_read(UINT8 *TxData, UINT8 TXlen, UINT8 *rx_data, UINT8 RXlen)
//{
//	spi_write(TxData, TXlen);
//	spi_read(rx_data, RXlen);
//}