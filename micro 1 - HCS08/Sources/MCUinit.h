/*
** ###################################################################
**     This code is generated by the Device Initialization Tool.
**     It is overwritten during code generation.
**     USER MODIFICATION ARE PRESERVED ONLY INSIDE EXPLICITLY MARKED SECTIONS.
**
**     Project   : Project_RTOS
**     Processor : MC9S08QE128CLK
**     Version   : Component 01.002, Driver 01.06, CPU db: 3.00.060
**     Datasheet : MC9S08QE128RM Rev. 2 6/2007
**     Date/Time : 12/8/2021, 11:48
**     Abstract  :
**         This module contains device initialization code 
**         for selected on-chip peripherals.
**     Contents  :
**         Function "MCU_init" initializes selected peripherals
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################
*/

#ifndef __Project_RTOS_H
#define __Project_RTOS_H 1

/* Include shared modules, which are used for whole project */


/* User declarations and definitions */
/*   Code, declarations and definitions here will be preserved during code generation */
/* End of user declarations and definitions */

#ifdef __cplusplus
extern "C" {
#endif
extern void MCU_init(void);
#ifdef __cplusplus
}
#endif
/*
** ===================================================================
**     Method      :  MCU_init (component MC9S08QE128_80)
**
**     Description :
**         Device initialization code for selected peripherals.
** ===================================================================
*/



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void isrVlvd(void);
/*
** ===================================================================
**     Interrupt handler : isrVlvd
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/





#pragma CODE_SEG DEFAULT

/* END Project_RTOS */

#endif
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
