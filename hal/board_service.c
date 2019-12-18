/*******************************************************************************
  Board Service Configuration Routine Source File

  File Name:
    board_service.c

  Summary:
    This file includes subroutine for initializing peripherals of Controller
	and control the hardware of the development board.

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
#include <stdint.h>
#include <stdbool.h>
#include "board_service.h"

BUTTON_T buttonStartStop;
#ifdef MCLV2
    BUTTON_T buttonFwdRev;
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
static void ButtonGroupInitialize(void);
static void ButtonScan(BUTTON_T * ,bool);
uint16_t boardServiceISRCounter = 0;
// *****************************************************************************
bool IsPressed_Button1(void)
{
    if(buttonStartStop.status)
    {
        buttonStartStop.status = false;
        return true;
    }
    else
    {
        return false;
    }
}
#ifdef MCLV2
bool IsPressed_Button2(void)
{
    if(buttonFwdRev.status)
    {
        buttonFwdRev.status = false;
        return true;
    }
    else
    {
        return false;
    }
}
#endif
void BoardServiceStepIsr(void)
{
    if (boardServiceISRCounter <  BOARD_SERVICE_TICK_COUNT)
    {
        boardServiceISRCounter += 1;
    }
}
void BoardService(void)
{
    if (boardServiceISRCounter ==  BOARD_SERVICE_TICK_COUNT)
    {
        /* Button scanning loop for Button 1 to start Motor A */
        ButtonScan(&buttonStartStop,BUTTON_START_STOP);
#ifdef MCLV2
        /* Button scanning loop for SW2 to enter into filed
            weakening mode */
        ButtonScan(&buttonFwdRev,BUTTON_FWD_REV);
#endif
        boardServiceISRCounter = 0;
    }
}
void BoardServiceInit(void)
{
    ButtonGroupInitialize();
    boardServiceISRCounter = BOARD_SERVICE_TICK_COUNT;
}

void ButtonScan(BUTTON_T *pButton,bool button) 
{
    if (button == true) 
    {
        if (pButton->debounceCount < BUTTON_DEBOUNCE_COUNT) 
        {
            pButton->debounceCount++;
            pButton->state = BUTTON_DEBOUNCE;
        }
    } 
    else 
    {
        if (pButton->debounceCount < BUTTON_DEBOUNCE_COUNT) 
        {
            pButton->state = BUTTON_NOT_PRESSED;
        } 
        else 
        {
            pButton->state = BUTTON_PRESSED;
            pButton->status = true;
        }
        pButton->debounceCount = 0;
    }
}
void ButtonGroupInitialize(void)
{
    buttonStartStop.state = BUTTON_NOT_PRESSED;
    buttonStartStop.debounceCount = 0;
    buttonStartStop.state = false;
#ifdef MCLV2
    buttonFwdRev.state = BUTTON_NOT_PRESSED;
    buttonFwdRev.debounceCount = 0;
    buttonFwdRev.state = false;
#endif
}
// *****************************************************************************
/* Function:
    InitPeripherals()

  Summary:
    Routine initializes controller peripherals

  Description:
    Routine to initialize Peripherals used for Inverter Control

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
void InitPeripherals(void)
{                
	InitOscillator();   
    SetupGPIOPorts();   
    Init_SCCP4 ();      
    CCP4CON1Lbits.CCPON = 1;
    PWM_Initialize();
    Init_ADC();
}
uint16_t HAL_HallValueRead(void)
{
    uint16_t buffer;
    uint16_t hallValue;
    
    buffer = PORTD;
    buffer = buffer >> 1;
    hallValue = buffer & 0x0007; 
    
    return hallValue;
}

void HAL_MC1PWMEnableOutputs(void)
{
    /* Set PWM Duty Cycles */
    MC1_PWM_PDC3 = 0;
    MC1_PWM_PDC2 = 0;
    MC1_PWM_PDC1 = 0;
    
    // Enable PWMs only on PWMxL ,to charge bootstrap capacitors initially.
    // Hence PWMxH is over-ridden to "LOW"
    PG3IOCONLbits.OVRENH = 0;  // 0 = PWM generator provides data for PWM3H pin
    PG2IOCONLbits.OVRENH = 0;  // 0 = PWM generator provides data for PWM2H pin
    PG1IOCONLbits.OVRENH = 0;  // 0 = PWM generator provides data for PWM1H pin

    PG3IOCONLbits.OVRENL = 0;  // 0 = PWM generator provides data for PWM3L pin
    PG2IOCONLbits.OVRENL = 0;  // 0 = PWM generator provides data for PWM2L pin
    PG1IOCONLbits.OVRENL = 0;  // 0 = PWM generator provides data for PWM1L pin
}
void HAL_MC1PWMDisableOutputs(void)
{
    /* Set PWM Duty Cycles */
    MC1_PWM_PDC3 = 0;
    MC1_PWM_PDC2 = 0;
    MC1_PWM_PDC1 = 0;
    
    PG3IOCONLbits.OVRDAT = 0;  // 0b00 = State for PWM3H,L, if Override is Enabled
    PG2IOCONLbits.OVRDAT = 0;  // 0b00 = State for PWM2H,L, if Override is Enabled
    PG1IOCONLbits.OVRDAT = 0;  // 0b00 = State for PWM1H,L, if Override is Enabled

    PG3IOCONLbits.OVRENH = 1;  // 1 = OVRDAT<1> provides data for output on PWM3H
    PG2IOCONLbits.OVRENH = 1;  // 1 = OVRDAT<1> provides data for output on PWM2H
    PG1IOCONLbits.OVRENH = 1;  // 1 = OVRDAT<1> provides data for output on PWM1H

    PG3IOCONLbits.OVRENL = 1;  // 1 = OVRDAT<0> provides data for output on PWM3L
    PG2IOCONLbits.OVRENL = 1;  // 1 = OVRDAT<0> provides data for output on PWM2L
    PG1IOCONLbits.OVRENL = 1;  // 1 = OVRDAT<0> provides data for output on PWM1L
}

