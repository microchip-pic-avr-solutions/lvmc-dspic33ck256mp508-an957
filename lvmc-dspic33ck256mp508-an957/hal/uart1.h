/*******************************************************************************
  Source File for UART Configuration for X2C Scope.
   File Name:
    uart1.h

  Summary:
    This file includes subroutine to configure UART1 Module.
	
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

#ifndef __UART1_H
#define __UART1_H
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
#include <stdbool.h>
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************                
/*  UART1_Initialize(void);
 * Enables and initializes UART1 with a default configuration:
 * - Continues module operation in Idle mode
 * - IrDA encoder and decoder are disabled
 * - UxRTS pin is in Flow Control mode
 * - UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx pins are
 *   controlled by PORT latches
 * - No wake-up is enabled
 * - Loop back mode is disabled
 * - Baud rate measurement is disabled
 * - UxRX Idle state is '1'
 * - BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
 * - 8-bit data, no parity
 * - One Stop bit.
 * This function does not configure the baud rate for UART1, however, it does
 * clear interrupt flags and disable interrupts for UART1 before initializing.
 * Summary: Enables and initializes UART1 with a default configuration.
 */        
extern void UART1_Initialize(void);

/* UART1_InterruptTransmitFlagClear();
 * Section: Driver Interface
 * Clears UART1 transmit interrupt request flag.
 * Summary: Clears UART1 transmit interrupt request flag.
 */
inline static void UART1_InterruptTransmitFlagClear(void) {_U1TXIF = 0; }

/* UART1_InterruptReceiveFlagClear();
 * Clears UART1 receive interrupt request flag.
 * Summary: Clears UART1 receive interrupt request flag.
 */
inline static void UART1_InterruptReceiveFlagClear(void) {_U1RXIF = 0; }

/* UART1_InterruptTransmitEnable();
 * Enables UART1 transmit interrupt.
 * Summary: Enables UART1 transmit interrupt.
 */
inline static void UART1_InterruptTransmitEnable(void) {_U1TXIE = 1; }

/* UART1_InterruptTransmitDisable();
 * Disables UART1 transmit interrupt.
 * Summary: Disables UART1 transmit interrupt.
 */
inline static void UART1_InterruptTransmitDisable(void) {_U1TXIE = 0; }

/* UART1_InterruptReceiveEnable();
 * Enables UART1 receive interrupt.
 * Summary: Enables UART1 receive interrupt.
 */
inline static void UART1_InterruptReceiveEnable(void) {_U1RXIE = 1; }

/* UART1_InterruptReceiveDisable();
 * Disables UART1 receive interrupt.
 * Summary: Disables UART1 receive interrupt
 */
inline static void UART1_InterruptReceiveDisable(void) {_U1RXIE = 0; }

/* UART1_SpeedModeStandard();
 * Configures UART1 module to operate in standard baud rate mode 
 *(i.e. 16x baud clock). 
 * Baud Rate = FREQ_UART_CLK (BCLKSEL)/ (16*(BRG +1))
 */
inline static void UART1_SpeedModeStandard(void) {U1MODEbits.BRGH = 0; }

/* UART1_SpeedModeHighSpeed();
 * Configures UART1 module to operate in High-speed baud rate mode 
 * (i.e. 4x baud clock).
 * Baud Rate = FREQ_UART_CLK (BCLKSEL) / (4*(BRG +1))
 */
inline static void UART1_SpeedModeHighSpeed(void) {U1MODEbits.BRGH = 1; }

/* UART1_BaudRateDividerSet(500);
 * Configures the baud rate divider for UART1 module.
 * Baud Rate = FREQ_UART_CLK (BCLKSEL)/ (16*(BRG +1)),if BRGH = 0, BCLKMOD = 0
 * Baud Rate = FREQ_UART_CLK (BCLKSEL)/ (4*(BRG +1)),if BRGH = 0, BCLKMOD = 0
 */
inline static void UART1_BaudRateDividerSet(uint16_t baudRateDivider)
{
    U1BRG = baudRateDivider;
}

/* UART1_ModuleDisable();
 * Disables UART1 module.
 * Summary: Disables UART1 module.
 */
inline static void UART1_ModuleDisable(void) 
{
    U1MODEbits.UARTEN = 0;
}

/* UART1_ModuleEnable();
 * Enables UART1 module.
 * Summary: Enables UART1 module.
 */
inline static void UART1_ModuleEnable(void) 
{
    U1MODEbits.UARTEN = 1;
}

/* UART1_TransmitModeEnable();
 * Enables UART1 module transmit mode.
 */
