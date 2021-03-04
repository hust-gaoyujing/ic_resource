/*
 * File Name: iso14443a.c
 *
 * Description: Contain functions to search for ISO14443A tags and
 * execute ISO14443A Anticollision
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

/****************************************************************
 * FILENAME: iso14443a.c
 *
 * BRIEF: Contain functions to search ISO14443A standardized tags
 * in stand alone mode and execute ISO14443A Anticollision in
 * remote mode.
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 *
 * AUTHOR(S): Reiser Peter		DATE: 02 DEC 	2010
 *
 * EDITED BY: Jace Hall			DATE: 02 April 	2012
 *
 * Changes Made: 				Date: 13 July	2012
 *	UID check complete, Layer 3 Complee, Switch optimization, comments added
 *	Layer4 function added, MiFareDESFire function added, Authentication function added
 *
 ****************************************************************/

#include "iso14443a.h"

//===============================================================
//		Global Variables
//===============================================================

u08_t buf[128];

u08_t g_ui8UidPos = 0;
u08_t g_pui8CompleteUid[14];

static volatile tTRF797x_Status g_sTrfStatus;

//static u08_t g_pui8AppId[3] = {0x00, 0x00, 0x00};		// Only needed when reading data from DESFire EV1 tags

//===============================================================

//===============================================================
// NAME: void Iso14443aAnticollision(u08_t ui8Command)
//
// BRIEF: Is used to start the ISO14443A Anticollision Loop.
//
// INPUTS:
//	Parameters:
//		u08_t		ui8Command		REQA or WUPA
//
// OUTPUTS:
//
// PROCESS:	[1] send REQA or WUPA command
//			[2] receive ATQA
//			[3] perform bit frame anticollison loop
//
//===============================================================

u08_t Iso14443aAnticollision(u08_t ui8Command)
{
	u08_t ui8Index, ui8Return;

	ui8Index = 0;

    IRQ_CLR;			// PORT2 interrupt flag clear

	Trf797xSetRxTxState(1);

	// Poll for a ISO14443A tag
	Iso14443aPollingCommand(ui8Command);

	g_sTrfStatus = Trf797xGetTrfStatus();

	// Wait for a reply
	if(g_sTrfStatus == RX_COMPLETE || g_sTrfStatus == PROTOCOL_ERROR)
	{
		for(ui8Index = 0; ui8Index < 14; ui8Index++)
		{
			g_pui8CompleteUid[ui8Index] = 0x00;
		}
		g_ui8UidPos = 0;
		Iso14443aLoop(0x01, NVB_INIT, &g_pui8CompleteUid[0]);

		LEDtypeAON;			//Type A tag found.
		ui8Return = STATUS_SUCCESS;
	}
	else
	{
		// No card detected
		LEDtypeAOFF;
		LEDAuth1OFF;
		LEDAuth2OFF;
		LEDAuth3OFF;
		LED_MFC_READ_BLOCK_OFF;
		ui8Return = STATUS_FAIL;
	}

	return ui8Return;
}

//===============================================================
// NAME: Iso14443aLoop(u08_t ui8CascadeLevel, u08_t ui8NVB, u08_t
//		*pui8UID)
//
// BRIEF: Is used to run through the cascade levels.
//
// INPUTS:
//	Parameters:
//		u08_t		ui8CascadeLevel		Indicates cascade level
//		u08_t		ui8NVB				Number of valid bits
//		u08_t		*pui8UID			Known part of UID
//	
// OUTPUTS:
//	Globals:
//		u08_t		g_pui8CompleteUid[14]	stores UID
//
// PROCESS:	(ISO14443-3)
//
//===============================================================

