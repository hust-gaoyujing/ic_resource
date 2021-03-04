/*
 * File Name: nfc.c
 *
 * Description: Contains functions to handle processing the NFC/RFID
 * stack.
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

//===============================================================

#include "nfc.h"
//#include "RFid_functions.h"
//#include "MFDESFireAESAuth.h"

//===============================================================
//		Global Variables
//===============================================================

//static u08_t pui8SessionKey[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// This is the Random Number A (RndA) that will be used during the AES authentication sequence.
// It is hard coded in this example, however a Random Number Generator (RNG) could be used to generate the number instead.
//static u08_t pui8RndA[16] = {0x79, 0xd4, 0x66, 0x29,0xf7,0xe1,0x12,0xc3,0x79, 0xd4, 0x66, 0x29,0xf7,0xe1,0x12,0xc3};

// This is the key that will be used for the MFC authentication process. Modify this key with the Nfc_setMFCKey() function.
static u08_t g_pui8MFCkey[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// This is the Block that will be authenticated. Modify the block with the Nfc_setMFCBlock() function.
static u08_t pui8MFCblock = 0;
unsigned char pui8BlockData[16];
//===============================================================

//===============================================================
// NAME: void Nfc_runAesAuth(void)
//
// BRIEF: Runs through the NFC stack for DESFire EV1
//
// INPUTS: None
//
// OUTPUTS: None
//
// PROCESS:	[1] Turn RF field on
//			[2] Set up TRF for initiator
//			[3] Call Anticollision function for tag detection
//			[4] Call Layer 4 commands
//			[5] If a tag is activated, go through AES Auth process
//
//===============================================================

void
Nfc_runMFCAuth(void)
{
	Trf797xTurnRfOn();				// Enable RF Field for TRF7970

	Trf797xWriteInitiatorSetup();	// Send initial command to configure Type 4A Reader/Writer

	IRQ_ON;							// Enable interrupts on I/O pin for the IRQ

	// When a PICC is exposed to an unmodulated operating field
	// it shall be able to accept a quest within 5 ms.
	// PCDs should periodically present an unmodulated field of at least
	// 5.1 ms duration. (ISO14443-3)
	McuDelayMillisecond(6);

	if(Iso14443aAnticollision(REQA) == STATUS_SUCCESS) 	// Do a complete anticollision sequence as described
	{													// in ISO14443-3 standard for type A

		if(RFID_AuthenticateA(g_pui8MFCkey, pui8MFCblock, g_pui8CompleteUid, g_ui8UidPos) == STATUS_SUCCESS)
		{												// Do MFC Authentication
#ifdef ENABLE_UART
			UartSendCString("==========================================");
			UartPutNewLine();
			UartSendCString(" MIFARE Classic Authentication Successful ");
			UartPutNewLine();
			UartSendCString("==========================================");
			UartPutNewLine();
			UartPutNewLine();
#endif

			//===================================================================================
			// This section is an example of a user specific application that will allow for the
			// contents of an MFC Tag to be read out after Authentication
			//===================================================================================
			RFID_ReadBlock(pui8MFCblock, pui8BlockData);
		}
	}

	IRQ_OFF;
	Trf797xTurnRfOff();			// Turns off the RF Field
	Trf797xResetIrqStatus();	// Reads IRQ to ensure it has been cleared
}

//===============================================================
// NAME: void Nfc_setAesKey(const u08_t * pui8AESKey, u08_t ui8AESKeyLength)
//
// BRIEF: Used to change the AES key used for Authentication
//
// INPUTS:
//	Parameters:
//		u08_t		ui8AESKeyLength		Length of the AES key
//		u08_t		*pui8AESKey			Pointer to array containing AES key
//
// OUTPUTS: None
//
// PROCESS:	[1] Check if MFC key length is equal to 6
//			[2] Copy Key array
//
//===============================================================


void Nfc_setMFCKey(const u08_t * pui8MFCKey, u08_t ui8MFCKeyLength)
{
	u08_t i = 0;

	if (ui8MFCKeyLength == 6)
	{
		for (i = 0; i < ui8MFCKeyLength; i++)
		{
			g_pui8MFCkey[i] = pui8MFCKey[i];
		}
	}
	else
	{
		// Wrong Length
	}
}

//===============================================================
// NAME: void Nfc_setMFCBlock(const u08_t * pui8MFCBlock)
//
// BRIEF: Used to select the MFC block to be Authentication
//
// INPUTS:
//	Parameters:
//		u08_t		pui8MFCBlock		Pointer to new Block number
//
// OUTPUTS: None
//
// PROCESS:	[1] Copy Block number
//
//===============================================================


void Nfc_setMFCBlock(const u08_t pui8MFCBlock)
{
	pui8MFCblock = pui8MFCBlock;
}
