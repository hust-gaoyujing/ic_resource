/****************************************************************
* FILENAME: host.c
*
* BRIEF: Contains functions to interpret the data received from
* host send, appropriate commands to the TRF796x and return the
* response to host.
*
* Copyright (C) 2010 Texas Instruments, Inc.
*
* AUTHOR(S): Reiser Peter		DATE: 02 DEC 2010
*
* CHANGES:
* REV.	DATE		WHO	DETAIL
* 02	22Dec2010	RP	Included Write Command (Code 0xEB) for
* 						Mifare Ultralight in HostCommands()
*
****************************************************************/

#include "host.h"
#include "iso14443a.h"
#include "iso14443b.h"
#include "iso15693.h"
#include "type2.h"

//===============================================================

char* const revision = "Firmware Revision 06_16_2011\r\n\0";

//===============================================================

#if TRIGGER
	u08_t trigger = 0;
#endif

u08_t	first_spi_data = 1;
extern u08_t buf[300];
extern u08_t rx_error_flag;
extern s08_t rxtx_state;
extern u08_t irq_flag;
extern u08_t stand_alone_flag;
extern u08_t enable;
extern u08_t i_reg;

#ifdef ENABLE15693
	extern u08_t	afi;
	extern u08_t	flags;
#endif

//===============================================================

u08_t HostRequestCommand(u08_t *pbuf, u08_t length, u08_t broken_bits, u08_t no_crc);
u08_t HostRequest14443A(u08_t *pbuf, u08_t length, u08_t BitRate);

//===============================================================
// NAME: void HostCommands(void)
//
// BRIEF: Interprets the data received from host, initiates
// execution of right commands and return response to host.
//
// INPUTS:
//	Globals:
//		u08_t		buf[300]	contains received data from host
// OUTPUTS:
//	Globals:
//		u08_t		buf[300]	values for command execution
//
// PROCESS:	[1] get data from host
//			[2] interpret data
//			[3] start execution
//
// NOTE: Response for all unknown commands is “unknown command.”
//
// CHANGE:
// DATE  		WHO	DETAIL
// 18 Nov 2010	RP	Original Code
// 22 Dec 2010	RP	Write Command for MF UL Code 0xEB
//===============================================================

