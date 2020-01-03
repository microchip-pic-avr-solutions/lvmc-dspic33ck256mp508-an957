/*******************************************************************************
  Sensored BLDCM Drive main functions, interrupts and subroutines.

  File Name:
    bldc_main.c

  Summary:
    This main file runs the BLDCM drive related functions, interrupts and
    subroutines.

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
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "bldc_main.h"
#include "diagnostics.h"
#include "board_service.h"
#include "port_config.h"
#include "general.h"
#include "clock.h"
// *****************************************************************************
// *****************************************************************************
// Section: MODE OF OPERATION
// *****************************************************************************
// *****************************************************************************  
MCAPP_DATA_T     mcappData;
/******************************************************************************
 * Description: The main function initialises the microcontroller port configurations
 *              and settings. It is the main program which also reads the button 
 *              data, that runs the mtor as well as reverses the direction of spin.
 *****************************************************************************/
int main(void)
{
    HAL_SystemInitialize();
	DiagnosticsInit();
    HAL_BoardServiceInit();
    mcappData.state = MCAPP_INIT;
    HAL_MC1HallStateChangeTimerPrescalerSet(SPEED_MEASURE_TIMER_PRESCALER);
    HAL_MC1HallStateChangeMaxPeriodSet(SPEED_MULTI);
    HAL_MC1HallStateChangeTimerStart();
    LED1 = 1;
        
	while(1)
	{
        DiagnosticsStepMain();
        HAL_BoardService();

        if(HAL_IsPressed_Button1())
        {
            if(mcappData.runCmd == 0)
            {
                mcappData.runCmd = 1;
                LED2 = 1;
            }
            else
            {
                mcappData.runCmd = 0;
                LED2 = 0;
            }
        }
        if(HAL_IsPressed_Button2() && mcappData.changeDirection == 0)
        {
            mcappData.changeDirection = 1;
        }            
    }
}
/******************************************************************************
 * Description: The MCAPP_LoadSwitchingTable function initializes and loads the
 *              switching table into an array required for forward and reverse
 *              rotation of motor.
 *****************************************************************************/
void MCAPP_LoadSwitchingTable()
{
	uint16_t arrayIndex = 0;
    
	if(mcappData.runDirection == 1)
    {
	    for(arrayIndex = 0; arrayIndex < 8; arrayIndex++)
        {
            PWM_STATE1[arrayIndex] = PWM_STATE1_CLKW[arrayIndex];
            PWM_STATE2[arrayIndex] = PWM_STATE2_CLKW[arrayIndex];
            PWM_STATE3[arrayIndex] = PWM_STATE3_CLKW[arrayIndex];
		}
	}
    else
    {
		for(arrayIndex = 0;arrayIndex < 8; arrayIndex++)
        {
            PWM_STATE1[arrayIndex] = PWM_STATE1_CLKW[7-arrayIndex];
            PWM_STATE2[arrayIndex] = PWM_STATE2_CLKW[7-arrayIndex];
            PWM_STATE3[arrayIndex] = PWM_STATE3_CLKW[7-arrayIndex];
        }
    }
}
/******************************************************************************
 * Description: The MCAPP_CheckHallUpdatePWM function reads the Hall Sensor data
 *              and generates the switching pattern based on its sector.
 *****************************************************************************/
void MCAPP_CheckHallUpdatePWM(void)
{
    mcappData.sector = HAL_MC1HallValueRead();
    if((mcappData.sector >=1) && (mcappData.sector <= 6))
    {
        //set PWM overdrive to the corresponding PWM channel
        PWM1_SwapOverrideEnableDataSet(PWM_STATE1[mcappData.sector]);
        PWM2_SwapOverrideEnableDataSet(PWM_STATE2[mcappData.sector]);
        PWM3_SwapOverrideEnableDataSet(PWM_STATE3[mcappData.sector]);      
    }
}
/******************************************************************************
 * Description: The ADCAN19 Interrupt operates at 20kHz. The analog data such as 
 *              the potentiometer voltage, Bus Current are read. It services
 *              the X2C Scope ISR, The MCAPP_StateMachine routines as well.
 *****************************************************************************/
void __attribute__((__interrupt__,no_auto_psv)) HAL_MC1ADCInterrupt()
{
    DiagnosticsStepIsr();
/**  Routine to read the potentiometer value and the bus current value */    
    HAL_MC1MotorInputsRead(&mcappData.analogInputs);  
/**  Routine to run the Motor Control State Machine */    
    MCAPP_StateMachine();

    HAL_BoardServiceStepIsr();          
    HAL_MC1ADCInterruptFlagClear();
}
/******************************************************************************
 * Description: The CND Interrupt is serviced at every hall signal transition. 
 *              This enables to calculate the speed based on the time taken for
 *              360 degree electrical rotation.
 *****************************************************************************/