void
Iso14443aLoop(u08_t ui8CascadeLevel, u08_t ui8NVB, u08_t * pui8UID)
{
	u08_t	ui8Index = 0;
	u08_t	ui8NVBytes = 0;
	u08_t	ui8NVbits = 0;
	u08_t	ui8Xbits = 0;
	u08_t 	ui8SelectLevel;
	bool	bRXErrorFlag;
	u08_t	ui8CollisionPosition = 0;
	u08_t pui8Write[2];

	ui8CollisionPosition = 0x21;
	Trf797xSetCollisionPosition(ui8CollisionPosition);

	Trf797xSetRxErrorFlag(false);

	while (ui8CascadeLevel < 4)
	{
		switch (ui8CascadeLevel)
		{
		case 1:
			ui8SelectLevel = SEL_CASCADE1;
			break;
		case 2:
			ui8SelectLevel = SEL_CASCADE2;
			break;
		case 3:
			ui8SelectLevel = SEL_CASCADE3;
			break;
		default:
			break;
		}

		if((ui8NVB & 0x0F) != 0x00)
		{
			ui8NVBytes = (ui8NVB >> 4) - 2;	// the number of known valid bytes
			ui8Xbits = ui8NVB & 0x07;			// the number of known valid bits

			// Both are used in the UID calculation
			for(ui8Index = 0; ui8Index < ui8Xbits; ui8Index++)
			{
				ui8NVbits = (ui8NVbits << 1) + 1;
			}
		}

		Trf797xSetRxTxState(1);

		pui8Write[0] = TRF797x_ISO_CONTROL;
		pui8Write[1] = 0x88;
		Trf797xWriteSingle(pui8Write, 2);

		Iso14443aSelectCommand(ui8SelectLevel, ui8NVB, pui8UID);

		g_sTrfStatus = Trf797xGetTrfStatus();

		ui8CollisionPosition = Trf797xGetCollisionPosition();

		while (ui8CollisionPosition < 0x20)
		{
			if(g_sTrfStatus == TX_COMPLETE || g_sTrfStatus == NO_RESPONSE_RECEIVED || g_sTrfStatus == PROTOCOL_ERROR)
			{
				break;
			}
		};

		if (ui8CollisionPosition == 0x20)
		{
			g_sTrfStatus = PROTOCOL_ERROR;
			Trf797xSetTrfStatus(g_sTrfStatus);	// In case coll_poss=0x20 means didn't receive response
		}

		for(ui8Index = 0; ui8Index < 5; ui8Index++)
		{
			g_pui8CompleteUid[ui8Index+g_ui8UidPos] = buf[ui8Index + 1];
		}

		bRXErrorFlag = Trf797xGetRxErrorFlag();

		if(bRXErrorFlag == true)
		{
			g_sTrfStatus = PROTOCOL_ERROR;
			Trf797xSetTrfStatus(g_sTrfStatus);
		}

		if(g_sTrfStatus == PROTOCOL_ERROR || g_sTrfStatus == TX_COMPLETE || g_sTrfStatus == NO_RESPONSE_RECEIVED)	// collision or timeout
		{
			break;
		}

		if(g_sTrfStatus == RX_COMPLETE)					// if data received
		{
			for (ui8Index=0; ui8Index<ui8NVBytes; ui8Index++)
			{
				g_pui8CompleteUid[ui8Index+g_ui8UidPos] = *(pui8UID + ui8Index);
			}

			g_pui8CompleteUid[ui8NVBytes+g_ui8UidPos] = (buf[1] &~ui8NVbits) | (*(pui8UID + ui8NVBytes) & ui8NVbits);

			for (ui8Index=1; ui8Index<(5-ui8NVBytes); ui8Index++)
			{
				g_pui8CompleteUid[ui8Index+ui8NVBytes+g_ui8UidPos] = buf[1+ui8Index];
			}

			ui8NVB = NVB_FULL;
			Trf797xSetRxTxState(1);

			pui8Write[0] = TRF797x_ISO_CONTROL;
			pui8Write[1] = 0x08;
			Trf797xWriteSingle(pui8Write, 2);

			Iso14443aSelectCommand(ui8SelectLevel, ui8NVB, &g_pui8CompleteUid[g_ui8UidPos]);

			McuDelayMillisecond(6);

			if (buf[1] & BIT2)				// UID not complete
			{
				ui8CascadeLevel++;
				g_ui8UidPos += 5;
				ui8NVB = NVB_INIT;
			}
			else							// UID completed
			{
				g_sTrfStatus = RX_WAIT;
				Trf797xSetTrfStatus(g_sTrfStatus);
				break;
			}
		}
	}

	if(g_sTrfStatus == PROTOCOL_ERROR)									// if collision occurred go into anti-collision
	{
		u08_t pui8NewUid[4];
		u08_t pui8NewUid1[4];
		u08_t ui8TempCollisionPosition;
		u08_t ui8TempNVbits;
		u08_t ui8TempCascadeLevel;
		u08_t ui8TempUidPos;

		for (ui8Index=0; ui8Index<4; ui8Index++)
		{
			pui8NewUid[ui8Index] = buf[ui8Index+1];
		}
		// a RX interrupt will happen after collision interrupt
		McuDelayMillisecond(5);

		ui8CollisionPosition = Trf797xGetCollisionPosition();

		if (ui8CollisionPosition == 0x60 || ui8CollisionPosition == 0x70)		// if all of 4 or 5 bytes of last level were in collision
		{
			ui8CascadeLevel++;
			g_ui8UidPos += 5;
			ui8CollisionPosition = 0x20;
		}
		else
		{
			// combine UCLn collected of last loop
			for (ui8Index=0; ui8Index<ui8NVBytes; ui8Index++)
			{
				pui8NewUid[ui8Index] = *(pui8UID + ui8Index);
			}
			pui8NewUid[ui8NVBytes] = (pui8NewUid[ui8NVBytes] &~ ui8NVbits) | (*(pui8UID + ui8NVBytes) & ui8NVbits);
		}
		// calculate new parameters
		ui8NVBytes = (ui8CollisionPosition >> 4) - 2;				// how many bytes were collected
		ui8Xbits  = ui8CollisionPosition & 0x07;					// how many bits were collected
		ui8CollisionPosition++;
		ui8NVbits = 0;
		for (ui8Index = 0; ui8Index < ui8Xbits; ui8Index++)
		{
			ui8NVbits = (ui8NVbits << 1) + 1;				// left shift to make a mask for last broken byte (create all bit 1 belong to how many bit in broken byte)
		}
		ui8TempNVbits = (ui8NVbits << 1) + 1;
		ui8TempNVbits = ui8TempNVbits - ui8NVbits;					// bit_mask1 use to separate next bit after last broken bit

		pui8NewUid[ui8NVBytes] = pui8NewUid[ui8NVBytes] & ui8NVbits;			// only keep collision bits

		//back up before loop
		for (ui8Index=0; ui8Index<=4; ui8Index++)
		{
			pui8NewUid1[ui8Index] = pui8NewUid[ui8Index];
		}
		ui8TempCollisionPosition = ui8CollisionPosition;
		ui8TempUidPos = g_ui8UidPos;
		ui8TempCascadeLevel = ui8CascadeLevel;

		Iso14443aLoop(ui8CascadeLevel, ui8CollisionPosition, pui8NewUid);		// recursive call for anti-collision procedure
		McuDelayMillisecond(6);

		Iso14443aHalt();

		g_sTrfStatus = RX_WAIT;
		Trf797xSetTrfStatus(g_sTrfStatus);
		Iso14443aPollingCommand(WUPA);
		McuDelayMillisecond(6);

		g_ui8UidPos = ui8TempUidPos;
		pui8NewUid1[ui8NVBytes] = pui8NewUid1[ui8NVBytes] + ui8TempNVbits;
		Iso14443aLoop(ui8TempCascadeLevel, ui8TempCollisionPosition, pui8NewUid1);	// recursive call for anti-collision procedure
	}
}