inline static void UART1_TransmitModeEnable(void) {U1MODEbits.UTXEN = 1; }

/* UART1_TransmitModeDisable();
 * Disables UART1 module transmit mode.
 */
inline static void UART1_TransmitModeDisable(void) {U1MODEbits.UTXEN= 0; }

/* UART1_IsReceiveBufferDataReady();
 * Gets the status of UART1 Receive Buffer Data Available flag
 * @return status of UART1 Receive Buffer Data Available flag; 
 * 1 = Receive buffer has data, 0 = Receive buffer is empty 
 */
inline static bool UART1_IsReceiveBufferDataReady(void)
{
    return(!U1STAHbits.URXBE) ;
}

/* UART1_IsReceiveBufferOverFlowDetected();
 * Gets the status of UART1 Receive Buffer Overrun Error Status flag
 * @return status of UART1 Receive Buffer Overrun Error Status flag; 
 * 1 = Receive buffer has overflowed, 0 = Receive buffer has not overflowed
 */
inline static bool UART1_IsReceiveBufferOverFlowDetected(void)
{
    return(U1STAbits.OERR) ;
}

/* UART1_IsFrameErrorDetected();
 * Gets the status of UART1 Framing Error Status flag
 * @return status of UART1 Framing Error Status flag; 
 * 1 = Framing error has been detected for the character at the top of 
 *     the receive FIFO
 * 0 = Framing error has not been detected
 */
inline static bool UART1_IsFrameErrorDetected(void)
{
    return(U1STAbits.FERR) ;
}

/* UART1_IsParityErrorDetected();
 * Gets the status of UART1 Parity Error Status flag
 * @return status of UART1 Parity Error Status flag; 
 * 1 = Parity error has been detected for the character 
 *     at the top of the receive FIFO
 * 0 = Parity error has not been detected
 */
inline static bool UART1_IsParityErrorDetected(void)
{
    return(U1STAbits.PERR) ;
}

/* UART1_IsReceiverIdle();
 * Gets the status of UART1 Receiver is Idle Status Flag
 * @return status of UART1 Receiver is Idle Status flag; 
 * 1 = Receiver is Idle
 * 0 = Receiver is Active
 */
inline static bool UART1_IsReceiverIdle(void)
{
    return(U1STAHbits.RIDLE) ;
}

/* UART1_IsTransmissionComplete();
 * Gets the status of UART1 Transmit Shift Register Empty Status Flag
 * @return status of UART1 Transmit Shift Register Empty Status flag; 
 * 1 = Transmit Shift Register is empty and transmit buffer is empty
 * 0 = Transmit Shift Register is not empty, a transmission is in progress 
 *     or queued
 */
inline static bool UART1_IsTransmissionComplete(void)
{
    return(U1STAbits.TRMT) ;
}

/* UART1_StatusBufferFullTransmitGet();
 * Gets the status of UART1 transmit buffer full flag
 * @return status of UART1 transmit buffer full flag; 1 = Transmit buffer is
 *  full, 0 = Transmit buffer is not full
 */
inline static bool UART1_StatusBufferFullTransmitGet(void)
{
    return U1STAHbits.UTXBF;
}

/* UART1_StatusGet();
 * Gets the transmitter and receiver status of UART1. The returned status may
 * contain a value with more than one of the bits specified in the
 * UART1_STATUS enumeration set. The caller should perform an "AND" with the
 * bit of interest and verify if the result is non-zero to verify the 
 * desired status bit.
 * @return UART1_STATUS value describing the current status of the transfer.
 */
inline static uint16_t UART1_StatusGet(void)
{
    return U1STA;
}

/* UART1_ReceiveBufferOverrunErrorFlagClear();
 * Clears the Receive Buffer Overrun Error Status bit for UART1. If this bit
 * was previously set, then calling this function will reset the receiver buffer
 * and the U1RSR to an empty state.
 */
inline static void UART1_ReceiveBufferOverrunErrorFlagClear(void)
{
    U1STAbits.OERR = 0;
}

/* UART1_DataWrite(txdata);
 * Writes a 16-bit data word to UART1 transmit register.
 * @param data data to be transmitted on UART1
 */
inline static void UART1_DataWrite(uint16_t data)
{
    U1TXREGbits.TXREG =(uint8_t)data;
}

/* UART1_DataRead();
 * Reads a 16-bit data word from the UART1 receive register.
 * @return data read from the UART1 receive register
 */
inline static uint16_t UART1_DataRead(void)
{
    return U1RXREG;
}
#endif      // end of __UART1_H
    