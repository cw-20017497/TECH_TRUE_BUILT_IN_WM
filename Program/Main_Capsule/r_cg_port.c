/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_port.c
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100GE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for PORT module.
* Creation Date: 2025-04-16
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
    P1 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn6_OUTPUT_0 |
         _00_Pn7_OUTPUT_0;
    P2 = _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0 |
         _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
    P5 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0;
    P6 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0;
    P7 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0;
    P12 = _00_Pn0_OUTPUT_0;
    P13 = _00_Pn0_OUTPUT_0;
    P14 = _00_Pn0_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
    PMC12 = _00_PMCn0_DI_ON | _FE_PMC12_DEFAULT;
    PMC14 = _00_PMCn7_DI_ON | _7F_PMC14_DEFAULT;
    ADPC = _02_ADPC_DI_ON;
    PM1 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT |
          _10_PMn4_MODE_INPUT | _20_PMn5_MODE_INPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
    PM2 = _01_PMn0_NOT_USE | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT |
          _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
    PM3 = _01_PMn0_MODE_INPUT | _02_PMn1_NOT_USE | _FC_PM3_DEFAULT;
    PM5 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _FC_PM5_DEFAULT;
    PM6 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | _F0_PM6_DEFAULT;
    PM7 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _04_PMn2_MODE_INPUT | _08_PMn3_MODE_INPUT |
          _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _C0_PM7_DEFAULT;
    PM12 = _00_PMn0_MODE_OUTPUT | _FE_PM12_DEFAULT;
    PM14 = _00_PMn0_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT | _3E_PM14_DEFAULT;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