void
HostCommands(void)
{
	char *phello;
	u08_t i = 0;
	u08_t *pbuf, count = 0;
	
	STOP_COUNTER;

	if (stand_alone_flag == 0)
	{	
		pbuf = &buf[0];
		#if TRIGGER
			if(trigger != 0)
			{	for(i = 0; i < 50; i++)
				{	*(pbuf + i) = *(pbuf + i + 250);
				}
			}
			else
			{
				UartGetLine(pbuf);
				if(trigger != 0)
				{	for(i = 0; i < 50; i++)
					{	*(pbuf + i + 250) = *(pbuf + i);
					}
				}
				trigger = 0;
			}
		#else
			UartGetLine(pbuf);
		#endif
	}
	stand_alone_flag = 0;
	UartPutCrlf(); 						// finish line

	pbuf = &buf[4];
	rx_error_flag = 0;
	
	switch(*pbuf)
	{
		case 0xFF:							// check COM port number
			phello = "TRF7960 EVM \r\n";
			UartSendCString(phello);
			break;
		
#ifdef TYPE2_ENABLE							// can be enabled for TRF7960A in type2.h
		
		case 0x72:
			phello = "NFC Type 2 command\r\n";
			UartSendCString(phello);
			Type2Command(pbuf + 1);
			#if TRIGGER
				trigger = 1;
			#endif
			break;
		
#endif
	
		case 0x10:							// register write (adress:data, adress:data, ...)
			phello = "Register write request.\r\n";
			UartSendCString(phello);
			count = buf[0] - 8;
			Trf796xWriteSingle(&buf[5], count);
			for(i = 5; count > 0; i = i + 2)
			{
				if(buf[i] == ISO_CONTROL)
				{
					Trf796xWriteIsoControl(buf[i + 1]);
				}
				count--;
				count--;
			}
			break;
			
		case 0x11:							// continuous write (adress:data, data, ...)
			phello = "Continous write request.\r\n";
			UartSendCString(phello);
			count = buf[0] - 8;
			Trf796xWriteCont(&buf[5], count);
			break;
			
		case 0x12:							// register read (adress:data, adress:data, ...
			phello = "Register read request.\r\n";
			UartSendCString(phello);
			count = buf[0] - 8;
			Trf796xReadSingle(&buf[5], count);
			UartResponse(&buf[5], count);
			break;
			
		case 0x13:							//continuous read (adress:data, data, ...)
			phello = "Continous read request\r\n";
			UartSendCString(phello);
			pbuf++;
			count = *pbuf;					// the amount of registers to be read
	
			// is specified after the command
			pbuf++;
			buf[5] = *pbuf;					// the start register is speciffied
	
			// after the amount of registers
			Trf796xReadCont(&buf[5], count);
			UartResponse(&buf[5], count);
			break;
			
#ifdef ENABLE15693
		case 0x14:							// ISO 15693 Anticollision
			phello = "ISO 15693 Inventory request.\r\n";
			UartSendCString(phello);
			flags = buf[5];
			if(flags & 0x10)
			{	
				afi = buf[7];
			}
			else
			{	
				afi = 0;
			}
			for(count = 0; count < 8; count++)
			{	
				buf[count + 20] = 0x00;		// clear mask
			}
			Iso15693Anticollision(&buf[20], 0x00);
			break;
#endif
		case 0x15:
			phello = "Direct command.\r\n";
			UartSendCString(phello);
			Trf796xDirectCommand(&buf[5]);
			break;
			
		case 0x16:							// raw
			phello = "RAW mode.\r\n";
			UartSendCString(phello);
			count = buf[0] - 8;
			Trf796xRawWrite(&buf[5], count);
			break;
			
		case 0x18:							// request code
			phello = "Request mode.\r\n";
			UartSendCString(phello);
			count = buf[0] - 8;
			#if TRIGGER
				Trf796xTurnRfOn();
			#endif
			HostRequestCommand(&buf[0], count, 0x00, 0);
			#if TRIGGER
				trigger = 1;
			#endif
			break;
			
#ifdef ENABLE14443A
		case 0x19:
			// testing 14443A - sending and recieving
			// in different bitrates with changing
			// the ISOmode register after TX
			phello = "14443A Request - change bit rate.\r\n";
			UartSendCString(phello);
			count = buf[0] - 9;
			HostRequest14443A(&buf[1], count, buf[5]);
			break;
			
#endif
		case 0x0F:							// Direct mode
			phello = "Direct mode.\r\n";
			UartSendCString(phello);
			Trf796xDirectMode();
			break;
			
#ifdef ENABLE14443B
		case 0xB0:									// 0xB0 - REQB
			phello = "14443B REQB.\r\n";
		case 0xB1:									// 0xB1 - WUPB
			if(*pbuf == 0xB1)
			{
				phello = "14443B WUPB.\r\n";
			}
			UartSendCString(phello);
			iso14443bAnticollision(*pbuf, *(pbuf + 1));
			break;
			
#endif
#ifdef ENABLE14443A
		case 0xA0:									// 0xA0 - REQA
			phello = "14443A REQA.\r\n";
		case 0xA1:									// 0xA1 - WUPA
			*pbuf &= 0x0F;
			*pbuf ^= 0x01;
			if(*pbuf == 0)
			{
				phello = "14443A WUPA.\r\n";
			}
			UartSendCString(phello);
			Iso14443aAnticollision(*pbuf);
			break;
			
		case 0xA2:									// 0xA0 - REQA
			phello = "14443A Select.\r\n";
			UartSendCString(phello);
			switch(buf[0])
			{
				case 0x0D:
					for(count = 1; count < 6; count++)
					{	
						buf[99 + count] = *(pbuf + count);
					}
					break;
			
					case 0x11:
					for(count = 1; count < 11; count++)
					{	
						buf[100 + count] = *(pbuf + count);
					}
					buf[100] = 0x88;
					break;
			
				case 0x15:
					for(count = 1; count < 5; count++)
					{	
						buf[100 + count] = *(pbuf + count);
					}
					buf[100] = 0x88;
					buf[105] = 0x88;
					for(count = 1; count < 10; count++)
					{	
						buf[105 + count] = *(pbuf + count + 4);
					}
			}
			Trf796xWriteIsoControl(0x88);
		
			buf[5] = 0x26;							// send REQA command
			if(HostRequestCommand(&buf[0], 0x00, 0x0f, 1) == 0)
			{	
				if(Iso14443aSelectCommand(0x93, &buf[100]))
				{	
					if(Iso14443aSelectCommand(0x95, &buf[105]))
					{	
						Iso14443aSelectCommand(0x97, &buf[110]);
					}
				}
			}
			break;
#endif

		case 0x03:							// enable or disable the reader chip
			switch(*(pbuf + 1))
			{
				case 0xFF:	
					UartBaudSet(*(pbuf + 1));
					McuOscSel(*(pbuf + 1));
					TRF_DISABLE;
					UartSendCString("Reader disabled.");
					enable = 0;
					break;
					
				case 0x00:
					UartBaudSet(*(pbuf + 1));
					McuOscSel(*(pbuf + 1));
					TRF_ENABLE;
					UartSendCString("Reader enabled.");
					enable = 1;
					break;
					
				case 0x0A:
					UartBaudSet(0x00);
					McuOscSel(0x00);
					UartSendCString("External clock.");
					break;
					
				case 0x0B:
					UartBaudSet(0x01);
					McuOscSel(0x01);
					UartSendCString("Internal clock.");
					break;
			}
			break;
			
		case 0xF0:							// AGC toggle
			buf[0] = CHIP_STATE_CONTROL;
			buf[1] = CHIP_STATE_CONTROL;
			Trf796xReadSingle(&buf[1], 1);
			
			if(*(pbuf + 1) == 0xFF)
			{	
				buf[1] |= BIT2;
			}
			else
			{	
				buf[1] &= ~BIT2;
			}
			Trf796xWriteSingle(buf, 2);
			break;
			
		case 0xF1:							// AM PM toggle
			buf[0] = CHIP_STATE_CONTROL;
			buf[1] = CHIP_STATE_CONTROL;
			Trf796xReadSingle(&buf[1], 1);
			
			if(*(pbuf + 1) == 0xFF)
			{	
				buf[1] &= ~BIT3;
			}
			else
			{	
				buf[1] |= BIT3;
			}
			Trf796xWriteSingle(buf, 2);
			break;
			
		case 0xF2:							// Full - half power selection (FF - full power)
			buf[0] = CHIP_STATE_CONTROL;
			buf[1] = CHIP_STATE_CONTROL;
			Trf796xReadSingle(&buf[1], 1);
			
			if(*(pbuf + 1) == 0xFF)
			{	
				buf[1] &= ~BIT4;
			}
			else
			{	
				buf[1] |= BIT4;
			}
			Trf796xWriteSingle(buf, 2);
			break;
			
		case 0xFE:							// Firmware Version Number	
			UartSendCString(revision);
			break;
			
		case 0xF3:							// LED open on
			LED_OPEN2_ON;
			break;
			
		case 0xF4:							// LED open off
			LED_OPEN2_OFF;
			break;
			
		case 0xF5:							// LED TAG-IT on
			LED_OPEN1_ON;
			break;
			
		case 0xF6:							// LED TAG-IT off
			LED_OPEN1_OFF;
			break;
			
		case 0xF7:							// LED ISO15693 on
			LED_15693_ON;
			break;
			
		case 0xF8:							// LED ISO15693 off
			LED_15693_OFF;
			break;
		
		case 0xF9:							// LED typeB on
			LED_14443B_ON;
			break;
			
		case 0xFA:							// LED typeB off
			LED_14443B_OFF;
			break;
			
		case 0xFB:							// LED typeA on
			LED_14443A_ON;
			break;
			
		case 0xFC:							// LED typeA off
			LED_14443A_OFF;
			break;

#ifdef ENABLE14443A			
		case 0xE6:
			Iso14443aAnticollision(0x01);
			UartPutCrlf();
			for (count = 0; count<20; count++)
			{	
			}
			Iso14443aAnticollision(0x01);
			break;
#endif
			
	    default:
			phello = "Unknown command.\r\n";
			UartSendCString(phello);
			break;
	}										// switch
	Trf796xStopDecoders();	
	Trf796xReadIrqStatus(&buf[1]);
		
	while(!(IFG2 & UCA0TXIFG))						// till UART Transmit buffer is empty
	{
	}
}													// HostCommands

