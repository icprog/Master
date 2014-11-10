/*
 * File: Closed_Loop_NCM_29Ah.h
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

#ifndef RTW_HEADER_Closed_Loop_NCM_29Ah_h_
#define RTW_HEADER_Closed_Loop_NCM_29Ah_h_
#ifndef Closed_Loop_NCM_29Ah_COMMON_INCLUDES_
# define Closed_Loop_NCM_29Ah_COMMON_INCLUDES_
#include <math.h>

/*#include "rtwtypes.h"*/
#endif                                 /* Closed_Loop_NCM_29Ah_COMMON_INCLUDES_ */

#include "Closed_Loop_NCM_29Ah_types.h"

#include"PE_Types.h"


/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((void) 0)
#endif

/* user code (top of header file) */
//#include "PE_Types.h"

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  TPE_Float X_i[3];                     /* '<S1>/X_PMerge' */
  TPE_Float P_i[9];                     /* '<S1>/X_PMerge' */
  TPE_Float XandP_2_DSTATE[9];          /* '<S1>/XandP' */
  TPE_Float XandP_1_DSTATE[3];          /* '<S1>/XandP' */
  TPE_Float I_DSTATE;                   /* '<S5>/I' */
  uint8_t F_Init;                      /* '<S1>/Event_Call' */
} D_Work_Closed_Loop_NCM_29Ah;

/* Block signals and states (auto storage) */
extern D_Work_Closed_Loop_NCM_29Ah Closed_Loop_NCM_29Ah_DWork;

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern Closed_Loop_CtlParamBus Closed_Loop_CtlParam;/* Variable: Closed_Loop_CtlParam
                                                     * Referenced by:
                                                     *   '<S4>/Constant1'
                                                     *   '<S6>/K_Dt'
                                                     *   '<S8>/K_Dt'
                                                     *   '<S8>/K_I'
                                                     *   '<S8>/K_U'
                                                     *   '<S11>/D1'
                                                     *   '<S11>/Qn'
                                                     *   '<S12>/D2'
                                                     */

/* Model entry point functions */
extern void Closed_Loop_NCM_29Ah_initialize(void);

/* Customized model step function */
extern void Closed_Loop_NCM_29Ah_custom(Closed_Loop_InParamBus
  *arg_Closed_Loop_In, Closed_Loop_OutParamBus *arg_Closed_Loop_Out);

/* Exported data declaration */
extern Closed_Loop_RCParamBus Closed_Loop_RCParam;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Closed_Loop_NCM_29Ah'
 * '<S1>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive'
 * '<S2>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Event_Call'
 * '<S3>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/OUT_Process'
 * '<S4>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/SOC_Init'
 * '<S5>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm'
 * '<S6>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/OUT_Process/OutData_Conversion'
 * '<S7>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/ClosedLoopEable'
 * '<S8>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/InData_Conversion'
 * '<S9>'   : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm'
 * '<S10>'  : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm/Model'
 * '<S11>'  : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm/Time_update'
 * '<S12>'  : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm/state_update'
 * '<S13>'  : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm/Model/DOCV'
 * '<S14>'  : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm/Model/OCV'
 * '<S15>'  : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm/Time_update/Transpose'
 * '<S16>'  : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm/state_update/Transpose'
 * '<S17>'  : 'Closed_Loop_NCM_29Ah/ClosedLoopActive/Soc_algorithm/Soc_algorithm/state_update/Transpose1'
 */
#endif                                 /* RTW_HEADER_Closed_Loop_NCM_29Ah_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
