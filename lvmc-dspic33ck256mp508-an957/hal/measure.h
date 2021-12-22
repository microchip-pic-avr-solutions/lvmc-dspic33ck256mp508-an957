/*******************************************************************************
  Current Measurement Routine Source File

  File Name:
    measure.h

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
* Microchip Technology  ("Microchip") retains all ownership and
* intellectual property rights in the code aIncorporatedccompanying this message and in all
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
#ifndef __MEASURE_H
#define __MEASURE_H

#ifdef __cplusplus
extern "C" {
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************
// Current Offset Constraints
#define OFFSET_COUNT_BITS   (int16_t)10
#define OFFSET_COUNT_MAX    (int16_t)(1 << OFFSET_COUNT_BITS)
// *****************************************************************************
// *****************************************************************************
// Section: Enums, Structures
// *****************************************************************************
// *****************************************************************************  
typedef struct
{
    int16_t
        offsetIa,       /* A phase current offset */
        offsetIb,       /* B phase current offset */
        offsetIbus,     /* BUS current offset */
        Ia,             /* A phase Current Feedback */
        Ib,             /* B phase Current Feedback */
        Ibus,           /* BUS current Feedback */
        counter,        /* counter */
        status;         /* flag to indicate offset measurement completion */

    int32_t
        sumIa,          /* Accumulation of Ia */
        sumIb,          /* Accumulation of Ib */
        sumIbus;        /* Accumulation of Ibus */

} MCAPP_MEASURE_CURRENT_T;

typedef struct
{
    int16_t
        value,              /* Measured value of DC Bus Voltage. */
        dcMinRun,           /* Minimum voltage for the motor to run */
        dcMaxStop;          /* Maximum voltage at which the motor would stop */
} MCAPP_MEASURE_VDC_T;

typedef struct
{
    int16_t
        Va,                 /* A phase terminal voltage w.r.t. DC_Neg */
        Vb,                 /* B phase terminal voltage w.r.t. DC_Neg */
        Vc,                 /* C phase terminal voltage w.r.t. DC_Neg */
        status,             /* Status if phase voltages are available */
        samplingFactor;     /* Ratio of sampling time to ADC interrupt */
}MCAPP_MEASURE_PHASEVOLT_T;

typedef struct
{
    int16_t 
        sharedCoreChannelNumber,    /* Shared core channel number for switching */
        measurePot;         /* Measure potentiometer */
    
    MCAPP_MEASURE_CURRENT_T
        measureCurrent;     /* Current measurement parameters */
        
    MCAPP_MEASURE_VDC_T
        measureVdc;         /* DC Bus Sensing parameters */
    
    MCAPP_MEASURE_PHASEVOLT_T
        measurePhaseVolt;   /* Phase voltage measurement parameters */
    
}MCAPP_MEASURE_T;
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void MCAPP_MeasureCurrentOffset (MCAPP_MEASURE_T *);
void MCAPP_MeasureCurrentCalibrate (MCAPP_MEASURE_T *);
void MCAPP_MeasureCurrentInit (MCAPP_MEASURE_T *);
int16_t MCAPP_MeasureCurrentOffsetStatus (MCAPP_MEASURE_T *);
#endif /* end of __MEASURE_H */
