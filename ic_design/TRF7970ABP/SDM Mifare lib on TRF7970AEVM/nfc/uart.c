/*
 * File Name: uart.c
 *
 * Description: Contains functions to initialize UART interface using
 * USCI_A0 and communicate with the host via this interface.
 *
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


#include "uart.h"

//===============================================================

#ifdef ENABLE_UART

//===============================================================
// convert a nibble to ASCII hex byte                           ;
// output a binary coded byte as two hex coded ascii bytes      ;
// 02DEC2010	RP	Original Code
//===============================================================

u08_t
UartNibble2Ascii(u08_t anibble)
{
	u08_t	ascii_out = anibble;

	if(anibble > 9)							// If req ASCII A-F then add 7(hex)
	{	
		ascii_out = ascii_out + 0x07;
	}

	// Add offset to convert to Ascii
	ascii_out = ascii_out + 0x30;

	return(ascii_out);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutBksp(void)
{
	UartPutChar('\b');
	UartPutChar(' ');
	UartPutChar('\b');
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutByte(u08_t abyte)
{
	u08_t	temp1 = 0, temp2 = 0;

	temp1 = (abyte >> 4) & 0x0F;			// get high nibble
	temp2 = UartNibble2Ascii(temp1);		// convert to ASCII
	UartPutChar(temp2);						// output */

	temp1 = abyte & 0x0F;					// get low nibble
	temp2 = UartNibble2Ascii(temp1);		// convert to ASCII
	UartPutChar(temp2);						// output
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutChar(u08_t tx_char)
{
	while(!(IFG2 & UCA0TXIFG))				// till UART Transmit buffer is empty
	{
	}

	UCA0TXBUF = tx_char;						// send the character
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutNewLine(void)
{
	UartPutChar('\r');
	UartPutChar('\n');
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
UartPutSpace(void)
{
	UartPutChar(' ');
}

//===============================================================
// Send a specified number of bytes from buffer to host         ;
// 02DEC2010	RP	Original Code
//===============================================================

void
UartResponse(u08_t *pbuf, u08_t length)
{
	while(length > 0)
	{	
		UartPutChar('[');
		UartPutByte(*pbuf);
		UartPutChar(']');
		pbuf++;
		length--;
	}
	UartPutNewLine();
}

//===============================================================
//
//===============================================================

void
UartPutBuffer(const u08_t * pbuf, u08_t length)
{
	while(length > 0)
	{
		UartPutByte(*pbuf);
		pbuf++;
		length--;
	}
}

//===============================================================
//
//===============================================================

void
UartPutBufferAscii(const u08_t * pbuf, u08_t length)
{
	while(length > 0)
	{
		UartPutChar(*pbuf);
		pbuf++;
		length--;
	}
}

//===============================================================
// Send a character string to USART                             ;
// 02DEC2010	RP	Original Code
//===============================================================

void
UartSendCString(char *pstr)
{
	while(*pstr != '\0')
	{	
		UartPutChar(*pstr++);
	}
}

//===============================================================
// communication with host via USCI_A0                          ;
// 02DEC2010	RP	Original Code
//===============================================================

void
UartSetup(void)							// uses USCI_A0
{
    // modified for using the ez430-RF256x on the -G2553 LaunchPad
    P1SEL |= BIT2;						// P1.2=TXD
    // this is for TX only... (short TX & RX on the board P1.1 to P1.2)
    P1SEL2 |= BIT2;						// P1.2=TXD

    UCA0CTL1 |= UCSWRST;				// disable UART

    UCA0CTL0 = 0x00;

    UCA0CTL1 |= UCSSEL_2;				// SMCLK

    UCA0BR0 = 0x41;						// Baud Rate = 9600
    UCA0BR1 = 0x03;

    UCA0MCTL = 0x92;					// Modulation UCBRSx = 2

    UCA0CTL1 &= ~UCSWRST;				// Initialize USCI state machine
}

//===============================================================
//
//===============================================================

void
UartTrfDFReaderMsg(void)
{
	UartPutNewLine();
	UartSendCString("##################################################");
	UartPutNewLine();
	UartSendCString("   TRF7970A DESFire EV1 Reader has been enabled   "); 	// Board is alive and UART is up
	UartPutNewLine();
	UartSendCString("##################################################");
	UartPutNewLine();
	UartPutNewLine();
}

#endif

