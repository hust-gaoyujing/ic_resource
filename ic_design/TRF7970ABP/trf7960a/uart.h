//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _UART_H_
#define _UART_H_

//================================================================

#include <MSP430x23x0.h> 					// prozessor spezific header
//#include <stdio.h>							// standard input/output header
#include "mcu.h"
#include "types.h"

//===============================================================

#define BAUD0	0x3A	//baud rate generator = 115200 for 6.78 MHz Clock
#define BAUD1	0x00
#define BAUD0EN 0x2B	//baud rate for DCO = 4.9MHz
#define BAUD1EN 0x00

//===============================================================

// if disabled file host.c and uart.c may be excluded from build
#define ENABLE_HOST					// delete to disable remot mode

//===============================================================

void UartBaudSet(u08_t mode);
u08_t UartGetLine(u08_t *pline);
void UartPutBksp(void);
void UartPutByte(u08_t abyte);
void UartPutChar(u08_t TXchar);
void UartPutCrlf(void);
void UartPutSpace(void);
void UartResponse(u08_t *pbuf, u08_t length);
void UartSendCString(char *pstr);
void UartSetup (void);

//===============================================================

#endif
