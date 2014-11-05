/*********************************************************
 *
 * 闂佽法鍠愰弸濠氬箯閻ゎ垰袩婵犙勫姇娴滄洟鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕梺璺ㄥ枑閺嬪骞忛悜鑺ユ櫢闁活偒鍋嗗▍銏ゅ箯瀹勬澘绶�UCT)
 *
 * File Description
 *
 *   $Revision: 3404 $
 *   $Author: huhai $
 *   $Date: 2014-09-01 17:02:15 +0800 (闂佸憡绋忛崝瀣博閿燂拷? 01 婵炴垶姊圭换鍐耿閿燂拷?2014) $
 *
 *********************************************************/

#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include "public/types.h"

//#include "init_class.h"

#define assert(v)  while(!(v))

#define BMS_VERSION "$Revision: 3404 $"
#define HARDWARE_VER 0

#define TOTAL_SOC_NUM 1
#define TOTAL_SOH_NUM 1
#define TOTAL_CHARGE_STATUS_NUM 1
#define TOTAL_BALANCE_NUM TOTAL_VOLTAGE_NUM

#define PL536_NUM 1
#define N_CELL_PER_PL536 6
#define N_TEMP_PER_PL536 2
#define VOLTAGE_NUM PL536_NUM*N_CELL_PER_PL536
#define TEMPERATURE_NUM PL536_NUM*N_TEMP_PER_PL536
//
//#ifndef _USE_BM03
//#define _USE_BM03
//#endif
#define USE_BM06

#ifdef _USE_BM01
#define TOTAL_FANS_NUM 2
#define TOTAL_HEATPANNALS_NUM 1
#define TOTAL_CURRENTS 1
#define USE_BM01_OLD_PROTOCAL 1
#endif

#ifdef _USE_BM03
#define TOTAL_FANS_NUM 0
#define TOTAL_HEATPANNALS_NUM 0
//#define TOTAL_CURRENTS 4
//#define USE_BM01_OLD_PROTOCAL 1

#define VALUE_TH_OV 39000 //3.9V
#define VALUE_TH_OV_RECOVER 38000 //3.8V
#define VALUE_TH_UV 20000
#define VALUE_TH_UV_RECOVER 23000

#define VALUE_TH_TIME_FILTER_OV 200  //maybe 1.5s
#define VALUE_TH_TIME_FILTER_UV 100  //maybe 0.98

#define VALUE_TH_OC 25 // 25A
#define VALUE_TH_TIME_FILTER_OC 150 // 1s
#define VALUE_TH_SHORT_CIRCUIT 250 // 250A
#define VALUE_TH_TIME_FILTER_SC 10 // 500 us

#define VALUE_TH_OT 80
#define VALUE_TH_OT_RECOVER 60

#define VALUE_TH_TIME_GO_TO_SLEEP 5 //60  // the time that between the time pin status changed and system go to sleep   (s)
#define VALUE_TH_VOLTAGE_SLEEP 33400 //3.34v

#define VALUE_K_ADC2C_DISCHARGE  (0.02)
#define VALUE_K_ADC2C_DISCHARGE_OC  (0.0125)
#define VALUE_K_ADC2C_CHARGE  (0.039)
#define VALUE_K_ADC2C_CHARGE_OC  (0.0125)

#endif

#ifdef USE_BM06
#define TOTAL_FANS_NUM 0
#define TOTAL_HEATPANNALS_NUM 0
#define TOTAL_TEMPERATURE_SENSORS_NUM 20
//#define TOTAL_CURRENTS 4
//#define USE_BM01_OLD_PROTOCAL 1

#define VALUE_TH_OV 39000 //3.9V
#define VALUE_TH_OV_RECOVER 38000 //3.8V
#define VALUE_TH_UV 20000
#define VALUE_TH_UV_RECOVER 23000

#define VALUE_TH_TIME_FILTER_OV 200  //maybe 1.5s
#define VALUE_TH_TIME_FILTER_UV 100  //maybe 0.98

#define VALUE_TH_OC 25 // 25A
#define VALUE_TH_TIME_FILTER_OC 150 // 1s
#define VALUE_TH_SHORT_CIRCUIT 250 // 250A
#define VALUE_TH_TIME_FILTER_SC 10 // 500 us

#define VALUE_TH_OT 80
#define VALUE_TH_OT_RECOVER 60

#define VALUE_TH_TIME_GO_TO_SLEEP 5 //60  // the time that between the time pin status changed and system go to sleep   (s)
#define VALUE_TH_VOLTAGE_SLEEP 33400 //3.34v

#define VALUE_K_ADC2C_DISCHARGE  (0.02)
#define VALUE_K_ADC2C_DISCHARGE_OC  (0.0125)
#define VALUE_K_ADC2C_CHARGE  (0.039)
#define VALUE_K_ADC2C_CHARGE_OC  (0.0125)

