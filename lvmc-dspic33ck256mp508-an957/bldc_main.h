/*******************************************************************************
 *  Sensored BLDCM Drive Configuration, Constants,Routine Header File
 * 
 *  File Name:
 *    bldc_main.h
 *   
 *  Summary:
 *    This header file lists BLDCM drive Configuration related functions and
 *    definitions.
 *
 *  Description:
 *    Definitions in the file are for dsPIC33CK256MP508 External OP-AMP PIM
 *    plugged onto Motor Control Development board from Microchip.
 */
/*******************************************************************************/
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
#ifndef BLDC_MAIN_H
#define	BLDC_MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "measure.h"
#include "clock.h"  
#include "timer.h"
#include "port_config.h"
#include "motor_control_types.h"
#include "motor_control_declarations.h"
// *****************************************************************************
// *****************************************************************************
// Section: MODE OF OPERATION
// Enable only one mode at a time
// *****************************************************************************
// *****************************************************************************   
#undef OPENLOOP
    
#define SPEED_PI_CLOSEDLOOP
    
#undef CURRENT_PI_CLOSEDLOOP
// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************
/** Motor Ratings (from Name Plate Details or Datasheet)*/
// *****************************************************************************
    
#define POLEPAIRS		 5		   // Number of pole pairs    
#define SECTOR           6         // Number of Electrical Sectors in Motor
#define MAX_MOTORSPEED   2000      // Specify the maximum speed in rpm of motor
#define MAX_MOTORCURRENT 1         // Max-Ampere Rating of Motor
#define MAX_BOARDCURRENT 22       // Max-Ampere Rating of MCLV2 Development Board

// *****************************************************************************    
    
#define REVERSE_DROP_SPEED   200   // Speed to be reduced in rpm before reversing the direction
// ***************************************************************************** 
/** Constants for Mathematical Computation */
#define TICKS            FCY/(SPEED_MEASURE_TIMER_PRESCALER)
//Timer count pre division for speed calculation
#define TimerCountPrescaler   6 //2^6 = 64
/**  SPEED MULTIPLIER CALCULATION = ((FCY*60)/(TIMER_PRESCALER*POLEPAIRS))  */
#define SPEED_MULTI     (unsigned long)(((float)FCY/(float)(SPEED_MEASURE_TIMER_PRESCALER*POLEPAIRS))*(float)60)
    
#define REV_SPEED_LIMIT   (unsigned long) ((float)(SPEED_MULTI)/(float)(REVERSE_DROP_SPEED))
    
#define DESIRED_CURRENT_LIMIT  ((float)32767/(float)(MAX_BOARDCURRENT))*(float)(MAX_MOTORCURRENT)  
    
#define OC_FAULT_TIMEOUT 20000    // Count to prevent the overcurrent happening due to the stalling of the rotor
//******************************************************************************
/** Velocity Control Loop Coefficients */    
#define SPEEDCNTR_PTERM        Q15(0.1)
#define SPEEDCNTR_ITERM        Q15(0.01)
#define SPEEDCNTR_CTERM        Q15(0.999)
#define SPEEDCNTR_OUTMAX       Q15(0.999)
#define SPEEDCNTR_OUTMIN       Q15(0.0)

/** Current Control Loop Coefficients */
#define CURRCNTR_PTERM         Q15(0.02)
#define CURRCNTR_ITERM         Q15(0.002)
#define CURRCNTR_CTERM         Q15(0.999)
#define CURRCNTR_OUTMAX        Q15(0.999)    
    
// *****************************************************************************
/** Moving Average - No of Samples*/
#define CURRENT_MOVING_AVG_FILTER_SCALE     4
#define CURRENT_MOVING_AVG_FILTER_SIZE      (uint16_t)(1 << CURRENT_MOVING_AVG_FILTER_SCALE)
#define SPEED_MOVING_AVG_FILTER_SCALE       4
#define SPEED_MOVING_AVG_FILTER_SIZE        (uint16_t)(1 << SPEED_MOVING_AVG_FILTER_SCALE)    
    
