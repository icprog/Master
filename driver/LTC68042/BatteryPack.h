#ifndef _BATTERYPACK_H_
#define _BATTERYPACK_H_
#include "Gpio/gpio.h"
#define MAX_PACKS 8
#define N_CELL_PER_PACK 12
#define MAX_CELLS 90

#define MAX_CELL_PER_CHIP 12
#define MAX_TEMP_PRE_CHIP 6
#define MAX_CHIPS 8

struct bq_chip
{
	UINT16 cells[MAX_CELL_PER_CHIP];
	int temperature[MAX_TEMP_PRE_CHIP];
	UINT8 balance[MAX_CELL_PER_CHIP];
};

struct bt
{
	struct bq_chip m_packs[MAX_CHIPS];
};

int bt_init(void);
void bt_BatteryMonitor(void);
UINT16 bt_ReadCellVoltageMvByIndex( UINT8 PackIndex, UINT8 CellIndex );
int bt_ReadTempByPackIndex( UINT8 Packindex, UINT8 tempIndex );
UINT16 bt_ReadVoltageMvByIndex(UINT8 index);
int bt_ReadTempByIndex(UINT8 index);
int bt_setBalanceByIndex( UINT8 index);
int bt_CopyVoltagesMv(UINT32 *dst_buff, UINT8 len);


#endif
