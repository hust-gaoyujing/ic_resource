/*
 * File Name: main.c
 *
 * Description: The TRF797x is an integrated analog front end and
 * data framing system for a 13.56 MHz RFID reader system.
 * Built-in programming options make it suitable for a wide range
 * of applications both in proximity and vicinity RFID systems.
 * The reader is configured by selecting the desired protocol in
 * the control registers. Direct access to all control registers
 * allows fine tuning of various reader parameters as needed.
 *
 * The anti-collision procedures (as described in the ISO
 * standard 14443A) are implemented in the MCU firmware to help
 * the reader detect and communicate with one PICC among several
 * PICCs.
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

#include "nfc.h"
#include "trf797x.h"
#include "mcu.h"
#include "uart.h"

//===============================================================

const static u08_t u8DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//===============================================================

void main(void)
{
	// Stop the Watchdog timer
	WDTCTL = WDTPW + WDTHOLD;
	
	// Select DCO to be 8 MHz
	McuOscSel(0x00);
	McuDelayMillisecond(10);

	// This pin needs to be pulled low while in Special Direct Mode (SDM)
	MOD_OFF;
	MOD_DIR_OUT;

	// Need to pull I/O2 high to bring the TRF7970A up in SPI Mode.
	SDM_PORT_SET;
	SDM_TXENABLE_ON;

	McuDelayMillisecond(5);

	// Set the SPI SS high
	SLAVE_SELECT_PORT_SET;
	SLAVE_SELECT_HIGH;

	// Set TRF Enable Pin high
	TRF_ENABLE_SET;
	TRF_ENABLE;

	// Wait until TRF system clock started
	McuDelayMillisecond(5);

	// Initialize SPI settings for communication with TRF
	Trf797xCommunicationSetup();

#ifdef ENABLE_UART
	// Set up UART
	UartSetup();
#endif

	// Set up TRF initial settings
	Trf797xInitialSettings();
	
	// Wait until TRF to intialize
	McuDelayMillisecond(5);

	// Enable global interrupts
	__bis_SR_register(GIE);

    // Initial settings now complete
#ifdef ENABLE_UART
	UartTrfDFReaderMsg();
#endif

	while(1)
	{	
		// Set MFC Key to a custom key stored in Flash Memory
		Nfc_setMFCKey(u8DefaultKey, sizeof(u8DefaultKey));

		// Runs Selection and Authentication sequence
		Nfc_runMFCAuth();

		// Arbitrary delay between each authentication to simulate the run time of a user defined application
		McuDelayMillisecond(200);
	}
}
