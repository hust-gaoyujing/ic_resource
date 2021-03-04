/*
 * File Name: trf797x.c
 *
 * Description: Contains functions to initialize and execute
 * communication with the TRF797x via the selected interface.
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

#include "trf797x.h"

//===============================================================

extern u08_t	buf[128];

#ifdef ENABLE14443A
static u08_t	g_ui8CollisionPosition;
#endif

static s08_t	g_s08RxTxState;
static u08_t	pui8Command[2];
static bool		g_bRXErrorFlag;
static u08_t	g_ui8FifoRxLength;

static tTRF797x_Status g_sTrf797xStatus;

static volatile u08_t g_ui8IrqFlag = 0x00;
u08_t g_ui8TimeoutFlag = 0x00;
// static volatile
//===============================================================
//                                                              ;
//===============================================================

void 
Trf797xCommunicationSetup(void)
{
	SpiSetup();
}

//===============================================================
// NAME: void Trf797xDirectCommand (u08_t *pbuf)
//
// BRIEF: Is used to transmit a Direct Command to Trf797x.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		Direct Command
//
// OUTPUTS:
//
// PROCESS:	[1] transmit Direct Command
//
//===============================================================

void 
Trf797xDirectCommand(u08_t *pbuf)
{
	SpiDirectCommand(pbuf);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xInitialSettings(void)
{	
	u08_t pui8ModControl[2];

	pui8ModControl[0] = SOFT_INIT;
	Trf797xDirectCommand(pui8ModControl);
	pui8ModControl[0] = IDLE;
	Trf797xDirectCommand(pui8ModControl);

	pui8ModControl[0] = 0x09;
	pui8ModControl[1] = 0x01;                    // ASK 100%, no SYS_CLK output
	Trf797xWriteSingle(pui8ModControl, 2);

    pui8ModControl[0] = 0x18;
    pui8ModControl[1] = 0x00;
    Trf797xWriteSingle(pui8ModControl, 2);
}

//===============================================================
// The Interrupt Service Routine determines how the IRQ should  ;
// be handled. The Trf797x IRQ status register is read to       ;
// determine the cause of the IRQ. Conditions are checked and   ;
// appropriate actions taken.                                   ;
//===============================================================

void
Trf797xISR(u08_t *irq_status)
{
	u08_t	ui8DummyRead;

#ifdef ENABLE14443A
	u08_t	ui8Length;
#endif

	if(*irq_status == 0xA0)			// BIT5 and BIT7
	{								// TX active and only 3 bytes left in FIFO
		g_sTrf797xStatus = TX_COMPLETE;
	}

	else if(*irq_status == TRF797x_IRQ_STATUS_TX_COMPLETE)
	{								// TX complete
		g_sTrf797xStatus = TX_COMPLETE;
		Trf797xReset();				// reset the FIFO after TX
	}

	else if((*irq_status & BIT1) == TRF797x_IRQ_STATUS_COLLISION_ERROR)
	{								// Collision error
		g_sTrf797xStatus = PROTOCOL_ERROR;

#ifdef ENABLE14443A
		g_ui8CollisionPosition = TRF797x_COLLISION_POSITION;
		Trf797xReadSingle(&g_ui8CollisionPosition, 1);

		ui8Length = g_ui8CollisionPosition - 0x20;		// number of valid bytes if FIFO

		if((ui8Length & 0x0f) != 0x00) ui8Length = ui8Length + 0x10;	// add 1 byte if broken byte recieved
		ui8Length = ui8Length >> 4;

		if(ui8Length != 0x00)
		{
			buf[g_s08RxTxState] = FIFO;		// write the recieved bytes to the correct place of the buffer

			Trf797xReadCont(&buf[g_s08RxTxState], ui8Length);
			g_s08RxTxState = g_s08RxTxState + ui8Length;
		}
#endif

		Trf797xStopDecoders();

		Trf797xReset();		// reset the FIFO after TX

		Trf797xResetIrqStatus();

		IRQ_CLR;
	}
	else if(*irq_status == TRF797x_IRQ_STATUS_RX_COMPLETE)
	{	// RX flag means that EOF has been recieved
		// and the number of unread bytes is in FIFOstatus regiter	
		if(g_bRXErrorFlag == true)
		{
			g_sTrf797xStatus = PROTOCOL_ERROR;
			return;
		}

		g_ui8FifoRxLength = TRF797x_FIFO_STATUS;
		Trf797xReadSingle(&g_ui8FifoRxLength, 1);	// determine the number of bytes left in FIFO

		g_ui8FifoRxLength = 0x7F & g_ui8FifoRxLength;
		buf[g_s08RxTxState] = FIFO;				// write the recieved bytes to the correct place of the buffer

		Trf797xReadCont(&buf[g_s08RxTxState], g_ui8FifoRxLength);
		g_s08RxTxState = g_s08RxTxState + g_ui8FifoRxLength;

		Trf797xReset();			// reset the FIFO after last byte has been read out

		g_sTrf797xStatus = RX_COMPLETE;
		g_bRXErrorFlag = false;
	}	
	else if(*irq_status == (TRF797x_IRQ_STATUS_RX_COMPLETE | TRF797x_IRQ_STATUS_FIFO_HIGH_OR_LOW))
	{									// RX active and 9 bytes allready in FIFO
		g_sTrf797xStatus = RX_WAIT;
		buf[g_s08RxTxState] = FIFO;
		Trf797xReadCont(&buf[g_s08RxTxState], 9);	// read 9 bytes from FIFO
		g_s08RxTxState = g_s08RxTxState + 9;

		if((IRQ_PORT & IRQ_PIN) == IRQ_PIN)			// if IRQ pin high
		{
			Trf797xReadIrqStatus(irq_status);

			IRQ_CLR;

			if(*irq_status == TRF797x_IRQ_STATUS_RX_COMPLETE)	// end of recieve
			{	
				g_ui8FifoRxLength = TRF797x_FIFO_STATUS;
				Trf797xReadSingle(&g_ui8FifoRxLength, 1);			// determine the number of bytes left in FIFO

				g_ui8FifoRxLength = 0x7F & g_ui8FifoRxLength;
				buf[g_s08RxTxState] = FIFO;						// write the recieved bytes to the correct place of the buffer

				Trf797xReadCont(&buf[g_s08RxTxState], g_ui8FifoRxLength);
				g_s08RxTxState = g_s08RxTxState + g_ui8FifoRxLength;

				Trf797xReset();		// reset the FIFO after last byte has been read out

				g_sTrf797xStatus = RX_COMPLETE;
			}
			else if(*irq_status == (TRF797x_IRQ_STATUS_RX_COMPLETE | TRF797x_IRQ_STATUS_CRC_ERROR))	// end of recieve and error
			{
				g_sTrf797xStatus = PROTOCOL_ERROR;
			}
		}
		else
		{
			Trf797xReadIrqStatus(irq_status);

			if(irq_status == 0x00)
			{	
				g_sTrf797xStatus = RX_COMPLETE;
			}
		}
	}
	else if((*irq_status & BIT4) == TRF797x_IRQ_STATUS_CRC_ERROR)		// CRC error
	{
		if((*irq_status & BIT5) == TRF797x_IRQ_STATUS_FIFO_HIGH_OR_LOW)
		{	
			g_sTrf797xStatus = RX_WAIT;
			g_bRXErrorFlag = true;
		}
		if((*irq_status & BIT6) == TRF797x_IRQ_STATUS_RX_COMPLETE)		// 4 Bit receive
		{
			ui8DummyRead = FIFO;		// write the recieved bytes to the correct place of the buffer

			Trf797xReadCont(&ui8DummyRead, 1);

			Trf797xReset();

			g_sTrf797xStatus = PROTOCOL_ERROR;
			g_bRXErrorFlag = true;
		}
		else
		{
			g_sTrf797xStatus = PROTOCOL_ERROR;
		}
	}
	else if((*irq_status & BIT2) == TRF797x_IRQ_STATUS_FRAMING_ERROR)	// byte framing error
	{
		if((*irq_status & BIT5) == TRF797x_IRQ_STATUS_FIFO_HIGH_OR_LOW)
		{
			g_sTrf797xStatus = RX_WAIT;
			g_bRXErrorFlag = true;
		}
		else
		{
			g_sTrf797xStatus = PROTOCOL_ERROR;
		}
	}
	else if(*irq_status == TRF797x_IRQ_STATUS_IDLE)
	{						// No response interrupt
		g_sTrf797xStatus = NO_RESPONSE_RECEIVED;
	}
	else if(*irq_status == TRF797x_IRQ_STATUS_NO_RESPONSE)
	{						// No response interrupt
		g_sTrf797xStatus = NO_RESPONSE_RECEIVED_15693;
	}
	else
	{						// Interrupt register not properly set
		g_sTrf797xStatus = PROTOCOL_ERROR;

		Trf797xStopDecoders();	// reset the FIFO after TX
		Trf797xReset();
		Trf797xResetIrqStatus();

		IRQ_CLR;
	}
}							// Interrupt Service Routine

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

#pragma vector = PORT2_VECTOR
__interrupt void
Trf797xPortB(void)							// interrupt handler
{
	u08_t ui8IrqStatus, ui8IsoControl;

	STOP_COUNTER;							// stop timer mode

	g_ui8IrqFlag = 0x01;

	do
	{
		IRQ_CLR;							// PORT2 interrupt flag clear

		// IRQ status register has to be read
		Trf797xReadIrqStatus(&ui8IrqStatus);

		if(ui8IrqStatus == 0xA0)				// TX active and only 3 bytes left in FIFO
		{
			break;
		}
		else
		{
			ui8IsoControl = TRF797x_ISO_CONTROL;
			Trf797xReadSingle(&ui8IsoControl, 1);
			if((ui8IsoControl & BIT5) != BIT5)			// RFID mode
			{
				Trf797xISR(&ui8IrqStatus);
			}
			else										// NFC mode
			{
				// Do Nothing
			}
		}
	} while((IRQ_PORT & IRQ_PIN) == IRQ_PIN);
}

//===============================================================
// NAME: void Trf797xRawWrite (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used to write direct to the Trf797x.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		raw data
//		u08_t		length		number of data bytes
//
// OUTPUTS:
//
// PROCESS:	[1] send raw data to Trf797x
//
//===============================================================

void 
Trf797xRawWrite(u08_t *pbuf, u08_t length)
{
	SpiRawWrite(pbuf, length);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xReConfig(void)
{
	SpiUsciExtClkSet();       			// Set the USART
}

//===============================================================
// NAME: void Trf797xReadCont (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used to read a specified number of Trf797x registers
// from a specified address upwards.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		address of first register
//		u08_t		length		number of registers
//
// OUTPUTS:
//
// PROCESS:	[1] read registers
//			[2] write contents to *pbuf
//
//===============================================================

void
Trf797xReadCont(u08_t *pbuf, u08_t length)
{
	SpiReadCont(pbuf, length);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xReadIrqStatus(u08_t *pbuf)
{
	*pbuf = TRF797x_IRQ_STATUS;
//	*(pbuf + 1) = TRF797x_IRQ_MASK;

	Trf797xReadCont(pbuf, 1);			// read second reg. as dummy read
}

//===============================================================
// NAME: void Trf797xReadSingle (u08_t *pbuf, u08_t number)
//
// BRIEF: Is used to read specified Trf797x registers.
//
// INPUTS:
//	Parameters:
//		u08_t		*pbuf		addresses of the registers
//		u08_t		number		number of the registers
//
// OUTPUTS:
//
// PROCESS:	[1] read registers
//			[2] write contents to *pbuf
//
//===============================================================

void
Trf797xReadSingle(u08_t *pbuf, u08_t number)
{
	SpiReadSingle(pbuf, number);
}

//===============================================================
// resets FIFO                                                  ;
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xReset(void)
{
	pui8Command[0] = RESET;
	Trf797xDirectCommand(pui8Command);
}

//===============================================================
// resets IRQ Status                                            ;
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xResetIrqStatus(void)
{
	u08_t puiIrqStatus[2];
	puiIrqStatus[0] = TRF797x_IRQ_STATUS;
	puiIrqStatus[1] = TRF797x_IRQ_MASK;

	Trf797xReadCont(puiIrqStatus, 2);		// read second reg. as dummy read
}

//===============================================================
//                                                              ;
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xRunDecoders(void)
{
	pui8Command[0] = RUN_DECODERS;
	Trf797xDirectCommand(pui8Command);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xStopDecoders(void)
{
	pui8Command[0] = STOP_DECODERS;
	Trf797xDirectCommand(pui8Command);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xTurnRfOff(void)
{
	pui8Command[0] = TRF797x_STATUS_CONTROL;
	pui8Command[1] = TRF797x_STATUS_CONTROL;
	Trf797xReadSingle(&pui8Command[1], 1);
	pui8Command[1] &= 0x1F;
	Trf797xWriteSingle(pui8Command, 2);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xTurnRfOn(void)
{
	pui8Command[0] = TRF797x_STATUS_CONTROL;
	pui8Command[1] = TRF797x_STATUS_CONTROL;
	Trf797xReadSingle(&pui8Command[1], 1);

	pui8Command[1] &= 0x3F;
	pui8Command[1] |= 0x20;
	Trf797xWriteSingle(pui8Command, 2);
}

//===============================================================
// NAME: void SpiWriteCont (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used to write to a specific number of reader chip
// registers from a specific address upwards.
//
// INPUTS:
//	u08_t	*pbuf	address of first register followed by the
//					contents to write
//	u08_t	length	number of registers + 1
//
// OUTPUTS:
//
// PROCESS:	[1] write to the registers
//
//===============================================================

void
Trf797xWriteCont(u08_t *pbuf, u08_t length)
{
	SpiWriteCont(pbuf, length);
}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xWriteIsoControl(u08_t iso_control)
{
//	u08_t write[13];
	u08_t pui8Write[2];

	if((iso_control & BIT5) == BIT5)
	{	
		return;
	}

	pui8Write[0] = TRF797x_ISO_CONTROL;
	pui8Write[1] = iso_control;
	pui8Write[1] &= ~BIT5;
	Trf797xWriteSingle(pui8Write, 2);

	iso_control &= 0x1F;

}

//===============================================================
// 02DEC2010	RP	Original Code
//===============================================================

void
Trf797xWriteInitiatorSetup(void)
{
	u08_t pui8Write[2];

	// Register 0x01
	pui8Write[0] = TRF797x_ISO_CONTROL;
	pui8Write[1] = 0x88;
	Trf797xWriteSingle(pui8Write, 2);

	// Register 0x02
	pui8Write[0] = TRF797x_ISO_14443_TX_OPTIONS;
	pui8Write[1] = 0x01;
	Trf797xWriteSingle(pui8Write, 2);

	// Register 0x08
	pui8Write[0] = TRF797x_RX_WAIT_TIME;
	pui8Write[1] = 0x07;
	Trf797xWriteSingle(pui8Write, 2);

	// Register 0x09 - System Clock Output, Modulation Scheme
	pui8Write[0] = TRF797x_MODULATOR_CONTROL;
	pui8Write[1] = 0x31; // Sys Clock Output = 6.78MHz, OOK = 100%
	Trf797xWriteSingle(pui8Write, 2);

	// Register 0x14 - Bandpass Filters
	pui8Write[0] = TRF797x_FIFO_IRQ_LEVELS;
	pui8Write[1] = 0x03;
	Trf797xWriteSingle(pui8Write, 2);

	// Register 0x0A - Bandpass Filters
	pui8Write[0] = TRF797x_RX_SPECIAL_SETTINGS;
	pui8Write[1] = 0x30;
	Trf797xWriteSingle(pui8Write, 2);

	// Register 0x18 - NFC Target Level
	pui8Write[0] = TRF797x_NFC_TARGET_LEVEL;
	pui8Write[1] = 0x00;
	Trf797xWriteSingle(pui8Write, 2);
}

//===============================================================
// NAME: void Trf797xWriteSingle (u08_t *pbuf, u08_t length)
//
// BRIEF: Is used to write to specified reader chip registers.
//
// INPUTS:
//	u08_t	*pbuf	addresses of the registers followed by the
//					contents to write
//	u08_t	length	number of registers * 2
//
// OUTPUTS:
//
// PROCESS:	[1] write to the registers
//
//===============================================================

void
Trf797xWriteSingle(u08_t *pbuf, u08_t length)
{
	SpiWriteSingle(pbuf, length);
}

//===============================================================
// NAME: Trf797xIrqWaitTimeout(u08_t txtimeout, u08_t rxtimeout)
//
// BRIEF: Is used to handle delays for transmit and receive
//
// INPUTS:
//	u08_t	txtimeout	amount of time to wait for TX complete
//	u08_t	rxtimeout	amount of time to wait for RX complete
//
//===============================================================

void Trf797xIrqWaitTimeout(u08_t txtimeout, u08_t rxtimeout)
{
	g_sTrf797xStatus = RX_WAIT;
	g_ui8TimeoutFlag = 0x00;
	while((g_sTrf797xStatus != TX_COMPLETE) && (g_sTrf797xStatus != NO_RESPONSE_RECEIVED))
	{										// Wait for end of TX
		// Clear the IRQ Flag
		g_ui8IrqFlag = 0x00;
		// Setup for the Timer
		McuCounterSet();
		// Calculate the timeout value for the timer
		COUNTER_VALUE = COUNT_1ms * txtimeout;
		// Start the Timeout
		START_COUNTER;
		while((g_ui8IrqFlag == 0x00) && (g_ui8TimeoutFlag == 0x00))	// Wait for an interrupt
		{
			// Do Nothing
		}
		RESET_COUNTER;
	}

	g_sTrf797xStatus = RX_WAIT;
	g_ui8TimeoutFlag = 0x00;
	while(g_sTrf797xStatus == RX_WAIT)		// Wait for end of RX or timeout
	{
		// Clear the IRQ Flag
		g_ui8IrqFlag = 0x00;
		// Setup for the Timer
		McuCounterSet();
		// Calculate the timeout value for the timer
		COUNTER_VALUE = COUNT_1ms * rxtimeout;
		// Start the Timeout
		START_COUNTER;
		while((g_ui8IrqFlag == 0x00) && (g_ui8TimeoutFlag == 0x00))	// Wait for an interrupt
		{
			// Do Nothing
		}
		RESET_COUNTER;
	}
}


//===============================================================
// NAME: Trf797xIrqWaitTimeout(u08_t txtimeout, u08_t rxtimeout)
//
// BRIEF: Is used to handle delays for transmit and receive
//
// INPUTS:
//	u08_t	txtimeout	amount of time to wait for TX complete
//	u08_t	rxtimeout	amount of time to wait for RX complete
//
//===============================================================

void Trf797x_timeout_ms(u16_t ui16Delay_ms)
{
	g_ui8TimeoutFlag = 0;

	McuCounterSet();
	// Calculate the timeout value for the timer
	COUNTER_VALUE = COUNT_1ms * ui16Delay_ms;
	// Start the Timeout
	START_COUNTER;
}


//===============================================================
//	Timer A0 Interrupt Vector
//===============================================================

#pragma vector=TIMER0_A0_VECTOR
__interrupt void
Msp430g2xx3TimerAHandler(void)
{
	u08_t ui8IrqStatus;
	ui8IrqStatus = 0x00;

	STOP_COUNTER;

	g_ui8TimeoutFlag = 0x01;

	if(!g_ui8DirectMode){
		Trf797xReadIrqStatus(&ui8IrqStatus);

		ui8IrqStatus = ui8IrqStatus & 0xF7;		// Set the parity flag to 0

		if(ui8IrqStatus == TRF797x_IRQ_STATUS_TX_COMPLETE)
		{
			g_sTrf797xStatus = TX_COMPLETE;
		}
		else if(ui8IrqStatus == TRF797x_IRQ_STATUS_IDLE)
		{
			g_sTrf797xStatus = NO_RESPONSE_RECEIVED;
		}
		else
		{
			g_sTrf797xStatus = RX_WAIT;
		}
	}
}



//===============================================================
// NAME: Trf797xIrqWaitTimeoutTxOnly(u08_t txtimeout)
//
// BRIEF: Is used to handle delays for transmit only
//		  This function will not delay to receive any responses
//
// INPUTS:
//	u08_t	txtimeout	amount of time to wait for TX complete
//
//===============================================================

void Trf797xIrqWaitTimeoutTxOnly(u08_t txtimeout)
{
	g_sTrf797xStatus = RX_WAIT;
	g_ui8TimeoutFlag = 0x00;
	while((g_sTrf797xStatus != TX_COMPLETE) && (g_sTrf797xStatus != NO_RESPONSE_RECEIVED))
	{										// Wait for end of TX
		// Clear the IRQ Flag
		g_ui8IrqFlag = 0x00;
		// Setup for the Timer
		McuCounterSet();
		// Calculate the timeout value for the timer
		COUNTER_VALUE = COUNT_1ms * txtimeout;
		// Start the Timeout
		START_COUNTER;
		while((g_ui8IrqFlag == 0x00) && (g_ui8TimeoutFlag == 0x00))	// Wait for an interrupt
		{
			// Do Nothing
		}
		RESET_COUNTER;
	}
}

//===============================================================
//	Returns current Trf Status
//===============================================================

tTRF797x_Status Trf797xGetTrfStatus(void)
{
	return g_sTrf797xStatus;
}

//===============================================================
//	Sets the current Trf Status
//===============================================================

void Trf797xSetTrfStatus(tTRF797x_Status sTrfStatus)
{
	g_sTrf797xStatus = sTrfStatus;
}

//===============================================================
//	Returns the RX Error Flag parameter
//===============================================================

bool Trf797xGetRxErrorFlag(void)
{
	return g_bRXErrorFlag;
}

//===============================================================
//	Sets the RX Error Flag parameter to a new value
//===============================================================

void Trf797xSetRxErrorFlag(bool bErrorFlag)
{
	g_bRXErrorFlag = bErrorFlag;
}

//===============================================================
//	Returns the current TX RX state value
//===============================================================

void Trf797xSetRxTxState(s08_t RxTxState)
{
	g_s08RxTxState = RxTxState;
}

//===============================================================
//	Sets the TX RX state value to a new value
//===============================================================

void Trf797xSetCollisionPosition(u08_t ui8ColPos)
{
	g_ui8CollisionPosition = ui8ColPos;
}

//===============================================================
//	Returns the current Anti-collision postion
//===============================================================

u08_t Trf797xGetCollisionPosition(void)
{
	return g_ui8CollisionPosition;
}

//===============================================================
//	Returns the Number of RX Bytes Received by the TRF FIFO
//
//	This can be used by the application layer to check for the
//	length of bytes received and ensure packets were correctly
//	received.
//===============================================================

u08_t Trf797xGetRxBytesReceived(void)
{
	return g_ui8FifoRxLength;
}
