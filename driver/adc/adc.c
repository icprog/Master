/************************************************************/
/*                     初始化ADC转换                        */
/************************************************************/
#include "MPC5634M.h"
#include "Gpio/gpio.h"
#include "adc/adc.h"

static void initADC0(void)
{
    EQADC.CFPR[0].R=0x80801001;            /* Send CFIFO 0 a ADC0 configuration command 
                                            enable ADC0 & sets prescaler= divide by 2 */    
    EQADC.CFCR[0].R=0x0410;                // Trigger CFIFO 0 using Single Scan SW
 	while(EQADC.FISR[0].B.EOQF!=1) {}      // Wait for End Of Queue flag
    EQADC.FISR[0].B.EOQF=1;        	       // Clear End Of Queue flag
}

void Adc_InitADC0(void)
{	
	UINT32 command = 0x80801001;
//	assert( channel<39);s
	//command |= (channel<<16);
    EQADC.CFPR[0].R=command;            /* Send CFIFO 0 a ADC0 configuration command 
                                            enable ADC0 & sets prescaler= divide by 2 */    
    EQADC.CFCR[0].R=0x0410;                // Trigger CFIFO 0 using Single Scan SW
 	while(EQADC.FISR[0].B.EOQF!=1) {}      // Wait for End Of Queue flag
    EQADC.FISR[0].B.EOQF=1;        	       // Clear End Of Queue flag
}

/************************************************************/
/*                       ADC转换命令        转换 0~39               */
/************************************************************/
static void Adc_ConvertByChannel(UINT8 channel)
{
	UINT32 command = 0x80000000;
	assert( channel<39);
	command |= (channel<<8);
    EQADC.CFPR[0].R = command;  /* Conversion command: convert channel 0 */
                                   /* with ADC0, set EOQ, and send result to RFIFO 0 */                           
    EQADC.CFCR[0].R = 0x0410;     /* Trigger CFIFO 0 using Single Scan SW mode */  	
}

static UINT32 Adc_ReadResult(void)
{
	UINT32 Result = 0;
//	UINT32 ResultInmv = 0;
	while (EQADC.FISR[0].B.RFDF != 1){}      /* Wait for RFIFO 0's Drain Flag to set */
	  Result = EQADC.RFPR[0].R;                /* ADC result */ 
//	  ResultInmv = (uint32_t)((5000*Result)/0x3FFC);  /* ADC result in millivolts */        
	  EQADC.FISR[0].B.RFDF = 1;                /* Clear RFIFO 0's Drain Flag */
	  EQADC.FISR[0].B.EOQF = 1;                /* Clear CFIFO's End of Queue flag */
	  return Result;
}

UINT32 Adc_AdcToMv( UINT32 adc_result )
{
	UINT32 result_mv = 0;
	result_mv = (uint32_t)((5000*adc_result)/0x3FFC);
	return result_mv;
}


UINT32 Adc_ReadResultByChannel( UINT8 channel)
{
	Adc_ConvertByChannel(channel);
	return Adc_ReadResult();
}

UINT32 Adc_ReadResultMvByChannel(UINT8 channel)
{
	UINT32 result_mv = 0;
	Adc_ConvertByChannel(channel);
	result_mv = Adc_ReadResultByChannel(channel);
	result_mv = (uint32_t)((5000*result_mv)/0x3FFC);
	return result_mv;
}
/************************************************************/
/*                       ADC转换命令                        */
/************************************************************/
static void SendConvCmd(void)
{
    
    EQADC.CFPR[0].R = 0x80000000;  /* Conversion command: convert channel 0 */
                                   /* with ADC0, set EOQ, and send result to RFIFO 0 */                           
    EQADC.CFCR[0].R = 0x0410;     /* Trigger CFIFO 0 using Single Scan SW mode */  	
}


/************************************************************/
/*                       ADC读取结果                       */
/************************************************************/
//uint32_t ReadResult(void) 
//{  
//  uint32_t Result = 0;
//  uint32_t ResultInmv = 0;
//  SendConvCmd();                /* Send one conversion command */
//  while (EQADC.FISR[0].B.RFDF != 1){}      /* Wait for RFIFO 0's Drain Flag to set */
//  Result = EQADC.RFPR[0].R;                /* ADC result */ 
//  ResultInmv = (uint32_t)((5000*Result)/0x3FFC);  /* ADC result in millivolts */        
//  EQADC.FISR[0].B.RFDF = 1;                /* Clear RFIFO 0's Drain Flag */
//  EQADC.FISR[0].B.EOQF = 1;                /* Clear CFIFO's End of Queue flag */
//  return Result;
//}

int adc_currentUpdate(void)
{
	return 0;
}

int adc_update(void)
{
	return 0; //TODO
}

int test_Adc_init(void)
{
	Adc_InitADC0();
}

int test_Adc(void)
{
	static UINT8 i = 0;
	static UINT32 adcresult[39];
	static UINT32 adcresult_mv[39];
//	while(1)
	{
		adcresult[i] = Adc_ReadResultByChannel(i);
		adcresult_mv[i] = Adc_AdcToMv(adcresult[i]);
		if( ++i == 20 ) 
			i = 0;
	}
	return 0;
}

