#ifndef _SPI_SIMULATOR_H_
#define _SPI_SIMULATOR_H_
#include "Gpio/gpio.h"


#define SCLK    179                     //定义SCLK所对应的GPIO接口编号  
#define MOSI    181                     //定义SCLK所对应的GPIO接口编号  
#define MISO    183                     //定义MISO所对应的GPIO接口编号  
#define SS      187                     //定义SS所对应的GPIO接口编号  
#define OUTP    GPIO_OUTPUT                       //表示GPIO接口方向为输出  
#define INP GPIO_INPUT                           //表示GPIO接口方向为输入  

#define set_gpio_direction(port, direction) gpio_init( port, direction)
#define	set_gpio_value(port, value) gpio_set(port, value)
#define get_gpio_value(port) gpio_read(port)

int spi_init(void);
void spi_write_byte(UINT8 b);
UINT8 spi_read_byte(UINT8 value);

#endif


