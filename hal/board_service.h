/*******************************************************************************
  Board Service Configuration Routine Source File
 
  File Name:
    board_service.h
 
  Summary:
    This file includes subroutine for initializing peripherals of Controller
 	 and control the hardware of the development board.
 
  Description:
    Definitions in the file are for dsPIC33CK256MP508 on Motor Control 
    Development board from Microchip
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

#ifndef __BOARD_SERVICE_H
#define __BOARD_SERVICE_H

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "clock.h"
#include "pwm.h"
#include "adc.h"
#include "port_config.h"
#include "timer.h"
#include "measure.h"
// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************  
#define MC1_PWM_PDC1      PG1DC
#define MC1_PWM_PDC2      PG2DC
#define MC1_PWM_PDC3      PG4DC

/** Button De-bounce in milli Seconds */
#define	BUTTON_DEBOUNCE_COUNT      30
/** The board service Tick is set as 1 millisecond - specify the count in terms 
 *  PWM ISR cycles (i.e. BOARD_SERVICE_TICK_COUNT = 1 milli Second / PWM period)
 */
#define BOARD_SERVICE_TICK_COUNT   20

#define HAL_MC1HallStateChangeInterrupt              _CNEInterrupt    
#define HAL_MC1HallStateChangeDetectionEnable        CN_PortEEnable        
#define HAL_MC1HallStateChangeInterruptFlagClear    CN_InterruptPortEFlagClear
#define HAL_MC1HallStateChangeMaxPeriodSet           SCCP4_SetTimerPeriod
#define HAL_MC1HallStateChangeTimerPrescalerSet      SCCP4_SetTimerPrescaler    
#define HAL_MC1HallStateChangeTimerStart             SCCP4_Timer_Start        
// *****************************************************************************
// *****************************************************************************
// Section: Enums, Structures
// *****************************************************************************
// *****************************************************************************  
/** Button Scanning State
 *
 * Description: This structure will host parameters required by Button scan routine. 
 */
typedef enum tagBUTTON_STATE
{
    BUTTON_NOT_PRESSED = 0, // wait for button to be pressed
    BUTTON_PRESSED = 1, // button was pressed, check if short press / long press
    BUTTON_DEBOUNCE = 2
} BUTTON_STATE;
    
// *****************************************************************************
/** Button data type
 * 
 *  Description: This structure will host parameters required by Button scan routine.
 */
typedef struct
{
   BUTTON_STATE state;
   uint16_t debounceCount;
   bool logicState;
   bool status;
} BUTTON_T;
/** Duty-cycle data type
 * 
 * Description: Parameters related to PWM module Duty Cycle values.
 */
typedef struct
{
    /** Duty cycle for phase #1  */
    uint16_t dutycycle1;
    
    /** Duty cycle for phase #2  */
    uint16_t dutycycle2;
    
    /** Duty cycle for phase #3  */
    uint16_t dutycycle3;
} MCAPP_DUTYCYCLEOUT_T;
// <editor-fold defaultstate="expanded" desc="ENUMERATED CONSTANTS ">
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void HAL_BoardServiceInit(void);
void HAL_BoardServiceStepIsr(void);
void HAL_BoardService(void);
bool HAL_IsPressed_Button1(void);
bool HAL_IsPressed_Button2(void);
void HAL_SystemInitialize(void);

void HAL_MC1PWMDisableOutputs(void);
void HAL_MC1PWMEnableOutputs(void);
void HAL_MC1PWMSetDutyCycles(MCAPP_DUTYCYCLEOUT_T *);
void HAL_MC1PWMSetDutyCyclesIdentical(int16_t dutyCycle);
void HAL_MC1MotorInputsRead(MCAPP_MEASURE_T *);
uint16_t HAL_MC1HallValueRead(void);
// *****************************************************************************
// *****************************************************************************
#endif /* __BOARD_SERVICE_H */
       