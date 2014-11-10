/*
 * File: Closed_Loop_NCM_29Ah.c
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

#include "Closed_Loop_NCM_29Ah.h"
#include "Closed_Loop_NCM_29Ah_private.h"
#include "PE_Types.h"

/* Exported block parameters */
Closed_Loop_CtlParamBus Closed_Loop_CtlParam = {
  { 1.0e-10F, 0.0F, 0.0F, 0.0F, 1.0e-10F, 0.0F, 0.0F, 0.0F, 1.0e-10F },
  0.1F,
  31.45F,
  1000,
  10,
  1000,
  1000.0F,
  -50
} ;                                    /* Variable: Closed_Loop_CtlParam
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

/* Block signals and states (auto storage) */
D_Work_Closed_Loop_NCM_29Ah Closed_Loop_NCM_29Ah_DWork;

/* Exported data definition */
Closed_Loop_RCParamBus Closed_Loop_RCParam = {
  0.00231660809F,
  0.00310176F,
  19329.082F
} ;

/* Model step function */
void Closed_Loop_NCM_29Ah_custom(Closed_Loop_InParamBus *arg_Closed_Loop_In,
  Closed_Loop_OutParamBus *arg_Closed_Loop_Out)
{
  TPE_Float rtb_Dt_float_s;
  TPE_Float rtb_Divide;
  boolean_t rtb_ClosedLoopEnable;
  TPE_Float rtb_Add1_c;
  TPE_Float rtb_Divide1;
  TPE_Float rtb_MatrixA[9];
  TPE_Float rtb_K[3];
  TPE_Float rtb_P1_time[9];
  int32_t i;
  TPE_Float rtb_Add1_c_0[3];
  TPE_Float tmp[9];
  int32_t i_0;

  /* Chart: '<S1>/Event_Call' */
  /* Gateway: ClosedLoopActive/Event_Call */
  /* During: ClosedLoopActive/Event_Call */
  /* Entry Internal: ClosedLoopActive/Event_Call */
  /* Transition: '<S2>:5' */
  if (Closed_Loop_NCM_29Ah_DWork.F_Init == 1) {
    /* Transition: '<S2>:7' */
    /* Transition: '<S2>:9' */
    Closed_Loop_NCM_29Ah_DWork.F_Init = 0U;

    /* Outputs for Function Call SubSystem: '<S1>/SOC_Init' */
    /* SignalConversion: '<S4>/Signal Conversion' incorporates:
     *  BusSelector: '<S1>/Bus Selector'
     *  Constant: '<S4>/Constant1'
     *  Constant: '<S4>/Constant2'
     *  Inport: '<Root>/Closed_Loop_In'
     *  Product: '<S4>/Divide'
     *  Reshape: '<S4>/Reshape1'
     */
    /* Event: '<S2>:2' */
    Closed_Loop_NCM_29Ah_DWork.X_i[0] = arg_Closed_Loop_In->IN_Soc_Init /
      Closed_Loop_CtlParam.K_Soc;
    Closed_Loop_NCM_29Ah_DWork.X_i[1] = 0.0F;
    Closed_Loop_NCM_29Ah_DWork.X_i[2] = 0.0F;

    /* SignalConversion: '<S4>/Signal Conversion' incorporates:
     *  BusSelector: '<S1>/Bus Selector'
     *  Inport: '<Root>/Closed_Loop_In'
     */
    for (i = 0; i < 9; i++) {
      Closed_Loop_NCM_29Ah_DWork.P_i[i] = arg_Closed_Loop_In->IN_P_Init[i];
    }

    /* End of SignalConversion: '<S4>/Signal Conversion' */
    /* End of Outputs for SubSystem: '<S1>/SOC_Init' */
    /* Transition: '<S2>:11' */
  } else {
    /* Outputs for Function Call SubSystem: '<S1>/Soc_algorithm' */
    /* Chart: '<S5>/ClosedLoopEable' incorporates:
     *  Inport: '<Root>/Closed_Loop_In'
     */
    /* Transition: '<S2>:12' */
    /* Event: '<S2>:3' */
    /* Gateway: ClosedLoopActive/Soc_algorithm/ClosedLoopEable */
    /* During: ClosedLoopActive/Soc_algorithm/ClosedLoopEable */
    /* Entry Internal: ClosedLoopActive/Soc_algorithm/ClosedLoopEable */
    /* Transition: '<S7>:6' */
    if (((arg_Closed_Loop_In->IN_I > 5.0F) && (arg_Closed_Loop_In->IN_I !=
          ((TPE_Float)0xFFFF))) && (arg_Closed_Loop_In->IN_U != ((TPE_Float)0xFFFF)))
    {
      /* Transition: '<S7>:9' */
      /* Transition: '<S7>:14' */
      rtb_ClosedLoopEnable = TRUE;

      /* Transition: '<S7>:15' */
    } else {
      /* Transition: '<S7>:10' */
      rtb_ClosedLoopEnable = FALSE;
    }

    /* End of Chart: '<S5>/ClosedLoopEable' */

    /* Product: '<S8>/Dt_float_s' incorporates:
     *  Constant: '<S8>/K_Dt'
     *  Inport: '<Root>/Closed_Loop_In'
     */
    /* Transition: '<S7>:12' */
    rtb_Dt_float_s = arg_Closed_Loop_In->IN_Dt / ((TPE_Float)
      Closed_Loop_CtlParam.K_Dt);

    /* Product: '<S8>/Divide' incorporates:
     *  Constant: '<S8>/K_I'
     *  Inport: '<Root>/Closed_Loop_In'
     */
    rtb_Divide = arg_Closed_Loop_In->IN_I / ((TPE_Float)Closed_Loop_CtlParam.K_I);

    /* MultiPortSwitch: '<S8>/I_float_A' incorporates:
     *  Constant: '<S8>/Ch'
     *  Inport: '<Root>/Closed_Loop_In'
     *  RelationalOperator: '<S8>/Relational Operator'
     *  UnaryMinus: '<S8>/Unary Minus'
     */
    if ((arg_Closed_Loop_In->IN_I_Direct == 2) == 0) {
      rtb_Divide = -rtb_Divide;
    }

    /* End of MultiPortSwitch: '<S8>/I_float_A' */

    /* Outputs for Enabled SubSystem: '<S5>/Soc_algorithm' incorporates:
     *  EnablePort: '<S9>/ClosedLoopEnable'
     */
    if (rtb_ClosedLoopEnable) {
      /* Product: '<S11>/dt_q1' incorporates:
       *  Constant: '<S11>/Constant3600'
       *  Constant: '<S11>/Qn'
       *  Product: '<S11>/dt_q'
       */
      rtb_Add1_c = (rtb_Dt_float_s / Closed_Loop_CtlParam.Qn) / 3600.0F;

      /* Math: '<S11>/Math Function' incorporates:
       *  Constant: '<S11>/C1'
       *  Constant: '<S11>/R1'
       *  Product: '<S11>/Divide'
       *  Product: '<S11>/tao'
       *  UnaryMinus: '<S11>/Unary Minus1'
       *
       * About '<S11>/Math Function':
       *  Operator: exp
       */
      rtb_Dt_float_s = (TPE_Float)exp(-(rtb_Dt_float_s / (Closed_Loop_RCParam.R1 *
        Closed_Loop_RCParam.C1)));

      /* Product: '<S11>/Divide1' incorporates:
       *  Constant: '<S11>/Constant6'
       *  Constant: '<S11>/R1'
       *  Sum: '<S11>/Add'
       */
      rtb_Divide1 = (1.0F - rtb_Dt_float_s) * Closed_Loop_RCParam.R1;

      /* Reshape: '<S11>/Reshape' incorporates:
       *  Constant: '<S11>/Constant'
       *  Constant: '<S11>/Constant1'
       *  Constant: '<S11>/Constant2'
       *  Constant: '<S11>/Constant3'
       *  Constant: '<S11>/Constant4'
       *  UnaryMinus: '<S11>/Unary Minus'
       *  UnaryMinus: '<S11>/Unary Minus2'
       */
      rtb_MatrixA[0] = 1.0F;
      rtb_MatrixA[1] = 0.0F;
      rtb_MatrixA[2] = -rtb_Add1_c;
      rtb_MatrixA[3] = 0.0F;
      rtb_MatrixA[4] = rtb_Dt_float_s;
      rtb_MatrixA[5] = -rtb_Divide1;
      rtb_MatrixA[6] = 0.0F;
      rtb_MatrixA[7] = 0.0F;
      rtb_MatrixA[8] = 1.0F;

      /* SignalConversion: '<S11>/TmpSignal ConversionAtMatrix Multiply1Inport2' incorporates:
       *  Saturate: '<S11>/SOC_Saturation'
       *  UnitDelay: '<S1>/XandP'
       */
      if (Closed_Loop_NCM_29Ah_DWork.XandP_1_DSTATE[0] >= 0.95F) {
        rtb_Dt_float_s = 0.95F;
      } else if (Closed_Loop_NCM_29Ah_DWork.XandP_1_DSTATE[0] <= 0.0498F) {
        rtb_Dt_float_s = 0.0498F;
      } else {
        rtb_Dt_float_s = Closed_Loop_NCM_29Ah_DWork.XandP_1_DSTATE[0];
      }

      /* Reshape: '<S11>/Reshape2' incorporates:
       *  Constant: '<S11>/Constant5'
       */
      rtb_Add1_c_0[0] = rtb_Add1_c;
      rtb_Add1_c_0[1] = rtb_Divide1;
      rtb_Add1_c_0[2] = 0.0F;

      /* Sum: '<S11>/X1_time' incorporates:
       *  Product: '<S11>/Matrix Multiply'
       *  Product: '<S11>/Matrix Multiply1'
       *  SignalConversion: '<S11>/TmpSignal ConversionAtMatrix Multiply1Inport2'
       *  UnitDelay: '<S1>/XandP'
       *  UnitDelay: '<S5>/I'
       */
      for (i = 0; i < 3; i++) {
        Closed_Loop_NCM_29Ah_DWork.X_i[i] = (((rtb_MatrixA[i + 3] *
          Closed_Loop_NCM_29Ah_DWork.XandP_1_DSTATE[1]) + (rtb_MatrixA[i] *
          rtb_Dt_float_s)) + (rtb_MatrixA[i + 6] *
                              Closed_Loop_NCM_29Ah_DWork.XandP_1_DSTATE[2])) +
          (rtb_Add1_c_0[i] * Closed_Loop_NCM_29Ah_DWork.I_DSTATE);
      }

      /* End of Sum: '<S11>/X1_time' */

      /* Sum: '<S11>/Add1' incorporates:
       *  Constant: '<S11>/R0'
       */
      rtb_Add1_c = (rtb_Add1_c - Closed_Loop_RCParam.R0) - rtb_Divide1;

      /* Reshape: '<S10>/Reshape' incorporates:
       *  Constant: '<S13>/Constant'
       *  Constant: '<S13>/Constant1'
       *  Constant: '<S13>/P10'
       *  Constant: '<S13>/P11'
       *  Constant: '<S13>/P5'
       *  Constant: '<S13>/P6 '
       *  Constant: '<S13>/P7'
       *  Constant: '<S13>/P8 '
       *  Constant: '<S13>/P9'
       *  Constant: '<S13>/constant'
       *  Constant: '<S13>/constant1'
       *  Constant: '<S13>/constant2'
       *  Constant: '<S13>/constant3'
       *  Constant: '<S13>/constant4'
       *  Constant: '<S13>/constant5'
       *  Constant: '<S13>/constant6'
       *  Constant: '<S13>/p2'
       *  Constant: '<S13>/p3'
       *  Constant: '<S13>/p4'
       *  Math: '<S13>/Math Function1'
       *  Math: '<S13>/Math Function2'
       *  Math: '<S13>/Math Function3'
       *  Math: '<S13>/Math Function4'
       *  Math: '<S13>/Math Function5'
       *  Math: '<S13>/Math Function6'
       *  Math: '<S13>/Math Function7'
       *  Math: '<S13>/Math Function8'
       *  Product: '<S13>/Product1'
       *  Product: '<S13>/Product2'
       *  Product: '<S13>/Product3'
       *  Product: '<S13>/Product4'
       *  Product: '<S13>/Product5'
       *  Product: '<S13>/Product6'
       *  Product: '<S13>/Product7'
       *  Product: '<S13>/Product8'
       *  Product: '<S13>/Product9'
       *  Sum: '<S13>/Add1'
       */
      rtb_Dt_float_s = ((((((((((2.0F * Closed_Loop_NCM_29Ah_DWork.X_i[0]) *
        -20.3367844F) + 1.45828342F) + ((3.0F * ((TPE_Float)pow
        (Closed_Loop_NCM_29Ah_DWork.X_i[0], 2.0F))) * 222.166489F)) + ((4.0F *
                               ((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0],
        3.0F))) * -1248.69641F)) + ((5.0F * ((TPE_Float)pow
        (Closed_Loop_NCM_29Ah_DWork.X_i[0], 4.0F))) * 4134.66113F)) + ((6.0F *
                             ((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0],
        5.0F))) * -8745.2627F)) + ((7.0F * ((TPE_Float)pow
        (Closed_Loop_NCM_29Ah_DWork.X_i[0], 6.0F))) * 12078.8047F)) + ((8.0F *
                           ((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0],
        7.0F))) * -10549.1348F)) + ((9.0F * ((TPE_Float)pow
        (Closed_Loop_NCM_29Ah_DWork.X_i[0], 8.0F))) * 5265.83936F)) + ((10.0F *
        ((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0], 9.0F))) * -1138.90942F);

      /* Product: '<S11>/Matrix Multiply2' incorporates:
       *  Math: '<S15>/Math Function'
       *  UnitDelay: '<S1>/XandP'
       */
      for (i = 0; i < 3; i++) {
        for (i_0 = 0; i_0 < 3; i_0++) {
          tmp[i + (3 * i_0)] = 0.0F;
          tmp[i + (3 * i_0)] = tmp[(3 * i_0) + i] +
            (Closed_Loop_NCM_29Ah_DWork.XandP_2_DSTATE[i] * rtb_MatrixA[i_0]);
          tmp[i + (3 * i_0)] = tmp[(3 * i_0) + i] +
            (Closed_Loop_NCM_29Ah_DWork.XandP_2_DSTATE[i + 3] * rtb_MatrixA[i_0
             + 3]);
          tmp[i + (3 * i_0)] = tmp[(3 * i_0) + i] +
            (Closed_Loop_NCM_29Ah_DWork.XandP_2_DSTATE[i + 6] * rtb_MatrixA[i_0
             + 6]);
        }
      }

      /* Sum: '<S11>/P1_time' incorporates:
       *  Constant: '<S11>/D1'
       *  Product: '<S11>/Matrix Multiply2'
       */
      for (i = 0; i < 3; i++) {
        for (i_0 = 0; i_0 < 3; i_0++) {
          rtb_P1_time[i + (3 * i_0)] = (((tmp[(3 * i_0) + 1] * rtb_MatrixA[i + 3])
            + (tmp[3 * i_0] * rtb_MatrixA[i])) + (tmp[(3 * i_0) + 2] *
            rtb_MatrixA[i + 6])) + Closed_Loop_CtlParam.D1[(3 * i_0) + i];
        }
      }

      /* End of Sum: '<S11>/P1_time' */

      /* Product: '<S12>/CPC Multiply' incorporates:
       *  Math: '<S17>/Math Function'
       *  Reshape: '<S10>/Reshape'
       */
      for (i = 0; i < 3; i++) {
        rtb_Add1_c_0[i] = (rtb_P1_time[i + 6] * rtb_Add1_c) + (rtb_P1_time[i + 3]
          + (rtb_P1_time[i] * rtb_Dt_float_s));
      }

      /* Product: '<S12>/Divide' incorporates:
       *  Constant: '<S12>/D2'
       *  Product: '<S12>/CPC Multiply'
       *  Reshape: '<S10>/Reshape'
       *  Sum: '<S12>/CPC_D'
       */
      rtb_Divide1 = 1.0F / ((((rtb_Dt_float_s * rtb_Add1_c_0[0]) + rtb_Add1_c_0
        [1]) + (rtb_Add1_c * rtb_Add1_c_0[2])) + Closed_Loop_CtlParam.D2);

      /* Product: '<S12>/K Multiply' incorporates:
       *  Math: '<S16>/Math Function'
       *  Reshape: '<S10>/Reshape'
       */
      for (i = 0; i < 3; i++) {
        rtb_K[i] = (rtb_P1_time[i + 6] * (rtb_Add1_c * rtb_Divide1)) +
          ((rtb_P1_time[i + 3] * rtb_Divide1) + ((rtb_Dt_float_s * rtb_Divide1) *
            rtb_P1_time[i]));
      }

      /* End of Product: '<S12>/K Multiply' */

      /* Sum: '<S12>/U_y' incorporates:
       *  Constant: '<S10>/Constant'
       *  Constant: '<S14>/p1'
       *  Constant: '<S14>/p10'
       *  Constant: '<S14>/p11'
       *  Constant: '<S14>/p2'
       *  Constant: '<S14>/p3'
       *  Constant: '<S14>/p4'
       *  Constant: '<S14>/p5'
       *  Constant: '<S14>/p6'
       *  Constant: '<S14>/p7'
       *  Constant: '<S14>/p8'
       *  Constant: '<S14>/p9'
       *  Constant: '<S8>/K_U'
       *  Inport: '<Root>/Closed_Loop_In'
       *  Math: '<S14>/Math Function'
       *  Math: '<S14>/Math Function1'
       *  Math: '<S14>/Math Function2'
       *  Math: '<S14>/Math Function3'
       *  Math: '<S14>/Math Function4'
       *  Math: '<S14>/Math Function5'
       *  Math: '<S14>/Math Function6'
       *  Math: '<S14>/Math Function7'
       *  Math: '<S14>/Math Function8'
       *  Product: '<S10>/Product'
       *  Product: '<S14>/Product'
       *  Product: '<S14>/Product1'
       *  Product: '<S14>/Product2'
       *  Product: '<S14>/Product3'
       *  Product: '<S14>/Product4'
       *  Product: '<S14>/Product5'
       *  Product: '<S14>/Product6'
       *  Product: '<S14>/Product7'
       *  Product: '<S14>/Product8'
       *  Product: '<S14>/Product9'
       *  Product: '<S8>/U_float_V'
       *  Sum: '<S10>/Add'
       *  Sum: '<S10>/Add1'
       *  Sum: '<S14>/Add1'
       */
      rtb_Divide1 = (arg_Closed_Loop_In->IN_U / ((TPE_Float)
        Closed_Loop_CtlParam.K_U)) - ((((((((((((Closed_Loop_NCM_29Ah_DWork.X_i
        [0] * 1.45828342F) + 3.3919723F) + (((TPE_Float)pow
        (Closed_Loop_NCM_29Ah_DWork.X_i[0], 2.0F)) * -20.3367844F)) +
        (((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0], 3.0F)) * 222.166489F))
        + (((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0], 4.0F)) *
           -1248.69641F)) + (((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0],
        5.0F)) * 4134.66113F)) + (((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i
        [0], 6.0F)) * -8745.2627F)) + (((TPE_Float)pow
        (Closed_Loop_NCM_29Ah_DWork.X_i[0], 7.0F)) * 12078.8047F)) + (((TPE_Float)
        pow(Closed_Loop_NCM_29Ah_DWork.X_i[0], 8.0F)) * -10549.1348F)) +
        (((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0], 9.0F)) * 5265.83936F))
        + (((TPE_Float)pow(Closed_Loop_NCM_29Ah_DWork.X_i[0], 10.0F)) *
           -1138.90942F)) + (((rtb_Divide - Closed_Loop_NCM_29Ah_DWork.X_i[2]) *
        Closed_Loop_RCParam.R0) + Closed_Loop_NCM_29Ah_DWork.X_i[1]));

      /* SignalConversion: '<S12>/Signal Conversion' incorporates:
       *  Product: '<S12>/Multiply'
       *  Sum: '<S12>/ADD_X'
       */
      Closed_Loop_NCM_29Ah_DWork.X_i[0] += rtb_K[0] * rtb_Divide1;
      Closed_Loop_NCM_29Ah_DWork.X_i[1] += rtb_K[1] * rtb_Divide1;
      Closed_Loop_NCM_29Ah_DWork.X_i[2] += rtb_K[2] * rtb_Divide1;

      /* Sum: '<S12>/Add2' incorporates:
       *  Constant: '<S12>/Constant1'
       *  Product: '<S12>/Matrix Multiply3'
       *  Product: '<S12>/Matrix Multiply4'
       *  Reshape: '<S10>/Reshape'
       */
      for (i = 0; i < 3; i++) {
        tmp[i] = rtCP_Constant1_Value_e[(i)] - (rtb_K[i] * rtb_Dt_float_s);
        tmp[i + 3] = rtCP_Constant1_Value_e[i + 3] - rtb_K[i];
        tmp[i + 6] = rtCP_Constant1_Value_e[i + 6] - (rtb_K[i] * rtb_Add1_c);
      }

      /* End of Sum: '<S12>/Add2' */

      /* SignalConversion: '<S12>/Signal Conversion' incorporates:
       *  Product: '<S12>/Matrix Multiply4'
       */
      for (i = 0; i < 3; i++) {
        for (i_0 = 0; i_0 < 3; i_0++) {
          Closed_Loop_NCM_29Ah_DWork.P_i[i + (3 * i_0)] = 0.0F;
          Closed_Loop_NCM_29Ah_DWork.P_i[i + (3 * i_0)] =
            Closed_Loop_NCM_29Ah_DWork.P_i[(3 * i_0) + i] + (rtb_P1_time[3 * i_0]
            * tmp[i]);
          Closed_Loop_NCM_29Ah_DWork.P_i[i + (3 * i_0)] = (rtb_P1_time[(3 * i_0)
            + 1] * tmp[i + 3]) + Closed_Loop_NCM_29Ah_DWork.P_i[(3 * i_0) + i];
          Closed_Loop_NCM_29Ah_DWork.P_i[i + (3 * i_0)] = (rtb_P1_time[(3 * i_0)
            + 2] * tmp[i + 6]) + Closed_Loop_NCM_29Ah_DWork.P_i[(3 * i_0) + i];
        }
      }

      /* End of SignalConversion: '<S12>/Signal Conversion' */
    }

    /* End of Outputs for SubSystem: '<S5>/Soc_algorithm' */

    /* Update for UnitDelay: '<S5>/I' */
    Closed_Loop_NCM_29Ah_DWork.I_DSTATE = rtb_Divide;

    /* End of Outputs for SubSystem: '<S1>/Soc_algorithm' */
  }

  /* End of Chart: '<S1>/Event_Call' */

  /* Outport: '<Root>/Closed_Loop_Out' incorporates:
   *  BusCreator: '<S3>/Bus Creator'
   *  Constant: '<S6>/K_Dt'
   *  Product: '<S6>/Product'
   */
  /* Transition: '<S2>:14' */
  arg_Closed_Loop_Out->OUT_Soc = Closed_Loop_NCM_29Ah_DWork.X_i[0] *
    Closed_Loop_CtlParam.K_Soc;
  arg_Closed_Loop_Out->OUT_X2 = Closed_Loop_NCM_29Ah_DWork.X_i[1];
  arg_Closed_Loop_Out->OUT_X3 = Closed_Loop_NCM_29Ah_DWork.X_i[2];
  for (i = 0; i < 9; i++) {
    /* Outport: '<Root>/Closed_Loop_Out' incorporates:
     *  BusCreator: '<S3>/Bus Creator'
     */
    arg_Closed_Loop_Out->OUT_P[i] = Closed_Loop_NCM_29Ah_DWork.P_i[i];

    /* Update for UnitDelay: '<S1>/XandP' */
    Closed_Loop_NCM_29Ah_DWork.XandP_2_DSTATE[i] =
      Closed_Loop_NCM_29Ah_DWork.P_i[i];
  }

  /* Update for UnitDelay: '<S1>/XandP' */
  Closed_Loop_NCM_29Ah_DWork.XandP_1_DSTATE[0] = Closed_Loop_NCM_29Ah_DWork.X_i
    [0];
  Closed_Loop_NCM_29Ah_DWork.XandP_1_DSTATE[1] = Closed_Loop_NCM_29Ah_DWork.X_i
    [1];
  Closed_Loop_NCM_29Ah_DWork.XandP_1_DSTATE[2] = Closed_Loop_NCM_29Ah_DWork.X_i
    [2];
}

/* Model initialize function */
void Closed_Loop_NCM_29Ah_initialize(void)
{
  /* InitializeConditions for Chart: '<S1>/Event_Call' */
  Closed_Loop_NCM_29Ah_DWork.F_Init = 1U;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */