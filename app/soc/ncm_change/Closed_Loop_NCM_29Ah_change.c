/*
 * File: Closed_Loop_NCM_29Ah_change.c
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

#include "Closed_Loop_NCM_29Ah_change.h"
#include "Closed_Loop_NCM_29Ah_change_private.h"

/* Block signals and states (auto storage) */
D_Work_Closed_Loop_NCM_29Ah_cha Closed_Loop_NCM_29Ah_chan_DWork;

/* Model step function */
void Closed_Loop_NCM_29Ah_custom(real32_T *soc, real32_T IN_U, real32_T IN_DT,
  real32_T IN_I, int8_T IN_I_Direct)
{
  real32_T rtb_MathFunction;
  real32_T rtb_Add1_c;
  real32_T rtb_Divide1;
  real32_T rtb_U_y;
  real32_T rtb_X1_time[3];
  real32_T rtb_P4X2;
  real32_T rtb_p5x3;
  real32_T rtb_p6x4;
  real32_T rtb_p7x5;
  real32_T rtb_p8x6;
  real32_T rtb_P9X7;
  real32_T rtb_P10X8;
  real32_T rtb_Divide_g;
  real32_T rtb_I_float_A;
  real32_T rtb_p2x_c;
  real32_T rtb_Product2;
  real32_T rtb_Product3;
  real32_T rtb_Product4;
  real32_T rtb_Product5;
  real32_T rtb_Product6;
  real32_T rtb_Product7;
  real32_T rtb_Product8;
  real32_T rtb_MatrixA[9];
  real32_T rtb_K[3];
  real32_T rtb_P1_time[9];
  real32_T rtb_Add1_c_0[3];
  int32_T i;
  real32_T tmp[9];
  int32_T i_0;
  real32_T tmp_0[9];
  int32_T i_1;
  int32_T i_2;
  int32_T i_3;
  int32_T i_4;
  real32_T rtb_P1_time_0[3];
  int32_T i_5;
  int32_T i_6;
  real32_T tmp_1[9];
  int32_T i_7;
  real32_T tmp_2[9];
  int32_T i_8;
  int32_T i_9;
  int32_T i_a;
  real32_T rtb_Reshape_idx;
  real32_T rtb_ADD_X_idx;
  real32_T y;

  /* Product: '<S1>/Dt_float_s' incorporates:
   *  Constant: '<S1>/K_Dt'
   *  Inport: '<Root>/IN_DT'
   */
  rtb_MathFunction = IN_DT / 1000.0F;

  /* Product: '<S4>/dt_q1' incorporates:
   *  Constant: '<S4>/Constant3600'
   *  Constant: '<S4>/Qn'
   *  Product: '<S4>/dt_q'
   */
  rtb_Add1_c = (rtb_MathFunction / 31.45F) / 3600.0F;

  /* Math: '<S4>/Math Function' incorporates:
   *  Product: '<S4>/Divide'
   *  UnaryMinus: '<S4>/Unary Minus1'
   *
   * About '<S4>/Math Function':
   *  Operator: exp
   */
  rtb_MathFunction = (real32_T)exp(-(rtb_MathFunction / 59.954174F));

  /* Product: '<S4>/Divide1' incorporates:
   *  Constant: '<S4>/Constant6'
   *  Constant: '<S4>/R1'
   *  Sum: '<S4>/Add'
   */
  rtb_Divide1 = (1.0F - rtb_MathFunction) * 0.00310176F;

  /* Reshape: '<S4>/Reshape' incorporates:
   *  Constant: '<S4>/Constant'
   *  Constant: '<S4>/Constant1'
   *  Constant: '<S4>/Constant2'
   *  Constant: '<S4>/Constant3'
   *  Constant: '<S4>/Constant4'
   *  UnaryMinus: '<S4>/Unary Minus'
   *  UnaryMinus: '<S4>/Unary Minus2'
   */
  rtb_MatrixA[0] = 1.0F;
  rtb_MatrixA[1] = 0.0F;
  rtb_MatrixA[2] = -rtb_Add1_c;
  rtb_MatrixA[3] = 0.0F;
  rtb_MatrixA[4] = rtb_MathFunction;
  rtb_MatrixA[5] = -rtb_Divide1;
  rtb_MatrixA[6] = 0.0F;
  rtb_MatrixA[7] = 0.0F;
  rtb_MatrixA[8] = 1.0F;

  /* SignalConversion: '<S4>/TmpSignal ConversionAtMatrix Multiply1Inport2' incorporates:
   *  Saturate: '<S4>/SOC_Saturation'
   *  UnitDelay: '<Root>/INI_X'
   */
  if (Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[0] >= 0.95F) {
    y = 0.95F;
  } else if (Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[0] <= 0.0498F) {
    y = 0.0498F;
  } else {
    y = Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[0];
  }

  /* Reshape: '<S4>/Reshape2' incorporates:
   *  Constant: '<S4>/Constant5'
   */
  rtb_Add1_c_0[0] = rtb_Add1_c;
  rtb_Add1_c_0[1] = rtb_Divide1;
  rtb_Add1_c_0[2] = 0.0F;

  /* Sum: '<S4>/X1_time' incorporates:
   *  Product: '<S4>/Matrix Multiply'
   *  Product: '<S4>/Matrix Multiply1'
   *  SignalConversion: '<S4>/TmpSignal ConversionAtMatrix Multiply1Inport2'
   *  UnitDelay: '<Root>/I'
   *  UnitDelay: '<Root>/INI_X'
   */
  for (i = 0; i < 3; i = i + 1) {
    rtb_X1_time[i] = (((0.0F + (rtb_MatrixA[i + 0] * y)) + (rtb_MatrixA[i + 3] *
      Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[1])) + (rtb_MatrixA[i + 6] *
      Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[2])) + (rtb_Add1_c_0[i] *
      Closed_Loop_NCM_29Ah_chan_DWork.I_DSTATE);
  }

  /* End of Sum: '<S4>/X1_time' */

  /* UnitDelay: '<Root>/INI_P' incorporates:
   *  Product: '<S4>/Matrix Multiply2'
   */
  for (i_0 = 0; i_0 < 9; i_0 = i_0 + 1) {
    tmp[i_0] = Closed_Loop_NCM_29Ah_chan_DWork.INI_P_DSTATE[i_0];
  }

  /* End of UnitDelay: '<Root>/INI_P' */

  /* Product: '<S4>/Matrix Multiply2' incorporates:
   *  Math: '<S4>/Math Function1'
   */
  for (i_1 = 0; i_1 < 3; i_1 = i_1 + 1) {
    for (i_2 = 0; i_2 < 3; i_2 = i_2 + 1) {
      tmp_0[i_1 + (3 * i_2)] = 0.0F;
      tmp_0[i_1 + (3 * i_2)] = tmp_0[i_1 + (3 * i_2)] + (tmp[i_1 + 0] *
        rtb_MatrixA[i_2 + 0]);
      tmp_0[i_1 + (3 * i_2)] = tmp_0[i_1 + (3 * i_2)] + (tmp[i_1 + 3] *
        rtb_MatrixA[i_2 + 3]);
      tmp_0[i_1 + (3 * i_2)] = tmp_0[i_1 + (3 * i_2)] + (tmp[i_1 + 6] *
        rtb_MatrixA[i_2 + 6]);
    }
  }

  /* Sum: '<S4>/P1_time' incorporates:
   *  Constant: '<S4>/D1'
   *  Product: '<S4>/Matrix Multiply2'
   */
  for (i_3 = 0; i_3 < 3; i_3 = i_3 + 1) {
    for (i_4 = 0; i_4 < 3; i_4 = i_4 + 1) {
      rtb_P1_time[i_3 + (3 * i_4)] = (((0.0F + (rtb_MatrixA[i_3 + 0] * tmp_0[0 +
        (3 * i_4)])) + (rtb_MatrixA[i_3 + 3] * tmp_0[1 + (3 * i_4)])) +
        (rtb_MatrixA[i_3 + 6] * tmp_0[2 + (3 * i_4)])) + rtCP_D1_Value[i_3 + (3 *
        i_4)];
    }
  }

  /* End of Sum: '<S4>/P1_time' */

  /* Math: '<S6>/Math Function1' incorporates:
   *  Constant: '<S6>/Constant2'
   */
  if ((rtb_X1_time[0] < 0.0F) && (2.0F > ((real32_T)floor(2.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 2.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 2.0F);
  }

  /* End of Math: '<S6>/Math Function1' */

  /* Product: '<S6>/Product2' incorporates:
   *  Constant: '<S6>/Constant'
   *  Constant: '<S6>/p4'
   */
  rtb_P4X2 = (3.0F * rtb_U_y) * 222.166489F;

  /* Math: '<S6>/Math Function2' incorporates:
   *  Constant: '<S6>/Constant3'
   */
  if ((rtb_X1_time[0] < 0.0F) && (3.0F > ((real32_T)floor(3.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 3.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 3.0F);
  }

  /* End of Math: '<S6>/Math Function2' */

  /* Product: '<S6>/Product3' incorporates:
   *  Constant: '<S6>/P5'
   *  Constant: '<S6>/constant'
   */
  rtb_p5x3 = (4.0F * rtb_U_y) * -1248.69641F;

  /* Math: '<S6>/Math Function3' incorporates:
   *  Constant: '<S6>/Constant4'
   */
  if ((rtb_X1_time[0] < 0.0F) && (4.0F > ((real32_T)floor(4.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 4.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 4.0F);
  }

  /* End of Math: '<S6>/Math Function3' */

  /* Product: '<S6>/Product4' incorporates:
   *  Constant: '<S6>/P6 '
   *  Constant: '<S6>/constant1'
   */
  rtb_p6x4 = (5.0F * rtb_U_y) * 4134.66113F;

  /* Math: '<S6>/Math Function4' incorporates:
   *  Constant: '<S6>/Constant5'
   */
  if ((rtb_X1_time[0] < 0.0F) && (5.0F > ((real32_T)floor(5.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 5.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 5.0F);
  }

  /* End of Math: '<S6>/Math Function4' */

  /* Product: '<S6>/Product5' incorporates:
   *  Constant: '<S6>/P7'
   *  Constant: '<S6>/constant2'
   */
  rtb_p7x5 = (6.0F * rtb_U_y) * -8745.2627F;

  /* Math: '<S6>/Math Function5' incorporates:
   *  Constant: '<S6>/Constant6'
   */
  if ((rtb_X1_time[0] < 0.0F) && (6.0F > ((real32_T)floor(6.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 6.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 6.0F);
  }

  /* End of Math: '<S6>/Math Function5' */

  /* Product: '<S6>/Product6' incorporates:
   *  Constant: '<S6>/P8 '
   *  Constant: '<S6>/constant3'
   */
  rtb_p8x6 = (7.0F * rtb_U_y) * 12078.8047F;

  /* Math: '<S6>/Math Function6' incorporates:
   *  Constant: '<S6>/Constant7'
   */
  if ((rtb_X1_time[0] < 0.0F) && (7.0F > ((real32_T)floor(7.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 7.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 7.0F);
  }

  /* End of Math: '<S6>/Math Function6' */

  /* Product: '<S6>/Product7' incorporates:
   *  Constant: '<S6>/P9'
   *  Constant: '<S6>/constant4'
   */
  rtb_P9X7 = (8.0F * rtb_U_y) * -10549.1348F;

  /* Math: '<S6>/Math Function7' incorporates:
   *  Constant: '<S6>/Constant8'
   */
  if ((rtb_X1_time[0] < 0.0F) && (8.0F > ((real32_T)floor(8.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 8.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 8.0F);
  }

  /* End of Math: '<S6>/Math Function7' */

  /* Product: '<S6>/Product8' incorporates:
   *  Constant: '<S6>/P10'
   *  Constant: '<S6>/constant5'
   */
  rtb_P10X8 = (9.0F * rtb_U_y) * 5265.83936F;

  /* Math: '<S6>/Math Function8' incorporates:
   *  Constant: '<S6>/Constant9'
   */
  if ((rtb_X1_time[0] < 0.0F) && (9.0F > ((real32_T)floor(9.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 9.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 9.0F);
  }

  /* End of Math: '<S6>/Math Function8' */

  /* Sum: '<S4>/Add1' incorporates:
   *  Constant: '<S4>/R0'
   */
  rtb_Add1_c = (rtb_Add1_c - 0.00231660809F) - rtb_Divide1;

  /* Reshape: '<S3>/Reshape' incorporates:
   *  Constant: '<S6>/Constant1'
   *  Constant: '<S6>/P11'
   *  Constant: '<S6>/constant6'
   *  Constant: '<S6>/p2'
   *  Constant: '<S6>/p3'
   *  Product: '<S6>/Product1'
   *  Product: '<S6>/Product9'
   *  Sum: '<S6>/Add1'
   */
  rtb_Reshape_idx = ((((((((1.45828342F + ((2.0F * rtb_X1_time[0]) *
    -20.3367844F)) + rtb_P4X2) + rtb_p5x3) + rtb_p6x4) + rtb_p7x5) + rtb_p8x6) +
                      rtb_P9X7) + rtb_P10X8) + ((10.0F * rtb_U_y) * -1138.90942F);

  /* Product: '<S5>/CPC Multiply' incorporates:
   *  Constant: '<S3>/Constant1'
   *  Math: '<S5>/Math Function1'
   *  Reshape: '<S3>/Reshape'
   */
  for (i_5 = 0; i_5 < 3; i_5 = i_5 + 1) {
    rtb_P1_time_0[i_5] = 0.0F;
    rtb_P1_time_0[i_5] = rtb_P1_time_0[i_5] + (rtb_P1_time[i_5 + 0] *
      rtb_Reshape_idx);
    rtb_P1_time_0[i_5] = rtb_P1_time_0[i_5] + (rtb_P1_time[i_5 + 3] * 1.0F);
    rtb_P1_time_0[i_5] = rtb_P1_time_0[i_5] + (rtb_P1_time[i_5 + 6] * rtb_Add1_c);
  }

  /* Product: '<S5>/Divide' incorporates:
   *  Constant: '<S3>/Constant1'
   *  Constant: '<S5>/D2'
   *  Product: '<S5>/CPC Multiply'
   *  Reshape: '<S3>/Reshape'
   *  Sum: '<S5>/CPC_D'
   */
  rtb_U_y = 1.0F / ((((0.0F + (rtb_Reshape_idx * rtb_P1_time_0[0])) + (1.0F *
    rtb_P1_time_0[1])) + (rtb_Add1_c * rtb_P1_time_0[2])) + 0.1F);

  /* Product: '<S5>/K Multiply' incorporates:
   *  Constant: '<S3>/Constant1'
   *  Math: '<S5>/Math Function'
   *  Reshape: '<S3>/Reshape'
   */
  for (i_6 = 0; i_6 < 3; i_6 = i_6 + 1) {
    rtb_K[i_6] = 0.0F;
    rtb_K[i_6] = rtb_K[i_6] + (rtb_P1_time[i_6 + 0] * (rtb_Reshape_idx * rtb_U_y));
    rtb_K[i_6] = rtb_K[i_6] + (rtb_P1_time[i_6 + 3] * (1.0F * rtb_U_y));
    rtb_K[i_6] = rtb_K[i_6] + (rtb_P1_time[i_6 + 6] * (rtb_Add1_c * rtb_U_y));
  }

  /* End of Product: '<S5>/K Multiply' */

  /* Product: '<S1>/Divide' incorporates:
   *  Constant: '<S1>/K_I'
   *  Inport: '<Root>/IN_I'
   */
  rtb_Divide_g = IN_I / 10.0F;

  /* MultiPortSwitch: '<S1>/I_float_A' incorporates:
   *  Inport: '<Root>/IN_I_Direct'
   *  RelationalOperator: '<S1>/Relational Operator'
   *  UnaryMinus: '<S1>/Unary Minus'
   */
  if ((IN_I_Direct == 2) == 0) {
    rtb_I_float_A = -rtb_Divide_g;
  } else {
    rtb_I_float_A = rtb_Divide_g;
  }

  /* End of MultiPortSwitch: '<S1>/I_float_A' */

  /* Math: '<S7>/Math Function' incorporates:
   *  Constant: '<S7>/Constant1'
   */
  if ((rtb_X1_time[0] < 0.0F) && (2.0F > ((real32_T)floor(2.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 2.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 2.0F);
  }

  /* End of Math: '<S7>/Math Function' */

  /* Product: '<S7>/Product1' incorporates:
   *  Constant: '<S7>/p3'
   */
  rtb_p2x_c = rtb_U_y * -20.3367844F;

  /* Math: '<S7>/Math Function1' incorporates:
   *  Constant: '<S7>/Constant2'
   */
  if ((rtb_X1_time[0] < 0.0F) && (3.0F > ((real32_T)floor(3.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 3.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 3.0F);
  }

  /* End of Math: '<S7>/Math Function1' */

  /* Product: '<S7>/Product2' incorporates:
   *  Constant: '<S7>/p4'
   */
  rtb_Product2 = rtb_U_y * 222.166489F;

  /* Math: '<S7>/Math Function2' incorporates:
   *  Constant: '<S7>/Constant3'
   */
  if ((rtb_X1_time[0] < 0.0F) && (4.0F > ((real32_T)floor(4.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 4.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 4.0F);
  }

  /* End of Math: '<S7>/Math Function2' */

  /* Product: '<S7>/Product3' incorporates:
   *  Constant: '<S7>/p5'
   */
  rtb_Product3 = rtb_U_y * -1248.69641F;

  /* Math: '<S7>/Math Function3' incorporates:
   *  Constant: '<S7>/Constant4'
   */
  if ((rtb_X1_time[0] < 0.0F) && (5.0F > ((real32_T)floor(5.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 5.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 5.0F);
  }

  /* End of Math: '<S7>/Math Function3' */

  /* Product: '<S7>/Product4' incorporates:
   *  Constant: '<S7>/p6'
   */
  rtb_Product4 = rtb_U_y * 4134.66113F;

  /* Math: '<S7>/Math Function4' incorporates:
   *  Constant: '<S7>/Constant5'
   */
  if ((rtb_X1_time[0] < 0.0F) && (6.0F > ((real32_T)floor(6.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 6.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 6.0F);
  }

  /* End of Math: '<S7>/Math Function4' */

  /* Product: '<S7>/Product5' incorporates:
   *  Constant: '<S7>/p7'
   */
  rtb_Product5 = rtb_U_y * -8745.2627F;

  /* Math: '<S7>/Math Function5' incorporates:
   *  Constant: '<S7>/Constant6'
   */
  if ((rtb_X1_time[0] < 0.0F) && (7.0F > ((real32_T)floor(7.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 7.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 7.0F);
  }

  /* End of Math: '<S7>/Math Function5' */

  /* Product: '<S7>/Product6' incorporates:
   *  Constant: '<S7>/p8'
   */
  rtb_Product6 = rtb_U_y * 12078.8047F;

  /* Math: '<S7>/Math Function6' incorporates:
   *  Constant: '<S7>/Constant7'
   */
  if ((rtb_X1_time[0] < 0.0F) && (8.0F > ((real32_T)floor(8.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 8.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 8.0F);
  }

  /* End of Math: '<S7>/Math Function6' */

  /* Product: '<S7>/Product7' incorporates:
   *  Constant: '<S7>/p9'
   */
  rtb_Product7 = rtb_U_y * -10549.1348F;

  /* Math: '<S7>/Math Function7' incorporates:
   *  Constant: '<S7>/Constant8'
   */
  if ((rtb_X1_time[0] < 0.0F) && (9.0F > ((real32_T)floor(9.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 9.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 9.0F);
  }

  /* End of Math: '<S7>/Math Function7' */

  /* Product: '<S7>/Product8' incorporates:
   *  Constant: '<S7>/p10'
   */
  rtb_Product8 = rtb_U_y * 5265.83936F;

  /* Math: '<S7>/Math Function8' incorporates:
   *  Constant: '<S7>/Constant9'
   */
  if ((rtb_X1_time[0] < 0.0F) && (10.0F > ((real32_T)floor(10.0F)))) {
    rtb_U_y = -((real32_T)pow(-rtb_X1_time[0], 10.0F));
  } else {
    rtb_U_y = (real32_T)pow(rtb_X1_time[0], 10.0F);
  }

  /* End of Math: '<S7>/Math Function8' */

  /* Sum: '<S5>/U_y' incorporates:
   *  Constant: '<S1>/K_U'
   *  Constant: '<S3>/R0'
   *  Constant: '<S7>/p1'
   *  Constant: '<S7>/p11'
   *  Constant: '<S7>/p2'
   *  Inport: '<Root>/IN_U'
   *  Product: '<S1>/U_float_V'
   *  Product: '<S3>/Product'
   *  Product: '<S7>/Product'
   *  Product: '<S7>/Product9'
   *  Sum: '<S3>/Add'
   *  Sum: '<S3>/Add1'
   *  Sum: '<S7>/Add1'
   */
  rtb_U_y = (IN_U / 1000.0F) - (((0.00231660809F * (rtb_I_float_A - rtb_X1_time
    [2])) + rtb_X1_time[1]) + ((((((((((3.3919723F + (rtb_X1_time[0] *
    1.45828342F)) + rtb_p2x_c) + rtb_Product2) + rtb_Product3) + rtb_Product4) +
    rtb_Product5) + rtb_Product6) + rtb_Product7) + rtb_Product8) + (rtb_U_y *
    -1138.90942F)));

  /* Sum: '<S5>/ADD_X' incorporates:
   *  Product: '<S5>/Multiply'
   */
  rtb_ADD_X_idx = rtb_X1_time[0] + (rtb_K[0] * rtb_U_y);

  /* Outport: '<Root>/soc' incorporates:
   *  Constant: '<Root>/K_Dt'
   *  Product: '<Root>/Product'
   */
  *soc = rtb_ADD_X_idx * 1000.0F;

  /* Update for UnitDelay: '<Root>/INI_X' incorporates:
   *  Product: '<S5>/Multiply'
   *  Sum: '<S5>/ADD_X'
   */
  Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[0] = rtb_ADD_X_idx;
  Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[1] = rtb_X1_time[1] + (rtb_K[1] *
    rtb_U_y);
  Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[2] = rtb_X1_time[2] + (rtb_K[2] *
    rtb_U_y);

  /* Update for UnitDelay: '<Root>/I' */
  Closed_Loop_NCM_29Ah_chan_DWork.I_DSTATE = rtb_I_float_A;

  /* Sum: '<S5>/Add2' incorporates:
   *  Constant: '<S3>/Constant1'
   *  Constant: '<S5>/Constant1'
   *  Product: '<S5>/Matrix Multiply3'
   *  Product: '<S5>/Matrix Multiply4'
   *  Reshape: '<S3>/Reshape'
   */
  for (i_7 = 0; i_7 < 3; i_7 = i_7 + 1) {
    tmp_1[i_7 + 0] = rtCP_Constant1_Value_e[i_7 + 0] - (0.0F + (rtb_K[i_7] *
      rtb_Reshape_idx));
    tmp_1[i_7 + 3] = rtCP_Constant1_Value_e[i_7 + 3] - (0.0F + (rtb_K[i_7] *
      1.0F));
    tmp_1[i_7 + 6] = rtCP_Constant1_Value_e[i_7 + 6] - (0.0F + (rtb_K[i_7] *
      rtb_Add1_c));
  }

  /* End of Sum: '<S5>/Add2' */

  /* Product: '<S5>/Matrix Multiply4' */
  for (i_8 = 0; i_8 < 3; i_8 = i_8 + 1) {
    for (i_9 = 0; i_9 < 3; i_9 = i_9 + 1) {
      tmp_2[i_8 + (3 * i_9)] = 0.0F;
      tmp_2[i_8 + (3 * i_9)] = tmp_2[i_8 + (3 * i_9)] + (tmp_1[i_8 + 0] *
        rtb_P1_time[0 + (3 * i_9)]);
      tmp_2[i_8 + (3 * i_9)] = tmp_2[i_8 + (3 * i_9)] + (tmp_1[i_8 + 3] *
        rtb_P1_time[1 + (3 * i_9)]);
      tmp_2[i_8 + (3 * i_9)] = tmp_2[i_8 + (3 * i_9)] + (tmp_1[i_8 + 6] *
        rtb_P1_time[2 + (3 * i_9)]);
    }
  }

  /* Update for UnitDelay: '<Root>/INI_P' incorporates:
   *  Product: '<S5>/Matrix Multiply4'
   */
  for (i_a = 0; i_a < 9; i_a = i_a + 1) {
    Closed_Loop_NCM_29Ah_chan_DWork.INI_P_DSTATE[i_a] = tmp_2[i_a];
  }

  /* End of Update for UnitDelay: '<Root>/INI_P' */
}

/* Model initialize function */
void Closed_Loop_NCM_29Ah_initialize(void)
{
  {
    int32_T i;

    /* InitializeConditions for UnitDelay: '<Root>/INI_X' */
    Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[0] = 1.0F;
    Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[1] = 0.0F;
    Closed_Loop_NCM_29Ah_chan_DWork.INI_X_DSTATE[2] = 0.0F;

    /* InitializeConditions for UnitDelay: '<Root>/INI_P' */
    for (i = 0; i < 9; i = i + 1) {
      Closed_Loop_NCM_29Ah_chan_DWork.INI_P_DSTATE[i] =
        rtCP_INI_P_InitialConditio[(i)];
    }

    /* End of InitializeConditions for UnitDelay: '<Root>/INI_P' */
  }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