void __attribute__((interrupt, no_auto_psv)) HAL_MC1HallStateChangeInterrupt ()
{
    MCAPP_CheckHallUpdatePWM();

/** Calculation of time taken to complete 360 electrical degrees & is obtained from sector 1 */
/** The sector can be set to any value between 1 to 6 */   
    if (mcappData.sector == 1)
    {
        mcappData.timerValue = SCCP4_TimerDataRead();
    /** Routine to calculate the instantaneous speed from the timer value obtained */    
        MCAPP_SpeedCalculate(mcappData.timerValue);   
    /** Routine to find the moving average of the instantaneous speed values obtained */    
        MCAPP_CalcMovingAvgSpeed(mcappData.calculateSpeed.speedValue);        
    }

    HAL_MC1HallStateChangeInterruptFlagClear();
}
/******************************************************************************
 * Description: The MCAPP_StateMachine describes the code flow and controls the
 *              BLDC motor based on its state. Thus enabling smooth control of motor
 *              and is easily understood by the user.
 *****************************************************************************/
void MCAPP_StateMachine(void)
{
    switch(mcappData.state)
    {
        case MCAPP_INIT:
            
            HAL_MC1PWMDisableOutputs();
            
            mcappData.runMotor = 0;
            mcappData.pwmPeriod = PWM_MasterPeriodRead();
            mcappData.dutyCycle = START_DUTY;
            mcappData.timerValue = 0;
            mcappData.sector = 0;
            mcappData.calculateSpeed.timerDeltaPreScaler = TimerCountPrescaler;
            mcappData.calculateSpeed.speedValue = 0;
            MCAPP_InitControlParameters();
            MCAPP_InitMovingAvgCurrent();        //Routine to Initialize the Moving Average Filter Block
            MCAPP_InitMovingAvgSpeed();     
            HAL_MC1HallStateChangeDetectionEnable();
            mcappData.state = MCAPP_CMD_WAIT;
            
        break;

        case MCAPP_CMD_WAIT:
            
            if(mcappData.runCmd == 1)
            {
                MCAPP_LoadSwitchingTable(mcappData.runDirection);
                
                mcappData.runMotor = 1; 
                
                HAL_MC1HallStateChangeDetectionEnable();
                mcappData.state = MCAPP_RUN;
            }
            
            if(mcappData.changeDirection == 1)
            {              
                if(mcappData.runDirection == 0)
                {
                    mcappData.runDirection = 1;
                }
                else
                {
                    mcappData.runDirection = 0;
                }
                mcappData.changeDirection = 0;
                MCAPP_LoadSwitchingTable(mcappData.runDirection);
            }
        break;

        case MCAPP_OFFSET:
        break;

        case MCAPP_RUN:
          
            MCAPP_CheckHallUpdatePWM();
            
            #ifdef   OPENLOOP                      
            mcappData.dutyCycle = (int16_t)((__builtin_mulss(mcappData.analogInputs.measurePot,
                                        mcappData.pwmPeriod)>>15)); 
            if(mcappData.dutyCycle <= MIN_DUTY)
            {
                mcappData.dutyCycle = MIN_DUTY;
            }
            #endif

            #ifdef   SPEED_PI_CLOSEDLOOP   
            mcappData.desiredSpeed = (int16_t) (__builtin_mulss(MAX_MOTORSPEED, mcappData.analogInputs.measurePot) >> 15);
            mcappData.piInputSpeed.inMeasure = mcappData.calculateSpeed.speedValue;//mcappData.movingAvgFilterSpeed.avg;
            mcappData.piInputSpeed.inReference = mcappData.desiredSpeed;
            MC_ControllerPIUpdate_Assembly(mcappData.piInputSpeed.inReference,
                                           mcappData.piInputSpeed.inMeasure,
                                           &mcappData.piInputSpeed.piState,
                                           &mcappData.piOutputSpeed.out);
            mcappData.dutyCycle = (int16_t) (__builtin_mulss(mcappData.piOutputSpeed.out, mcappData.pwmPeriod) >> 15);
            #endif

            #ifdef   CURRENT_PI_CLOSEDLOOP
            mcappData.desiredCurrent = (int16_t) (__builtin_mulss(DESIRED_CURRENT_LIMIT, mcappData.analogInputs.measurePot) >> 15);
            mcappData.measuredCurrent = (mcappData.analogInputs.measureCurrent.Ibus); // zero offset can be set if required.
            MCAPP_CalcMovingAvgCurrent(mcappData.measuredCurrent);
            mcappData.piInputCurrent.inMeasure = mcappData.movingAvgFilterCurrent.avg;
            mcappData.piInputCurrent.inReference = mcappData.desiredCurrent;
            MC_ControllerPIUpdate_Assembly(mcappData.piInputCurrent.inReference,
                                           mcappData.piInputCurrent.inMeasure,
                                           &mcappData.piInputCurrent.piState,
                                           &mcappData.piOutputCurrent.out);
            mcappData.dutyCycle = (int16_t) (__builtin_mulss(mcappData.piOutputCurrent.out, mcappData.pwmPeriod) >> 15);
            #endif

            /** Set Duty Cycle */
            HAL_MC1PWMSetDutyCyclesIdentical(mcappData.dutyCycle);
            
            if(mcappData.changeDirection == 1)
            {
                mcappData.dutyCycle = 0;
                HAL_MC1PWMSetDutyCyclesIdentical(mcappData.dutyCycle);
                HAL_MC1PWMDisableOutputs();
                mcappData.state =  MCAPP_CHANGE_DIRECTION;
            }
            
            if(mcappData.runCmd == 0)
            {
                mcappData.state = MCAPP_STOP;
            }
            
        break;
        
        case MCAPP_CHANGE_DIRECTION:
            
            if(mcappData.changeDirection == 1)
            {              
                if(mcappData.runDirection == 0)
                {
                    mcappData.runDirection = 1;
                }
                else
                {
                    mcappData.runDirection = 0;
                }
                mcappData.changeDirection = 0;
                MCAPP_LoadSwitchingTable(mcappData.runDirection);
            }
            
            /*Wait for speed to reduce to a safe speed before reversing the direction*/
            if(mcappData.timerValue > REV_SPEED_LIMIT )  
            {
                mcappData.state = MCAPP_RUN;  
                HAL_MC1PWMEnableOutputs();
            }
            break;
        
        case MCAPP_STOP:
            
            mcappData.sector = 0;
            
            /** Set Duty Cycle */
            mcappData.dutyCycle = 0;
            HAL_MC1PWMSetDutyCyclesIdentical(mcappData.dutyCycle);
            HAL_MC1PWMDisableOutputs();
            
            mcappData.state = MCAPP_INIT;
            
        break;

        case MCAPP_FAULT:
        break;
    }
}
/******************************************************************************
 * Description: The MCAPP_InitControlParameters function initializes the
 *              standard PI constants that are set in user defined variables in
 *              bldc_main.h file
 *****************************************************************************/
