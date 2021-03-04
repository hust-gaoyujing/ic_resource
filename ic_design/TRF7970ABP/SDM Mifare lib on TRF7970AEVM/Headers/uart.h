/*
 * File Name: uart.h
 *
 * Description: Header file for all functions for uart.c
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


#ifndef _UART_H_
#define _UART_H_

//================================================================

#include "mcu.h"
#include "trf797x.h"

//===============================================================

// TODO: Hardware modification to the default shipped board is required for UART:
// Take the two jumpers for the "TXD" and "RXD" pins and connect them horizontally
// instead of vertically. When done correctly, this should connect the pins which
// are labelled "TXD" and "RXD" pins which are on the MSP-EXP430G2 side of the
// board to each other. The two pins above them on the "Emulation" side of the
// board should also be tied together horizontally.

// Toggle UART Host Communication functionality
//#define ENABLE_UART

//===============================================================

#ifdef ENABLE_UART

u08_t UartNibble2Ascii(u08_t anibble);
void UartPutBksp(void);
void UartPutByte(u08_t abyte);
void UartPutChar(u08_t TXchar);
void UartPutNewLine(void);
void UartPutSpace(void);
void UartResponse(u08_t *pbuf, u08_t length);
void UartPutBuffer(const u08_t *pbuf, u08_t length);
void UartPutBufferAscii(const u08_t * pbuf, u08_t length);
void UartSendCString(char *pstr);
void UartSetup (void);
void UartTrfDFReaderMsg(void);


#endif

//===============================================================

#endif
