#ifndef _GPIOFUNCS_H_
#define _GPIOFUNCS_H_
#include "Gpio/gpio.h"
#include "adc/adc.h"

#define T_AC_VALUE 1000

#define KEY_TO_ACC() (gpio_read(IO_STATUS_ACC) == 1)
#define KEY_TO_ON() (gpio_read(IO_STATUS_ON) == 1)
#define DC_CHARGER_INSERT() (gpio_read(IO_STATUS_CC2) == 1)
#define DC_CHARGER_DISCONNECT() (gpio_read(IO_STATUS_CC2) == 0)
#define AC_CHARGER_INSERT() (Adc_ReadResultMvByChannel(ADC_PORT_ACCIN) > T_AC_VALUE )
#define AC_CHARGER_DISCONNECT() ( (Adc_ReadResultMvByChannel(ADC_PORT_ACCIN) < T_AC_VALUE))

#endif