//===============================================================
// NAME: void Iso14443aAnticollision(u08_t ui8Select, u08_t ui8NVB, u08_t * pui8UID)
//
// BRIEF: Issues an ISO14443 select command and waits for the reply
//
// INPUTS:
//	Parameters:
//		u08_t		ui8Select		Select Command
//		u08_t		ui8NVB			Number of valid bits
//		u08_t		*pui8AESKey		Pointer to array containing the UID
//
// OUTPUTS:	None
//
// PROCESS:	[1] Send Select command packet based on the received values of
//					1) The Select Command
//					2) The value of NVB
//			[2] Receive the reply from the tag
//
//===============================================================

void Iso14443aSelectCommand(u08_t ui8Select, u08_t ui8NVB, u08_t * pui8UID)
{
	buf[0] = 0x8F;
	if(ui8NVB == 0x70)				// Determine whether or not CRC is transmitted
	{
		buf[1] = 0x91;				// Transmit with CRC
	}
	else
	{
		buf[1] = 0x90;				// Transmit without CRC
	}
	buf[2] = 0x3D;
	buf[3] = 0x00;
	buf[4] = ui8NVB & 0xF0;			// Number of complete bytes
	if((ui8NVB & 0x07) != 0x00)
	{
		buf[4] |= ((ui8NVB & 0x07) << 1) + 1; 	// number of broken bits, last bit is 1 means broken byte
	}
	buf[5] = ui8Select;				// Select Command; can be 0x93, 0x95 or 0x97
	buf[6] = ui8NVB;				// Number of valid bits
	buf[7] = *pui8UID;				// UID Bytes
	buf[8] = *(pui8UID + 1);
	buf[9] = *(pui8UID + 2);
	buf[10] = *(pui8UID + 3);
	buf[11] = *(pui8UID + 4);

	Trf797xRawWrite(&buf[0], 5+((ui8NVB & 0xF0)>>4) );

    Trf797xIrqWaitTimeout(3,10);
}