/*
 * Writes three unique duty cycle values to the PWM duty cycle registers
 * corresponding to Motor #1.
 * Summary: Writes to the PWM duty cycle registers corresponding to Motor #1.
 * @param pdc Pointer to the array that holds duty cycle values
 * @example
 * <code>
 * HAL_PwmSetDutyCycles_Motor1(&pdcMotor1);
 * </code>
 */
void HAL_MC1PWMSetDutyCycles(MCAPP_DUTYCYCLEOUT_T *pdc)
{
    if(pdc->dutycycle3 < MIN_DUTY)
    {
        pdc->dutycycle3 = MIN_DUTY;
    }
    if(pdc->dutycycle2 < MIN_DUTY)
    {
        pdc->dutycycle2 = MIN_DUTY;
    }
    if(pdc->dutycycle1 < MIN_DUTY)
    {
        pdc->dutycycle1 = MIN_DUTY;
    }
   
    MC1_PWM_PDC3 = pdc->dutycycle3;
    MC1_PWM_PDC2 = pdc->dutycycle2;
    MC1_PWM_PDC1 = pdc->dutycycle1;
}
/*
 * Writes three unique duty cycle values to the PWM duty cycle registers
 * corresponding to Motor #1.
 * Summary: Writes to the PWM duty cycle registers corresponding to Motor #1.
 * @param pdc Pointer to the array that holds duty cycle values
 * @example
 * <code>
 * HAL_PwmSetDutyCycles_Motor1(&pdcMotor1);
 * </code>
 */
void HAL_MC1PWMSetDutyCyclesIdentical(int16_t dutyCycle)
{
    if(dutyCycle < MIN_DUTY)
    {
        dutyCycle = MIN_DUTY;
    }
    else if(dutyCycle > MAX_DUTY)
    {
        dutyCycle = MAX_DUTY;
    }
    
    MC1_PWM_PDC3 = dutyCycle;
    MC1_PWM_PDC2 = dutyCycle;
    MC1_PWM_PDC1 = dutyCycle;
}
  
void HAL_MC1MotorInputsRead(MCAPP_MEASURE_T *pMotorInputs)
{   
    pMotorInputs->measureCurrent.Ibus = (ADCBUF_IBUS);
    pMotorInputs->measurePot = (int16_t)(ADCBUF_POT >>1);
}