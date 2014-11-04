#ifndef _SPI_SIMULATOR_H_
#define _SPI_SIMULATOR_H_
#include "Gpio/gpio.h"


#define SCLK    179                     //����SCLK����Ӧ��GPIO�ӿڱ��  
#define MOSI    181                     //����SCLK����Ӧ��GPIO�ӿڱ��  
#define MISO    183                     //����MISO����Ӧ��GPIO�ӿڱ��  
#define SS      187                     //����SS����Ӧ��GPIO�ӿڱ��  
#define OUTP    GPIO_OUTPUT                       //��ʾGPIO�ӿڷ���Ϊ���  
#define INP GPIO_INPUT                           //��ʾGPIO�ӿڷ���Ϊ����  

#define set_gpio_direction(port, direction) gpio_init( port, direction)
#define	set_gpio_value(port, value) gpio_set(port, value)
#define get_gpio_value(port) gpio_read(port)

int spi_init(void);
void spi_write_byte(UINT8 b);
UINT8 spi_read_byte(UINT8 value);

#endif