//===============================================================
// NAME: u08_t HostRequestCommand(u08_t *pbuf, u08_t length,
// 			u08_t broken_bits, u08_t no_crc)
//
// BRIEF: Is used for request and response handling and timing
// for VCD to VICC communication. Host command = 0x18
//
// INPUTS:
//	Parameters:
//		u08_t	*pbuf		indicates used part of buffer
//		u08_t   length		length of request
//		u08_t   broken_bits	bits in last broken byte
//		u08_t   no_crc		if no CRC should be sent
//	Globals:
//		u08_t	buf[300]	contains data for execution
//	
// OUTPUTS:
//	Globals:
//		u08_t		buf[300]	response
//	Return:
//		Type:	u08_t	Error Code:
//		Values:	0		normal response or collision
//				1		timeout
//
// PROCESS:	[1] set registers for request
//			[2] wait till end of request
//			[3] wait till end of response
//			[4] send response to host
//
// NOTE: Collisions returned as “(z)”.
//       Timeouts returned as “()”.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 18Nov2010	RP	Original Code
//===============================================================

u08_t
HostRequestCommand(u08_t *pbuf, u08_t length, u08_t broken_bits, u08_t no_crc)
{
	u08_t	index = 0, j = 0;

	u08_t	iso_control[4];
	u16_t	tx_timeout = 1, rx_timeout = 1;
	
	tx_timeout = length / 10 + 3;
	
	Trf796xReadSingle(iso_control, 1);
	iso_control[0] &= 0x1F;
	if(iso_control[0] < 8)															// ISO15693
	{
		tx_timeout = tx_timeout * 1;												//ORIGINAL VALUE = 4
		rx_timeout = rx_timeout * 1;
		
		if((iso_control[0] < 2) || (iso_control[0] == 3) || (iso_control[0] == 4)) // low bit rate
		{
			tx_timeout = tx_timeout * 4;										//ORIGINAL VALUE = 4
			rx_timeout = rx_timeout * 4;
		}
		
		if(iso_control[0] % 2 == 1)													// 1 out of 256
		{
			tx_timeout = tx_timeout * 8;											//ORIGINAL VALUE = 8
			rx_timeout = rx_timeout * 1;											//ORIGINALLY NOT PRESENT 
		}
	}

	rxtx_state = length; 					// rxtx_state extern variable is the main transmit counter

	*pbuf = 0x8f;
	if(no_crc == 1)
	{	
		*(pbuf + 1) = 0x90;					// buffer setup for FIFO writing
	}
	else
	{	
		*(pbuf + 1) = 0x91;					// buffer setup for FIFO writing
	}
	*(pbuf + 2) = 0x3d;
	*(pbuf + 3) = rxtx_state >> 4;
	*(pbuf + 4) = (rxtx_state << 4) | broken_bits;

	if(length > 12)
	{	
		length = 12;
	}

	if(length == 0x00 && broken_bits != 0x00)
	{	
		length = 1;
		rxtx_state = 1;
	}

	Trf796xRawWrite(pbuf, length + 5);		// send the request using RAW writing

	// Write 12 bytes the first time you write to FIFO
	IRQ_CLR;								// PORT2 interrupt flag clear
	IRQ_ON;

	rxtx_state = rxtx_state - 12;
	index = 17;

	i_reg = 0x01;

	while(rxtx_state > 0)
	{	
		irq_flag = 0x00;
		while(irq_flag == 0x00)				// wait for interrupt
		{
		}
		if(rxtx_state > 9)					// the number of unsent bytes is in the rxtx_state extern
		{	
			length = 10;					// count variable has to be 10 : 9 bytes for FIFO and 1 address
		}
		else if(rxtx_state < 1)
		{	
			break;							// return from interrupt if all bytes have been sent to FIFO
		}
		else
		{	
			length = rxtx_state + 1; 		// all data has been sent out
		}									// if

		buf[index - 1] = FIFO;				// writes 9 or less bytes to FIFO for transmitting
		Trf796xWriteCont(&buf[index - 1], length);
		rxtx_state = rxtx_state - 9;		// write 9 bytes to FIFO
		index = index + 9;
	}

	rxtx_state = 1;							// the response will be stored in buf[1] upwards
	
	j = 0;
	while((i_reg == 0x01) && (j <= tx_timeout))
	{
		COUNT_VALUE = COUNT_1ms * 2;		// for TIMEOUT
		START_COUNTER;						// start timer up mode
		irq_flag = 0x00;
		while(irq_flag == 0x00)
		{
		}
		j++;
	}
	
	i_reg = 1;

	if( (((buf[5] & BIT6) == BIT6) && ((buf[6] == 0x21) || (buf[6] == 0x22)|| (buf[6] == 0x23)|| (buf[6] == 0x24) ||
		(buf[6] == 0x27)|| (buf[6] == 0x28) || (buf[6] == 0x29)|| (buf[6] == 0x2A)))
		|| (buf[5] == 0x00 && ((buf[6] & 0xF0) == 0x20 || (buf[6] & 0xF0) == 0x30 || (buf[6] & 0xF0) == 0x40))
	  )
	{	
		McuDelayMillisecond(30);					 //originally 20mSec, 100mSec seems to much
		Trf796xReset();                            //took out to see if low data rate RMB could improve
		McuDelayMillisecond(30);	
		Trf796xTransmitNextSlot();
	}  
	
	j = 0;
	while((i_reg == 0x01) && (j <= rx_timeout))
	{
		COUNT_VALUE = COUNT_1ms * 14;		// for TIMEOUT original value 14
		START_COUNTER;						// start timer up mode
		irq_flag = 0x00;
		while(irq_flag == 0x00)
	{
	}
		j++;
	}

	if(j > rx_timeout)
	{
		i_reg = 0x00;
	}

	if(stand_alone_flag == 0)
	{	
		switch(no_crc)
		{
			case 0:
				if (i_reg == 0xFF)			// received response
				{
					UartPutChar('[');
					for(j = 1; j < rxtx_state; j++)
					{	
						UartPutByte(buf[j]);
					}
					UartPutChar(']');
					return(0);
				}
				else if (i_reg == 0x02)		// collision occured
				{	
					UartPutChar('[');
					UartPutChar('z');
					UartPutChar(']');
					return(0);
				}
				else if (i_reg == 0x00)		// timer interrupt
				{
					UartPutChar('[');
					UartPutChar(']');
					return(1);
				}
				break;
	
			case 1:
				if (i_reg == 0xFF)			// received response
				{
					UartPutChar('(');
					for(j = 1; j < rxtx_state; j++)
					{	
						UartPutByte(buf[j]);
					}
					UartPutChar(')');
					return(0);
				}
				else if (i_reg == 0x02)		// collision occured
				{
					UartPutChar('(');
					UartPutChar('z');
					UartPutChar(')');
					return(0);
				}
				else if (i_reg == 0x00)		// timer interrupt
				{
					UartPutChar('(');
					UartPutChar(')');
					return(1);
				}
		}									// switch
	}										// if

	IRQ_OFF;
	return(1);
}										// HostRequestCommand

