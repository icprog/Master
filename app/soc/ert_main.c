/*
 * File: ert_main.c
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

#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "Closed_Loop_NCM_29Ah.h"      /* Model's header file */
#include "PE_Types.h"  
//include "rtwtypes.h"                  /* MathWorks types */

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_t OverrunFlag = 0;

  /* '<Root>/Closed_Loop_In' */
  static Closed_Loop_InParamBus arg_Closed_Loop_In = {
    0.0F,                              /* IN_I */
    0U,                                /* IN_I_Direct */
    0.0F,                              /* IN_U */
    0.0F,                              /* IN_Dt */
    0.0F,                              /* IN_Soc_Init */

    {
      0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F }
    /* IN_P_Init */
  } ;

  /* '<Root>/Closed_Loop_Out' */
  static Closed_Loop_OutParamBus arg_Closed_Loop_Out;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(0, "Overrun");
    return;
  }

  OverrunFlag = TRUE;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  Closed_Loop_NCM_29Ah_custom(&arg_Closed_Loop_In, &arg_Closed_Loop_Out);

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = FALSE;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */

/*
 * The example "main" function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific.  This example
 * illustates how you do this relative to initializing the model.
 */

}

  
main (void)
{
	
	
	
}