void MCAPP_InitControlParameters(void)
{
    /** PI - Speed Control */
    mcappData.piInputSpeed.piState.kp         = SPEEDCNTR_PTERM;
    mcappData.piInputSpeed.piState.ki         = SPEEDCNTR_ITERM;
    mcappData.piInputSpeed.piState.kc         = SPEEDCNTR_CTERM;
    mcappData.piInputSpeed.piState.outMax     = SPEEDCNTR_OUTMAX;
    mcappData.piInputSpeed.piState.outMin     = SPEEDCNTR_OUTMIN;
    mcappData.piInputSpeed.piState.integrator = 0;
    mcappData.piOutputSpeed.out               = 0;
    
    /** PI - Current Control */
    mcappData.piInputCurrent.piState.kp         = CURRCNTR_PTERM;
    mcappData.piInputCurrent.piState.ki         = CURRCNTR_ITERM;
    mcappData.piInputCurrent.piState.kc         = CURRCNTR_CTERM;
    mcappData.piInputCurrent.piState.outMax     = CURRCNTR_OUTMAX;
    mcappData.piInputCurrent.piState.outMin     = 0;
    mcappData.piInputCurrent.piState.integrator = 0;
    mcappData.piOutputCurrent.out               = 0;
    
    mcappData.calculateSpeed.timeIntervalMax = SPEED_MULTI+1;
}
/******************************************************************************
 * Description: The MCAPP_InitMovingAvgCurrent function initializes the current 
 *              array table that is being used to calculate the moving average,
 *              thereby eliminate the undesired response and variations during
 *              reset and restart of motor.
 *****************************************************************************/
void MCAPP_InitMovingAvgCurrent(void)
{
    uint16_t i;
    
    for(i=0; i<CURRENT_MOVING_AVG_FILTER_SIZE; i++)
    {
        mcappData.movingAvgFilterCurrent.buffer[i] = 0;
    }
    
    mcappData.movingAvgFilterCurrent.index = 0;
    mcappData.movingAvgFilterCurrent.sum = 0;
    mcappData.movingAvgFilterCurrent.avg = 0;
}
/******************************************************************************
 * Description: The MCAPP_CalcMovingAvgCurrent function calculates the moving
 *              average of bus current.
 *****************************************************************************/
