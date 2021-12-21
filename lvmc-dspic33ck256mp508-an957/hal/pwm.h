/*******************************************************************************
   Header File for High-Resolution PWM with Fine Edge Placement Configuration.

  File Name:
    pwm.h

  Summary:
    This header file lists routines to configure High-Resolution PWM with Fine 
    Edge Placement.

  Description:
     Definitions in the file are for dsPIC33CK256MP508 External OP-AMP PIM
    plugged onto Motor Control Development board from Microchip.

*******************************************************************************/
/*******************************************************************************
* Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.
*
* SOFTWARE LICENSE AGREEMENT:
* 
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
* STATUTORY DUTY),STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
* FOR ANY INDIRECT, SPECIAL,PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
* LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE,
* HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
* THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE,
* SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/
#ifndef _PWM_H
#define _PWM_H

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "clock.h"
// ***************************************************************************** 
 
// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************
        
#define START_DUTY       MPER>>2     
#define MIN_DUTY         0
#define MAX_DUTY         MPER - 1       

/* Specify PWM Frequency in Hertz */
#define PWMFREQUENCY_HZ         20000
/* Specify dead time in micro seconds */
#define DEADTIME_MICROSEC       1.0
/* Specify PWM Period in seconds, (1/ PWMFREQUENCY_HZ) */
#define LOOPTIME_SEC            0.00005
/* Specify PWM Period in micro seconds */
#define LOOPTIME_MICROSEC       50
        
// Specify bootstrap charging time in Seconds (mention at least 10mSecs)
#define BOOTSTRAP_CHARGING_TIME_SECS 0.01
  
// Calculate Bootstrap charging time in number of PWM Half Cycles
#define BOOTSTRAP_CHARGING_COUNTS (uint16_t)((BOOTSTRAP_CHARGING_TIME_SECS/LOOPTIME_SEC )* 2)
        
// Definition to enable or disable PWM Fault
// Enable only one mode at a time
//#define DISABLE_FAULT_MODE
//#define LATCHED_FAULT_MODE
#define CYCLE_BY_CYCLE_FAULT_MODE

        
#define DEADTIME               (uint16_t)(DEADTIME_MICROSEC*FOSC_MHZ)
// loop time in terms of PWM clock period
#define LOOPTIME_TCY            (uint16_t)(((LOOPTIME_MICROSEC*FOSC_MHZ)/2)-1)

/* Specify ADC Triggering Point w.r.t PWM Output for sensing Motor Currents */
#define ADC_SAMPLING_POINT      LOOPTIME_TCY-2
        
       
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************
void PWM_Initialize(void);        

inline static void PWM1_SwapOverrideEnableDataSet(uint16_t data)
{
    PG1IOCONL = data & 0X7C00;
}
inline static void PWM2_SwapOverrideEnableDataSet(uint16_t data)
{
    PG2IOCONL = data & 0X7C00;
}
inline static void PWM3_SwapOverrideEnableDataSet(uint16_t data)
{
    PG4IOCONL = data & 0X7C00;
}
inline static uint16_t PWM_MasterPeriodRead(void)
{
    return MPER;
}
        
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of PWM_H