//#define N_CELL_PER_LTC6804        (12)
//#define N_TEMPERATURE_PER_LTC6804 (5)
//#define N_LTC6804_PER_SLAVE       (2)
//#define N_CELLS_PER_SLAVE		  (N_CELL_PER_LTC6804*N_LTC6804_PER_SLAVE)
//#define N_TEMPERATURE_PER_SLAVE   (N_TEMPERATURE_PER_LTC6804*N_LTC6804_PER_SLAVE)
//#define NUMBER_OF_SLAVES          (20)    // max support slaves
//#define TOTAL_TEMP_NUM            (NUMBER_OF_SLAVES*N_TEMPERATURE_PER_LTC6804*N_LTC6804_PER_SLAVE)
//#define TOTAL_VOLTAGES            (NUMBER_OF_SLAVES*N_LTC6804_PER_SLAVE*N_CELL_PER_LTC6804) //NUMBER_OF_CELLS_OF_THIS_BTTERIES//              LTC6804_IC_NUM * LTC6804_MAX_VOL
#endif
//
//
//struct slave_info
//{
//	UINT8 id;
//	UINT16 voltage[N_CELLS_PER_SLAVE];
//	INT16 temperatures[N_TEMPERATURE_PER_SLAVE];
//	INT8 soc;
//	UINT8 balance_state[N_CELLS_PER_SLAVE/8 + N_CELLS_PER_SLAVE%8];
//	UINT8 io_state;
//	UINT8 warnning_state;
//	UINT8 error_state;
//};
//
//struct slave_list
//{
//	struct slave_info m_slave;
//	struct slave_info *next;
//};
//
//struct pack_info
//{
//	INT8 soc;
//	INT8 soh;
//	struct slave_list *slave_list;
//};
//

//
//enum SYSTEM_MODE
//{
//	SYSTEM_MODE_WORK,
//	 
//};

enum CANINFORPARAM
{

  COMMBYTE = 256, //闂侇偅宀搁弫鎾绘嚇濮樺崬顣奸柡鍫嫹
  BARTYPE = 3, //闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾绘晸閿燂拷? RATECURRENT = 60, //闂佽法鍠撻悺鏃傦拷瑙勫哺閺佹捇寮妶鍡楊伓闂佽法鍠愰弸濠氬箯閿燂拷?  RATEVOLTE = 84, //闂佽法鍠撻悺鏃傦拷瑙勫哺閺佹捇寮妶鍡楊伓闁告﹫鎷�
  SCHGV = 2400, //闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柦妯侯槸婵晠鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕梺璺ㄥ枑閺嬪骞忛悜鑺ユ櫢闁哄倶鍊栫�褰掓煥閻斿憡鐏柟椋庡厴閺佹捇寮妶鍡楊伓闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊宕㈤敓锟�  CHARGI = -4000, //闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕梺璺ㄥ枑閺嬪骞忛悜鑺ユ櫢闁哄倶鍊栭敓锟�  AWEIGHT = 10000, //闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲箣椤忓懎鏁鹃柟椋庡厴閺佹捇寮妶鍡楊伓闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊鏌ㄩ悤鍌涘  ACHATEV = 7500, //闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕梺璺ㄥ枑閺嬪骞忛悿顖氼棡闂佽法鍠愰弸濠氬箯閿燂拷?  ALTEMP = 75, //闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕梻鍥ㄧ叀閺佹捇鏁撻敓锟� //ELCSTATE = ; //闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲箣椤忓洤绁圭痪鏉款槹鐎氬綊鎮╅懜纰夋嫹閿燂拷?  //ALVOLT; //闂佽法鍠愰弸濠氬箯閻戣姤鏅搁柡鍌樺�鐎氬綊鏌ㄩ悢鍛婄伄闁归鍏橀弫鎾诲棘閵堝棗顏堕梺璺ㄥ枑濠㈠啰鍠婃径瀣伓闁告﹫鎷�
  VREQUEST = 7500 //闂佽法鍠愰弸濠氬箯瀹勬澘绔鹃梺璺ㄥ枑閺嬪骞忛悜鑺ユ櫢闁哄倶鍊栭敓锟�  IREQUEST = -4000,
};


enum flowdown_state
{
  STATE_BLOW = 1, STATE_DOWN = 3, STATE_IDLE = 0, STATE_SETUP = 2
};

#define     BIT0        0x01
#define     BIT1        0x02
#define     BIT2        0x04
#define     BIT3        0x08
#define     BIT4        0x10
#define     BIT5        0x20
#define     BIT6        0x40
#define     BIT7        0x80

/*bit set--add by mgc 20140730*/

#define MGC_SET_BIT(b,n)   ((b) = ((b)&(~(1<<(n))))|(1<<(n))) 
#define MGC_RESET_BIT(b, n)  (b) = (b)&(~(1<<(n)))

void
char_to_string(unsigned char data, unsigned char *output);
void
int16_to_string(unsigned int data, unsigned char *output);
void
int16_to_string_zero(UINT8 precision, UINT16 data, UINT8 *output);
void
delayus(UINT16 n);
//UINT32
//ver_software_get(void);
UINT8
ver_hardware_get(void);
int
set_simulator_mode(UINT8 mode);
int
get_simulator_mode(void);

void MCU_Reset(void);
enum EXCEPTION_SIMULATOR_MASK
{
 MASK_BIT_SIMULATOR_MODE_VOL = 0x01,
 MASK_BIT_SIMULATOR_MODE_TEMP = 0x02,
 MASK_BIT_SIMULATOR_MODE_CURRENT = 0x04
};

#ifdef _USE_BM03
#define EnableInterrupts  __enable_fault_irq()
#define DisableInterrupts __disable_fault_irq()
#endif

#endif

extern void
mprintf(const char *format, ...);
extern void
print2bank(const char *fmt, ...);