//===============================================================
// NAME: void Iso14443aPollingCommand(u08_t ui8Command)
//
// BRIEF: Issues an ISO14443 polling command and waits for a reply
//
// INPUTS:
//	Parameters:
//		u08_t		ui8Command		Either REQA or WUPA
//
// OUTPUTS:	None
//
// PROCESS:	[1] Send a command based on the inputted command code
//			[2] Receive the reply from the tag
//
//===============================================================

void Iso14443aPollingCommand(u08_t ui8Command)
{
    buf[0] = 0x8F;
    buf[1] = 0x90;
    buf[2] = 0x3D;
    buf[3] = 0x00;
    buf[4] = 0x0F;
	buf[5] = ui8Command;

    Trf797xRawWrite(&buf[0], 6);

	Trf797xIrqWaitTimeout(3,10);
}

//===============================================================
// NAME: void Iso14443aHalt(void)
//
// BRIEF: Issues an ISO14443 halt command
//
// INPUTS:	None
//
// OUTPUTS:	None
//
// PROCESS:	[1] Send a Halt Command
//
//===============================================================

void Iso14443aHalt(void)
{
	buf[0] = 0x8F;
	buf[1] = 0x90;
	buf[2] = 0x3D;
	buf[3] = 0x00;
	buf[4] = 0x20;
	buf[5] = 0x50;		// Halt Command
	buf[6] = 0x00;

	Trf797xRawWrite(&buf[0], 7);

	Trf797xIrqWaitTimeoutTxOnly(3);		// 3 ms Wait Timeout for TX complete
	// Do not wait for RX as tags do not reply to Halt command
}


//===================================================================
// NAME: u08_t ISO14443aLayer4(void)
//
// BRIEF: Is used to execute ISO14443A-4 Protocol in reference to
//		  identifying MIFARE DESFire card. The ATS response is specific
//		  to MIFARE DESFire EV1
//
// INPUTS:		none
//
// OUTPUTS:
//	Parameters:
//		u08_t	ui8Status		Returns the following:
//								"STATUS_FAIL" => Error has occurred
// 								"STATUS_SUCCESS" => Layer 4 successful
//
// Process:		[1] Send RATS Command
//				[2] Recieve ATS
//				[3] Send PPS
//				[4] Receive response
//				[5] Return if a success or not
//
//===================================================================