//===============================================================
// NAME: u08_t HostRequestCommand(u08_t *pbuf, u08_t length,
// 			u08_t, u08_t broken_bits, u08_t no_crc)
//
// BRIEF: Is used for request and response handling and timing
// for VCD to VICC communication. Host command = 0x18
//
// INPUTS:
//	Parameters:
//		u08_t	*pbuf		indicates used part of buffer
//		u08_t   length		length of request
//		u08_t   broken_bits	bits in last broken byte
//		u08_t   no_crc		if no CRC should be sent
//	Globals:
//		u08_t	   buf[300]	contains data for execution
//	
// OUTPUTS:
//	Globals:
//		u08_t		buf[300]	values for command execution
//	Return:
//		Type:		u08_t		Error Code:
//		Values:	0		normal response or collision
//				1		timeout
//
// PROCESS:	[1] set registers for request
//			[2] wait till end of request
//			[3] wait till end of response
//			[4] send response to host
//
// NOTE: Collisions returned as “(z)”.
//       Timeouts returned as “()”.
//
// CHANGE:
// DATE  		WHO	DETAIL
// 18Nov2010	RP	Original Code
//===============================================================

u08_t
HostRequest14443A(u08_t *pbuf, u08_t length, u08_t bit_rate)
{
	u08_t	index = 0, j = 0, rx_bit_rate = 0, tx_bit_rate = 0;

	tx_bit_rate = ((bit_rate >> 4) & 0x0F) + 0x08;
	rx_bit_rate = (bit_rate & 0x0F) + 0x08;

	Trf796xWriteIsoControl(tx_bit_rate);

	rxtx_state = length; 					// rxtx_state extern variable is the main transmit counter

	*pbuf = 0x8f;
	*(pbuf + 1) = 0x91; 					// buffer setup for FIFO writing
	*(pbuf + 2) = 0x3d;
	*(pbuf + 3) = rxtx_state >> 4;
	*(pbuf + 4) = rxtx_state << 4;

	if(length > 12) length = 12;

	Trf796xRawWrite(pbuf, length + 5);		// send the request using RAW writing

	// Write 12 bytes the first time you write to FIFO
	IRQ_CLR;									// PORT2 interrupt flag clear
	IRQ_ON;

	rxtx_state = rxtx_state - 12;
	index = 18;

	i_reg = 0x01;

	while(rxtx_state > 0)
	{
		irq_flag = 0x00;
		while(irq_flag == 0x00)				// wait for interrupt
		{
		}
		if(rxtx_state > 9)					// the number of unsent bytes is in the rxtx_state extern
		{	
			length = 10;					// count variable has to be 10 : 9 bytes for FIFO and 1 address
		}
		else if(rxtx_state < 1)
		{	
			break;							// return from interrupt if all bytes have been sent to FIFO
		}
		else
		{	
			length = rxtx_state + 1; 		// all data has been sent out
		}

		buf[index - 1] = FIFO;				// writes 9 or less bytes to FIFO for transmitting
		Trf796xWriteCont(&buf[index - 1], length);
		rxtx_state = rxtx_state - 9;		// write 9 bytes to FIFO
		index = index + 9;
	}

	rxtx_state = 1;							// the response will be stored in buf[1] upwards
	while(i_reg == 0x01)					// wait for interrupt
	{
	}

	Trf796xWriteIsoControl(rx_bit_rate);

	Trf796xStopDecoders();

	Trf796xRunDecoders();

	i_reg = 0x01;

	McuCounterSet();
	COUNT_VALUE = COUNT_1ms * 30;			// 30 ms TIMEOUT
	START_COUNTER;							// start timer up mode

	while(i_reg == 0x01)
	{
	}										// wait for RX complete

	if(i_reg == 0xFF)
	{										// recieved response
		UartPutChar('[');
		for(j = 1; j < rxtx_state; j++)
		{
			UartPutByte(buf[j]);
		}

		UartPutChar(']');
		return(0);
	}
	else if(i_reg == 0x02)					// collision occured
	{	
		UartPutChar('[');
		UartPutChar('z');
		UartPutChar(']');
		return(0);
	}
	else if(i_reg == 0x00)			// timer interrupt
	{	
		UartPutChar('[');
		UartPutChar(']');
		return(1);
	}
	else
	{
	}

	IRQ_OFF;
	return(1);
}									// HostRequest14443A
