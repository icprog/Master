/*
 * File: Closed_Loop_NCM_29Ah_change_private.h
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

#ifndef RTW_HEADER_Closed_Loop_NCM_29Ah_change_private_h_
#define RTW_HEADER_Closed_Loop_NCM_29Ah_change_private_h_
#include "rtwtypes.h"
#ifndef __RTWTYPES_H__
#error This file requires rtwtypes.h to be included
#else
#ifdef TMWTYPES_PREVIOUSLY_INCLUDED
#error This file requires rtwtypes.h to be included before tmwtypes.h
#else

/* Check for inclusion of an incorrect version of rtwtypes.h */
#ifndef RTWTYPES_ID_C08S16I32L32N32F1
#error This code was generated with a different "rtwtypes.h" than the file included
#endif                                 /* RTWTYPES_ID_C08S16I32L32N32F1 */
#endif                                 /* TMWTYPES_PREVIOUSLY_INCLUDED */
#endif                                 /* __RTWTYPES_H__ */

extern const real32_T rtCP_pooled_GEYRxL5LwuPZ[9];
extern const real32_T rtCP_pooled_93err6Slc9vl[9];
extern const real32_T rtCP_pooled_JJ9J2JQQzByy[9];

#define rtCP_INI_P_InitialConditio     rtCP_pooled_GEYRxL5LwuPZ  /* Expression: single(eye(3)*0.0001)
                                                                  * Referenced by: '<Root>/INI_P' (Parameter: InitialCondition)
                                                                  */
#define rtCP_D1_Value                  rtCP_pooled_93err6Slc9vl  /* Expression:  single(eye(3)*1e-10)
                                                                  * Referenced by: '<S4>/D1' (Parameter: Value)
                                                                  */
#define rtCP_Constant1_Value_e         rtCP_pooled_JJ9J2JQQzByy  /* Computed Parameter: Constant1_Value_e
                                                                  * Referenced by: '<S5>/Constant1' (Parameter: Value)
                                                                  */
#endif                                 /* RTW_HEADER_Closed_Loop_NCM_29Ah_change_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