u08_t Iso14443aLayer4(void)
{
	u08_t ui8Status;
	u08_t pui8WriteBuffer[2];

	ui8Status = STATUS_FAIL;

	// Buffer setup for FIFO writing
	buf[0] = 0x8F;
	buf[1] = 0x91;
	buf[2] = 0x3D;
	buf[3] = 0x00;
	buf[4] = 0x20;

	//RATS Command
	buf[5] = RATS_CMD;
	buf[6] = RATS_PARAM;

	Trf797xRawWrite(&buf[0], 7);

	Trf797xSetRxTxState(1);

	Trf797xIrqWaitTimeout(3,10);				// 3 millisecond TX timeout, 10 millisecond RX timeout

	g_sTrfStatus = Trf797xGetTrfStatus();

	// If data received
	if(g_sTrfStatus == RX_COMPLETE)
	{

		// Check for expected Mifare response
		if((buf[1] == ATS_Length) && (buf[2] == ATS_T0) && (buf[3] == ATS_TA1) && (buf[4] == ATS_TB1) && (buf[5] == ATS_TC1))
		{

			// If condition met then ATS is correct. Must now send PPS Request
			buf[0] = 0x8F;
			buf[1] = 0x91;
			buf[2] = 0x3D;
			buf[3] = 0x00;
			buf[4] = 0x30;
			buf[5] = PPSS;
			buf[6] = PPS0;
			buf[7] = PPS1_106;	// PPS1 is current set to the default rate of 106kbps
								// It is recommended to keep the data rate low to get better transmission ranges

			Trf797xRawWrite(&buf[0], 8);

			Trf797xSetRxTxState(1);

			Trf797xIrqWaitTimeout(3,10);		// 3 millisecond TX timeout, 10 millisecond RX timeout

			// Check PPS response
			if (buf[1] == PPSS)
			{
				ui8Status = STATUS_SUCCESS;

				// Bitrate Change
				pui8WriteBuffer[0] = TRF797x_ISO_CONTROL;
				pui8WriteBuffer[1] = 0x08;	// Use 0x08 for 106kbps - PPS1_106
									// Use 0x09 for 212kbps - PPS1_212
									// Use 0x0A for 424kbps - PPS1_424
									// Use 0x0B for 848kbps - PPS1_848
				Trf797xWriteSingle(pui8WriteBuffer, 2);

			}
		}
		else
		{
			ui8Status = STATUS_FAIL;
		}
	}
	else
	{
		ui8Status = STATUS_FAIL;
	}

	return ui8Status;
}

//===========================================================================
//
// Commented code for reading MIFARE DESFire EV1 Tags.
// Uncommenting will increase code size by approximately 1400 bytes of Flash
//
//===========================================================================

