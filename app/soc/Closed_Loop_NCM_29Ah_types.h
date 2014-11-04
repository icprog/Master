/*
 * File: Closed_Loop_NCM_29Ah_types.h
 *
 * Code generated for Simulink model 'Closed_Loop_NCM_29Ah'.
 *
 * Model version                  : 1.630
 * Simulink Coder version         : 8.3 (R2012b) 20-Jul-2012
 * TLC version                    : 8.3 (Jul 21 2012)
 * C/C++ source code generated on : Thu Oct 23 15:25:58 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->MPC5xx
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. MISRA-C:2004 guidelines
 *    3. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Closed_Loop_NCM_29Ah_types_h_
#define RTW_HEADER_Closed_Loop_NCM_29Ah_types_h_

/*#include "rtwtypes.h"*/

#include"PE_Types.h"

#ifndef _DEFINED_TYPEDEF_FOR_Closed_Loop_InParamBus_
#define _DEFINED_TYPEDEF_FOR_Closed_Loop_InParamBus_

typedef struct {
  TPE_Float IN_I;
  uint8_t IN_I_Direct;
  TPE_Float IN_U;
  TPE_Float IN_Dt;
  TPE_Float IN_Soc_Init;
  TPE_Float IN_P_Init[9];
} Closed_Loop_InParamBus;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_Closed_Loop_OutParamBus_
#define _DEFINED_TYPEDEF_FOR_Closed_Loop_OutParamBus_

typedef struct {
  TPE_Float OUT_Soc;
  TPE_Float OUT_X2;
  TPE_Float OUT_X3;
  TPE_Float OUT_P[9];
} Closed_Loop_OutParamBus;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_Closed_Loop_CtlParamBus_
#define _DEFINED_TYPEDEF_FOR_Closed_Loop_CtlParamBus_

typedef struct {
  TPE_Float D1[9];
  TPE_Float D2;
  TPE_Float Qn;
  int16_t K_U;
  int8_t K_I;
  int16_t K_Dt;
  TPE_Float K_Soc;
  int8_t K_T;
} Closed_Loop_CtlParamBus;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_Closed_Loop_RCParamBus_
#define _DEFINED_TYPEDEF_FOR_Closed_Loop_RCParamBus_

typedef struct {
  TPE_Float R0;
  TPE_Float R1;
  TPE_Float C1;
} Closed_Loop_RCParamBus;

#endif
#endif                                 /* RTW_HEADER_Closed_Loop_NCM_29Ah_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
