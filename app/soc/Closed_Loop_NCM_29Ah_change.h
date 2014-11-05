/*
 * File: Closed_Loop_NCM_29Ah_change.h
 *
 * Code generated for Simulink model 'Closed_Loop_NCM_29Ah_change'.
 *
 * Model version                  : 1.643
 * Simulink Coder version         : 8.3 (R2012b) 20-Jul-2012
 * TLC version                    : 8.3 (Jul 21 2012)
 * C/C++ source code generated on : Wed Nov 05 11:19:47 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->MPC5xx
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. MISRA-C:2004 guidelines
 *    3. RAM efficiency
 * Validation result: Passed (11), Warnings (2), Error (0)
 */

#ifndef RTW_HEADER_Closed_Loop_NCM_29Ah_change_h_
#define RTW_HEADER_Closed_Loop_NCM_29Ah_change_h_
#ifndef Closed_Loop_NCM_29Ah_change_COMMON_INCLUDES_
# define Closed_Loop_NCM_29Ah_change_COMMON_INCLUDES_
#include <math.h>
#include "rtwtypes.h"
#endif                                 /* Closed_Loop_NCM_29Ah_change_COMMON_INCLUDES_ */

#include "Closed_Loop_NCM_29Ah_change_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((void) 0)
#endif

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real32_T INI_X_DSTATE[3];            /* '<Root>/INI_X' (DWork 1)  */
  real32_T INI_P_DSTATE[9];            /* '<Root>/INI_P' (DWork 1)  */
  real32_T I_DSTATE;                   /* '<Root>/I' (DWork 1)  */
} D_Work_Closed_Loop_NCM_29Ah_cha;

/* Block signals and states (auto storage) */
extern D_Work_Closed_Loop_NCM_29Ah_cha Closed_Loop_NCM_29Ah_chan_DWork;

/* Model entry point functions */
extern void Closed_Loop_NCM_29Ah_initialize(void);

/* Customized model step function */
extern void Closed_Loop_NCM_29Ah_custom(real32_T *soc, real32_T IN_U, real32_T
  IN_DT, real32_T IN_I, int8_T IN_I_Direct);

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
 * '<Root>' : 'Closed_Loop_NCM_29Ah_change'
 * '<S1>'   : 'Closed_Loop_NCM_29Ah_change/InData_Conversion'
 * '<S2>'   : 'Closed_Loop_NCM_29Ah_change/Soc_algorithm'
 * '<S3>'   : 'Closed_Loop_NCM_29Ah_change/Soc_algorithm/Model'
 * '<S4>'   : 'Closed_Loop_NCM_29Ah_change/Soc_algorithm/Time_update'
 * '<S5>'   : 'Closed_Loop_NCM_29Ah_change/Soc_algorithm/state_update'
 * '<S6>'   : 'Closed_Loop_NCM_29Ah_change/Soc_algorithm/Model/DOCV'
 * '<S7>'   : 'Closed_Loop_NCM_29Ah_change/Soc_algorithm/Model/OCV'
 */
#endif                                 /* RTW_HEADER_Closed_Loop_NCM_29Ah_change_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