////===================================================================
//// NAME: u08_t Iso14443aDFEV1ReadData(u08_t * pui8Offset,
////							u08_t ui8Length, u08_t ui8IpduBlockNumber)
////
//// BRIEF:	Function to read data from a MF DESFire EV1 tag after
////			authentication is complete and a file ID is determined
////
//// INPUTS:
////	Parameters:
////		u08_t	pui8Offset			Address of the first byte to read out
////		u08_t	ui8Length			Number of bytes to be read out
////		u08_t	ui8IpduBlockNumber	Designate the current IPDU block number
////
////	Parameters:
////		u08_t	ui8Status		Returns the following:
////								"STATUS_FAIL" => Error has occurred
//// 								"STATUS_SUCCESS" => Read Data successful
////
//// Process:		[1] Send Read Data command
////				[2] Check response for errors, if no errors then:
////					[2a] Output to data to Uart (if enabled)
////					[2b] Check if all data has been read, if not go back to step [1]
////				[3] If communication error occurs, stop reading and returning
////				[4] If all data is read out, then return
////
////===================================================================
//
//u08_t Iso14443aDFEV1ReadData(u08_t ui8FileNumber, u08_t * pui8Offset, u08_t ui8Length, u08_t ui8IpduBlockNumber)
//{
//	u08_t ui8Status = STATUS_FAIL;
//	u08_t ui8BufLength = 0;
//	u08_t ui8RxLength = 0;
//	u08_t ui8TempLength = 0;
//	u08_t pui8ReadOffset[2];
//	u16_t ui16ReadOffset;
//
//	pui8ReadOffset[0] = pui8Offset[0];
//	pui8ReadOffset[1] = pui8Offset[1];
//
//	ui8RxLength = ui8Length;
//
//	while (ui8RxLength > 0)
//	{
//		if (ui8RxLength > 0x3B)
//		{
//			ui8TempLength = 0x3B;
//		}
//		else
//		{
//			ui8TempLength = ui8RxLength;
//		}
//
//		buf[ui8BufLength++] = 0x8F;
//		buf[ui8BufLength++] = 0x91;
//		buf[ui8BufLength++] = 0x3D;
//		buf[ui8BufLength++] = 0x00;
//		buf[ui8BufLength++] = 0xA0;
//
//		buf[ui8BufLength++] = 0x0A | ui8IpduBlockNumber; 	// I-block (Layer 4)
//		buf[ui8BufLength++] = CID;							// CID
//		buf[ui8BufLength++] = ReadData();
//		buf[ui8BufLength++] = ui8FileNumber;				// File number to be read from
//		buf[ui8BufLength++] = pui8ReadOffset[0];			// Offset for the read command
//		buf[ui8BufLength++] = pui8ReadOffset[1];
//		buf[ui8BufLength++] = 0x00;							// Not using the last byte of the Offset since
//															// no DESFire tag exceeds 0xFFFF memory locations.
//		buf[ui8BufLength++] = ui8TempLength;
//		buf[ui8BufLength++] = 0x00;
//		buf[ui8BufLength++] = 0x00;
//
//#ifdef ENABLE_UART
//		UartSendCString("Reading 0x");
//		UartPutByte(ui8TempLength);
//		UartSendCString(" Bytes of Data from File ID = ");
//		UartPutByte(ui8FileNumber);
//		UartPutNewLine();
//#endif
//
//		Trf797xRawWrite(&buf[0], ui8BufLength);
//		ui8BufLength = 0;
//
//		ui8RxLength = ui8RxLength - ui8TempLength;
//		ui8IpduBlockNumber = ui8IpduBlockNumber ^ 0x01;
//		if (pui8ReadOffset[0] > (0xFF-ui8TempLength))
//		{
//			ui16ReadOffset = (u16_t) ((pui8ReadOffset[1] << 8) | (pui8ReadOffset[0] & 0xFF));
//			ui16ReadOffset = ui16ReadOffset + ui8TempLength;
//			pui8ReadOffset[0] = (u08_t) (ui16ReadOffset & 0xFF);
//			pui8ReadOffset[1] = (u08_t) (ui16ReadOffset >> 8);
//		}
//		else
//		{
//			pui8ReadOffset[0] = pui8ReadOffset[0] + ui8TempLength;
//		}
//
//		Trf797xSetRxTxState(1);
//
//		Trf797xIrqWaitTimeout(5,30);			// 5 millisecond TX timeout, 30 millisecond RX timeout
//
//		McuDelayMillisecond(1);					// Small delay after RX complete
//
//		g_sTrfStatus = Trf797xGetTrfStatus();
//
//		if(g_sTrfStatus == RX_COMPLETE)
//		{
//			if (buf[3] == StatusCode2())
//			{
//				ui8Status = STATUS_SUCCESS;
//
//#ifdef ENABLE_UART
//				UartSendCString("Data Received: ");
//				UartPutBuffer(&buf[4],ui8TempLength);
//				UartPutNewLine();
//				UartPutNewLine();
//#endif
//			}
//			else
//			{
//#ifdef ENABLE_UART
//				UartSendCString("Data Not Received, Error Occurred: ");
//				UartPutByte(buf[3]);
//				UartPutNewLine();
//				UartPutNewLine();
//#endif
//			}
//			McuDelayMillisecond(1);					// Small delay after RX complete
//		}
//		else
//		{
//			// Communication error occurred, break out of while loop
//			ui8RxLength = 0;
//#ifdef ENABLE_UART
//			UartSendCString("Communication Error Occurred!");
//			UartPutNewLine();
//			UartPutNewLine();
//#endif
//		}
//	}
//	return ui8Status;
//}
//
////===================================================================
//// NAME: u08_t Iso14443aDFEV1GetAppId(u08_t ui8IpduBlockNumber)
////
//// BRIEF:	Function to fetch the Application ID's available on a MF
////			DESFire EV1 tag. Ideally used after authentication is complete.
////
//// INPUTS:
////	Parameters:
////		u08_t	ui8IpduBlockNumber	Designate the current IPDU block number
////
////	Parameters:
////		u08_t	ui8Status		Returns the following:
////								"STATUS_FAIL" => Error has occurred
//// 								"STATUS_SUCCESS" => Get App Id was successful
////
//// Process:		[1] Send Get App Id command
////				[2] Check response for errors, if no errors then:
////					[2a] Store the application ID to the global array
////					[2b] Output the app ID to Uart (if enabled)
////				[3] Return
////
////===================================================================
//
//u08_t Iso14443aDFEV1GetAppId(u08_t ui8IpduBlockNumber)
//{
//	u08_t ui8Status = STATUS_FAIL;
//	u08_t ui8BufLength = 0;
//	u08_t ui8RxLength = 0;
//
//	buf[ui8BufLength++] = 0x8F;
//	buf[ui8BufLength++] = 0x91;
//	buf[ui8BufLength++] = 0x3D;
//	buf[ui8BufLength++] = 0x00;
//	buf[ui8BufLength++] = 0x30;
//
//	buf[ui8BufLength++] = 0x0A | ui8IpduBlockNumber; 	// I-block (Layer 4)
//	buf[ui8BufLength++] = CID;							// CID
//	buf[ui8BufLength++] = GetAppId();
//
//	Trf797xRawWrite(&buf[0], ui8BufLength);
//	ui8BufLength = 0;
//
//	Trf797xSetRxTxState(1);
//
//	Trf797xIrqWaitTimeout(5,15);			// 5 millisecond TX timeout, 15 millisecond RX timeout
//
//	g_sTrfStatus = Trf797xGetTrfStatus();
//
//	// If data received
//	if(g_sTrfStatus == RX_COMPLETE)
//	{
//		if (buf[3] == StatusCode2())
//		{
//			ui8Status = STATUS_SUCCESS;
//
//			ui8RxLength = Trf797xGetRxBytesReceived();
//
//			if (ui8RxLength > 11)
//			{
//				g_pui8AppId[0] = buf[4];			// Fetch the Application ID
//				g_pui8AppId[1] = buf[5];
//				g_pui8AppId[2] = buf[6];
//#ifdef ENABLE_UART
//			UartSendCString("Available Application ID is: ");
//			UartPutByte(g_pui8AppId[2]);
//			UartPutByte(g_pui8AppId[1]);
//			UartPutByte(g_pui8AppId[0]);
//			UartPutNewLine();
//#endif
//			}
//			else
//			{
//#ifdef ENABLE_UART
//				UartSendCString("No Application Found on Tag");
//				UartPutNewLine();
//#endif
//			}
//		}
//		else
//		{
//#ifdef ENABLE_UART
//			UartSendCString("Application ID Not Received, Error Occurred: ");
//			UartPutByte(buf[3]);
//			UartPutNewLine();
//			UartPutNewLine();
//#endif
//		}
//		McuDelayMillisecond(1);					// Small delay after RX complete
//	}
//	else
//	{
//#ifdef ENABLE_UART
//		UartSendCString("Communication Error Occurred!");
//		UartPutNewLine();
//		UartPutNewLine();
//#endif
//	}
//	return ui8Status;
//}
//
////===================================================================
//// NAME: u08_t Iso14443aDFEV1SelectApp(u08_t ui8IpduBlockNumber)
////
//// BRIEF:	Function to select an Application based on the application
//// 			ID's available on a MF DESFire EV1 tag.
////			Can only be used after Iso14443aDFEV1GetAppId is called.
////			Ideally should be used after authentication is complete.
////
//// INPUTS:
////	Parameters:
////		u08_t	ui8IpduBlockNumber	Designate the current IPDU block number
////
////	Parameters:
////		u08_t	ui8Status		Returns the following:
////								"STATUS_FAIL" => Error has occurred
//// 								"STATUS_SUCCESS" => Select Application was successful
////
//// Process:		[1] Send Select Application Id command with Application based on
////					the global variable g_pui8AppId
////				[2] Check response for errors
////				[3] Return
////
////===================================================================
//
//u08_t Iso14443aDFEV1SelectApp(u08_t ui8IpduBlockNumber)
//{
//	u08_t ui8Status = STATUS_FAIL;
//	u08_t ui8BufLength = 0;
//
//	buf[ui8BufLength++] = 0x8F;
//	buf[ui8BufLength++] = 0x91;
//	buf[ui8BufLength++] = 0x3D;
//	buf[ui8BufLength++] = 0x00;
//	buf[ui8BufLength++] = 0x60;
//
//	buf[ui8BufLength++] = 0x0A | ui8IpduBlockNumber; 	// I-block (Layer 4)
//	buf[ui8BufLength++] = CID;							// CID
//	buf[ui8BufLength++] = SelectApp();
//	buf[ui8BufLength++] = g_pui8AppId[0];				// Use App Id received from Iso14443aDFEV1GetAppId
//	buf[ui8BufLength++] = g_pui8AppId[1];
//	buf[ui8BufLength++] = g_pui8AppId[2];
//
//	Trf797xRawWrite(&buf[0], ui8BufLength);
//	ui8BufLength = 0;
//
//	Trf797xSetRxTxState(1);
//
//	Trf797xIrqWaitTimeout(5,15);			// 5 millisecond TX timeout, 15 millisecond RX timeout
//
//	McuDelayMillisecond(1);					// Small delay after RX complete
//
//	g_sTrfStatus = Trf797xGetTrfStatus();
//
//	// If data received
//	if(g_sTrfStatus == RX_COMPLETE)
//	{
//		if (buf[3] == StatusCode2())
//		{
//			ui8Status = STATUS_SUCCESS;
//		}
//	}
//
//	return ui8Status;
//}
//
////===================================================================
//// NAME: u08_t Iso14443aDFEV1GetFileId(u08_t ui8IpduBlockNumber)
////
//// BRIEF:	Function to fetch the file ID's from the selected application
////			of a MF DESFire EV1 tag. Ideally used after authentication
////			has been completed.
////
//// INPUTS:
////	Parameters:
////		u08_t	ui8IpduBlockNumber	Designate the current IPDU block number
////
////	Parameters:
////		u08_t	ui8Status		Returns the following:
////								"STATUS_FAIL" => Error has occurred
//// 								"STATUS_SUCCESS" => Get File Ids was successful
////
//// Process:		[1] Send Get File Id command
////				[2] Check response for errors, if no errors then:
////					[2a] Output the app ID to Uart (if enabled)
////				[3] Return
////
////===================================================================
//
//
//u08_t Iso14443aDFEV1GetFileId(u08_t ui8IpduBlockNumber)
//{
//	u08_t ui8Status = STATUS_FAIL;
//	u08_t ui8BufLength = 0;
//#ifdef ENABLE_UART
//	u08_t ui8RxLength = 0;
//	u08_t ui8index;
//#endif
//
//	buf[ui8BufLength++] = 0x8F;
//	buf[ui8BufLength++] = 0x91;
//	buf[ui8BufLength++] = 0x3D;
//	buf[ui8BufLength++] = 0x00;
//	buf[ui8BufLength++] = 0x30;
//
//	buf[ui8BufLength++] = 0x0A | ui8IpduBlockNumber; 	// I-block (Layer 4)
//	buf[ui8BufLength++] = CID;							// CID
//	buf[ui8BufLength++] = GetFileId();
//
//	Trf797xRawWrite(&buf[0], ui8BufLength);
//	ui8BufLength = 0;
//
//	Trf797xSetRxTxState(1);
//
//	Trf797xIrqWaitTimeout(5,15);			// 5 millisecond TX timeout, 15 millisecond RX timeout
//
//	g_sTrfStatus = Trf797xGetTrfStatus();
//
//	// If data received
//	if(g_sTrfStatus == RX_COMPLETE)
//	{
//		if (buf[3] == StatusCode2())
//		{
//			ui8Status = STATUS_SUCCESS;
//
//#ifdef ENABLE_UART
//			ui8RxLength = Trf797xGetRxBytesReceived() - 3;
//
//			UartSendCString("Available File ID's are: ");
//			for (ui8index = ui8RxLength; ui8index > 0; ui8index--)
//			{
//				UartPutByte(buf[3+ui8index]);
//				UartPutSpace();
//			}
//			UartPutNewLine();
//			UartPutNewLine();
//#endif
//		}
//		else
//		{
//#ifdef ENABLE_UART
//			UartSendCString("File ID Not Received, Error Occurred: ");
//			UartPutByte(buf[3]);
//			UartPutNewLine();
//			UartPutNewLine();
//#endif
//		}
//		McuDelayMillisecond(1);					// Small delay after RX complete
//	}
//	else
//	{
//#ifdef ENABLE_UART
//		UartSendCString("Communication Error Occurred!");
//		UartPutNewLine();
//		UartPutNewLine();
//#endif
//	}
//	return ui8Status;
//}