void MCAPP_CalcMovingAvgCurrent(int16_t instCurrent)
{
    uint16_t i;
    
    mcappData.movingAvgFilterCurrent.buffer[mcappData.movingAvgFilterCurrent.index] = instCurrent;
    mcappData.movingAvgFilterCurrent.index++;
    if(mcappData.movingAvgFilterCurrent.index >= CURRENT_MOVING_AVG_FILTER_SIZE)
       mcappData.movingAvgFilterCurrent.index = 0;
    
    mcappData.movingAvgFilterCurrent.sum = 0;
    for(i=0; i<CURRENT_MOVING_AVG_FILTER_SIZE; i++)
    {
        mcappData.movingAvgFilterCurrent.sum = mcappData.movingAvgFilterCurrent.sum + mcappData.movingAvgFilterCurrent.buffer[i];
        mcappData.movingAvgFilterCurrent.avg = mcappData.movingAvgFilterCurrent.sum >> CURRENT_MOVING_AVG_FILTER_SCALE;
    }
}
/******************************************************************************
 * Description: The MCAPP_SpeedCalculate calculates the difference in the time
 *              between two sector changes and determines the instantaneous
 *              speed of the motor.
 *****************************************************************************/
void MCAPP_SpeedCalculate(uint32_t speedValue)
{
    mcappData.calculateSpeed.timerDelta = speedValue - mcappData.calculateSpeed.timerPrev; 
    mcappData.calculateSpeed.timerPrev = speedValue;
    
    if(mcappData.calculateSpeed.timerDelta < 0)
    {
        mcappData.calculateSpeed.timerDelta = mcappData.calculateSpeed.timeIntervalMax + mcappData.calculateSpeed.timerDelta;
    }
    
    if(mcappData.calculateSpeed.timerDelta != 0)
    {
        if(mcappData.calculateSpeed.timerDelta >= 30000)
        {
            mcappData.calculateSpeed.timerDelta  = mcappData.calculateSpeed.timerDelta>>mcappData.calculateSpeed.timerDeltaPreScaler;   
            mcappData.calculateSpeed.speedValue = (int16_t)(__builtin_divud(SPEED_MULTI, mcappData.calculateSpeed.timerDelta));
            mcappData.calculateSpeed.speedValue = mcappData.calculateSpeed.speedValue>>mcappData.calculateSpeed.timerDeltaPreScaler;   
        }
        else
        {    
            mcappData.calculateSpeed.speedValue = (int16_t)(__builtin_divud(SPEED_MULTI, mcappData.calculateSpeed.timerDelta));
        }
    }
}
/******************************************************************************
 * Description: The MCAPP_InitMovingAvgSpeed function initializes the speed 
 *              array table that is being used to calculate the moving average,
 *              thereby eliminate the undesired response and variations during
 *              reset and restart of motor.
 *****************************************************************************/
void MCAPP_InitMovingAvgSpeed(void)
{
    uint16_t i;
    
    for(i=0; i<SPEED_MOVING_AVG_FILTER_SIZE; i++)
    {
        mcappData.movingAvgFilterSpeed.buffer[i] = 0;
    }
    
    mcappData.movingAvgFilterSpeed.index = 0;
    mcappData.movingAvgFilterSpeed.sum = 0;
    mcappData.movingAvgFilterSpeed.avg = 0;
}
/******************************************************************************
 * Description: The MCAPP_CalcMovingAvgSpeed function calculates the moving
 *              average of speed.
 *****************************************************************************/
void MCAPP_CalcMovingAvgSpeed(int16_t instSpeed)
{    
    uint16_t i;
    
    mcappData.movingAvgFilterSpeed.buffer[mcappData.movingAvgFilterSpeed.index] = instSpeed;
    mcappData.movingAvgFilterSpeed.index++;
    if(mcappData.movingAvgFilterSpeed.index >= SPEED_MOVING_AVG_FILTER_SIZE)
       mcappData.movingAvgFilterSpeed.index = 0;
    
    mcappData.movingAvgFilterSpeed.sum = 0;
    for(i=0; i<SPEED_MOVING_AVG_FILTER_SIZE; i++)
    {
        mcappData.movingAvgFilterSpeed.sum = mcappData.movingAvgFilterSpeed.sum + mcappData.movingAvgFilterSpeed.buffer[i];
        mcappData.movingAvgFilterSpeed.avg = mcappData.movingAvgFilterSpeed.sum >> SPEED_MOVING_AVG_FILTER_SCALE;
    }
}