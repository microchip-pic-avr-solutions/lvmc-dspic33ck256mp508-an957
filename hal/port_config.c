/*******************************************************************************
  Input / Output Port Configuration Routine source File

  File Name:
    port_config.c

  Summary:
    This file includes subroutine for initializing GPIO pins as analog/digital,
    input or output etc. Also to PPS functionality to Remap-able input or output 
    pins.

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
#include "port_config.h"

#undef MCBOARD_UART
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void CN_Configure (void);
void MapGPIOHWFunction (void);
// *****************************************************************************
/* Function:
    SetupGPIOPorts()

  Summary:
    Routine to set-up GPIO ports

  Description:
    Function initializes GPIO pins for input or output ports,analog/digital pins,
    remap the peripheral functions to desires RPx pins.

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */

void SetupGPIOPorts(void)
{
    // Reset all PORTx register (all inputs)
    #ifdef TRISA
        TRISA = 0xFFFF;
        LATA  = 0x0000;
    #endif
    #ifdef ANSELA
        ANSELA = 0x0000;
    #endif

    #ifdef TRISB
        TRISB = 0xFFFF;
        LATB  = 0x0000;
    #endif
    #ifdef ANSELB
        ANSELB = 0x0000;
    #endif

    #ifdef TRISC
        TRISC = 0xFFFF;
        LATC  = 0x0000;
    #endif
    #ifdef ANSELC
        ANSELC = 0x0000;
    #endif

    #ifdef TRISD
        TRISD = 0xFFFF;
        LATD  = 0x0000;
    #endif
    #ifdef ANSELD
        ANSELD = 0x0000;
    #endif

    #ifdef TRISE
        TRISE = 0xFFFF;
        LATE  = 0x0000;
    #endif
    #ifdef ANSELE
        ANSELE = 0x0000;
    #endif

    MapGPIOHWFunction();

    return;
}
// *****************************************************************************
/* Function:
    Map_GPIO_HW_Function()

  Summary:
    Routine to setup GPIO pin used as input/output analog/digital etc

  Description:
    Function initializes GPIO pins as input or output port pins,analog/digital 
    pins,remap the peripheral functions to desires RPx pins.

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */

void MapGPIOHWFunction(void)
{
    /* ANALOG SIGNALS */   
    // Potentiometer #1 input - used as Speed Reference
    // POT1 : PIM #32
    ANSELDbits.ANSELD11 = 1;
    TRISDbits.TRISD11 = 1;   // PIN36: AN19/CMP2C/RP75/PMPA0/PMPALL/PSPA0/RD11

    // IBUS : PIM #43
    ANSELCbits.ANSELC6 = 1;
    TRISCbits.TRISC6 = 1;   // PIN30: AN17/ANN1/IBIAS1/RP54/PMD12/PMA12/RC6
   
    /* Digital SIGNALS */   
    // Inverter Control - PWM Outputs
    // PWM1L : PIM #93  RP47/PWM1L/PMD6/RB15
    // PWM1H : PIM #94  RP46/PWM1H/PMD5/RB14
    // PWM2L : PIM #98  RP45/PWM2L/PMD4/RB13
    // PWM2H : PIM #99  TDI/RP44/PWM2H/PMD3/RB12
    // PWM3L : PIM #100 TCK/RP43/PWM3L/PMD2/RB11
    // PWM3H : PIM #03  TMS/RP42/PWM3H/PMD1/RB10
    TRISBbits.TRISB14 = 0 ;          
    TRISBbits.TRISB15 = 0 ;         
    TRISBbits.TRISB12 = 0 ;          
    TRISBbits.TRISB13 = 0 ;           
    TRISBbits.TRISB10 = 0 ;          
    TRISBbits.TRISB11 = 0 ;         
    
    // HALL Input Signals
    // HALLA : PIM #80  RP65/PWM4H/RD1
    // HALLB : PIM #47  RP66/RD2
    // HALLC : PIM #48  RP67/ASCL3/RD3
    TRISDbits.TRISD1 = 1 ;          
    TRISDbits.TRISD2 = 1 ;         
    TRISDbits.TRISD3 = 1 ; 
    
    // Debug LEDs
    // LED2 : PIM #01
    TRISEbits.TRISE9 = 0;           // PIN:44 - RE9
    // LED1 : PIM #60
    TRISEbits.TRISE8 = 0;           // PIN:42 - RE8

    // Push button Switches
#ifdef MCLV2    
    // S2 : PIM #83
    TRISDbits.TRISD5 = 1;           // PIN:54 - RP69/PMA15/PMCS2/RD5
    // S3 : PIM #84
    TRISEbits.TRISE7 = 1;           // PIN:39 - RE7
#endif

#ifdef MCHV2_MCHV3    
    // Push Button : PIM #68
    TRISEbits.TRISE5 = 1;   // PIN24: RE5 
#endif    
    
	/* Diagnostic Interface for MCLV-2,MCHV-2/3,LVMCDB etc.
        Re-map UART Channels to the device pins connected to the following 
        PIM pins on the Motor Control Development Boards .
        UART_RX : PIM #49 (Input)
        UART_TX : PIM #50 (Output)   */
    _U1RXR = 71;
    _RP70R = 0b000001;
    
    CN_Configure();
}

/* Function:
    CN_Configure()

  Summary:
    Routine to setup change notifications

  Description:
    Function initializes mismatch/edge change detection and enabling
	corresponding pins to obtain change notification status

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */

void CN_Configure(void)
{
    CNCOND = 0;
/*  ON: Change Notification (CN) Control for PORTx On bit
    1 = CN is enabled
    0 = CN is disabled   */
    CNCONDbits.ON = 0;
/*    CNSTYLE: Change Notification Style Selection bit
    1 = Edge style (detects edge transitions, bits are used for a CNE)
    0 = Mismatch style (detects change from last port read event)       */    
    CNCONDbits.CNSTYLE = 0;
     
    CNEN0D = 0;
    CNEN0Dbits.CNEN0D1 = 1;
    CNEN0Dbits.CNEN0D2 = 1;
    CNEN0Dbits.CNEN0D3 = 1;

    _CNDIF = 0;
    _CNDIE = 1;
    _CNDIP = 5;
}