// *****************************************************************************      
/** Inverter Switching Array Index Table */
/*STATE3 and STATE1 swapped in order to achieve standard wire connection at Motor Terminal*/
const unsigned int PWM_STATE3_CLKW[8]	=	{0x3000,0x1000,0x3000,0x1000,0x2000,0x3000,0x2000,0x3000};
const unsigned int PWM_STATE2_CLKW[8]	=	{0x3000,0x3000,0x2000,0x2000,0x1000,0x1000,0x3000,0x3000};
const unsigned int PWM_STATE1_CLKW[8]	=	{0x3000,0x2000,0x1000,0x3000,0x3000,0x2000,0x1000,0x3000};

unsigned int PWM_STATE1[8];
unsigned int PWM_STATE2[8];
unsigned int PWM_STATE3[8];
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************    
void MCAPP_LoadSwitchingTable();             //Initialize motor 
void MCAPP_DiscretePI_Control(void);         //PI control
void MCAPP_InitMovingAvgCurrent(void);       //Initialize Moving Average Filter
void MCAPP_CalcMovingAvgCurrent(int16_t data);//Calculates Moving Average
void MCAPP_SpeedCalculate(uint32_t speed);    //Function to calculate the speed of the motor
void MCAPP_InitMovingAvgSpeed(void);          //Initialize Moving Average Speed Filter
void MCAPP_CalcMovingAvgSpeed(int16_t avgSpeed); //Calculates Speed by Moving Average
void MCAPP_StateMachine(void);               //Function to run the State Machine 
void MCAPP_InitControlParameters(void);      //Initialize PI controller values
void MCAPP_CheckHallUpdatePWM(void);          //Read Hall Port Data and update PWM switching
// *****************************************************************************
// *****************************************************************************
// Section: Enums, Structures
// *****************************************************************************
// ***************************************************************************** 
typedef struct
{
   uint16_t index;
   int16_t buffer[CURRENT_MOVING_AVG_FILTER_SIZE];
   int32_t sum;
   int16_t avg;
}MCAPP_CURRENT_MOVING_AVG_T;

typedef struct
{
   uint16_t index;
   int16_t buffer[SPEED_MOVING_AVG_FILTER_SIZE];
   int32_t sum;
   int16_t avg;
}MCAPP_SPEED_MOVING_AVG_T;

typedef struct
{    
    int32_t  timerDelta;
    uint32_t  timerPrev;
    uint32_t  timeIntervalMax;
    uint16_t  speedValue;
    uint16_t timerDeltaPreScaler;
} MCAPP_SPEED_CALC_T;

typedef struct
{
    uint16_t runMotor;
    uint16_t changeDirection;
    uint16_t sectorReady;
    uint16_t nextDirection;
    uint16_t state;
    uint16_t runCmd;
    uint16_t runDirection;
    
    uint32_t timerValue;
    
    uint16_t desiredSpeed;
    
    uint16_t desiredCurrent;
    int16_t  measuredCurrent;
       
    uint16_t sector;
    int16_t  dutyCycle;
    int16_t  pwmPeriod;
           
    MCAPP_MEASURE_T    analogInputs;
    MC_PIPARMIN_T      piInputCurrent;
    MC_PIPARMOUT_T     piOutputCurrent;
    MC_PIPARMIN_T      piInputSpeed;
    MC_PIPARMOUT_T     piOutputSpeed;
    MCAPP_SPEED_CALC_T    calculateSpeed;
    MCAPP_CURRENT_MOVING_AVG_T movingAvgFilterCurrent;
    MCAPP_SPEED_MOVING_AVG_T movingAvgFilterSpeed;
} MCAPP_DATA_T;

typedef enum
{
    MCAPP_INIT             = 0,         /* Initialize Run time parameters */
    MCAPP_CMD_WAIT         = 1,         /* Wait for Run command */
    MCAPP_OFFSET           = 2,         /* Measure current offsets */
    MCAPP_RUN              = 3,         /* Run the motor */
    MCAPP_CHANGE_DIRECTION = 4,         /* Change motor running direction */
    MCAPP_STOP             = 5,         /* Stop the motor */
    MCAPP_FAULT            = 6,         /* Motor is in Fault mode */
} MCAPP_STATE_T;

#endif	/** BLDC_MAIN_H */