/****************************************************************
* Firmware of the TI TRF7960A EVM Rev. 03 
* 25.Feb.2011
* Copyright (C) 2011 Texas Instruments, Inc. 
*
* DESCRIPTION: The TRF796x is an integrated analog front end and
* data framing system for a 13.56 MHz RFID reader system.
* Built-in programming options make it suitable for a wide range
* of applications both in proximity and vicinity RFID systems.
* The reader is configured by selecting the desired protocol in
* the control registers. Direct access to all control registers
* allows fine tuning of various reader parameters as needed.
*
* The TRF796x is interfaced to a MSP430F2370 microcontroller
* through a parallel interface or a  SPI (serial) interface using
* hardware Usart. The MCU is the master device and initiates all
* communication with the reader. The interface is selected by
* jumper setting.
*
* The anti-collision procedures (as described in the ISO
* standards 14443A/B and 15693) are implemented in the MCU
* firmware to help the reader detect and communicate with one
* PICC/VICC among several PICCs/VICCs.
*
* Initial the Firmware is running in stand alone mode. Found tags
* are displayed by LEDs. If a host is detected, the Firmware will
* switch to remote mode by host_control_flag.
*
* The MCU communicates with the higher level host station
* (normally a PC) through a UART. The user can send the desired
* commands and the MCU interprets the data received and sends
* appropriate commands to the TRF796x.
* 
*
* AUTHOR:	Reiser Peter	DATE: 07 DEC 2010
*
* CHANGES:
* REV.	DATE		WHO	DETAIL
* 00	02Dec2010	RP	Orginal Version
* 01	07Dec2010	RP	Changed SPI clock frequency in spi.c
* 02	22Dec2010	RP	Included write cmd for MF UL in host.c
* 03	27Jan2011	RP	Inserted mcu.c/mcu.h to select mcu
* 						specific functions
* 03	24Feb2011	RP	renamed remote_mode to host_control_mode
*
* BUILT WITH:
* Code Composer Studio Core Edition Version: 4.2.1.00004 
* (C) Copyright Texas Instruments, 2009. All rights reserved.
*****************************************************************
*****************************************************************
* All software and related documentation is provided "AS IS" and
* without warranty or support of any kind and Texas Instruments
* expressly disclaims all other warranties, express or implied,
* including, but not limited to, the implied warranties of
* merchantability and fitness for a particular purpose. Under no
* circumstances shall Texas Instruments be liable for any
* incidental, special or consequential damages that result from
* the use or inability to use the software or related
* documentation, even if Texas Instruments has been advised of
* the liability.
*
* Unless otherwise stated, software written and copyrighted by
* Texas Instruments is distributed as "freeware". You may use
* and modify this software without any charge or restriction.
* You may distribute to others, as long as the original author
* is acknowledged.
****************************************************************/

//===============================================================
// Program with hardware USART and paralel communication        ;
// interface with TRF reader chip.                              ;
//                                                              ;
// PORT4.0 - 4.7 - (IO0-IO7) for parallel interface             ;
//                           with reader chip                   ;
// PORT3.0 - PORT3.3 - USCI_B0 ---> SPI interface with reader   ;
// PORT3.4 - PORT3.5 - USCI_A0 ---> USB/UART control signals    ;
// PORT2.1 - IRQ                                                ;
// PORT3.3 - DATA_CLK                                           ;
// PORT1.7 - PORT1.3 - signaling LEDs                           ;
// PORT1.0 - TRF Enable                                         ;
//===============================================================
// NFC Type 2 can be enabled in type2.h                         ;
// This should only be done for TRF7960A.                       ; 
//===============================================================

#include <MSP430x23x0.h> 	// prozessor spezific header
#include <stdlib.h>			// general purpose standard library
#include <stdio.h>			// standard input/output header
#include "host.h"
#include "iso14443a.h"
#include "iso14443b.h"
#include "iso15693.h"
#include "mcu.h"
#include "trf796x.h"
#include "types.h"
#include "uart.h"

//===============================================================

u08_t buf[300];

u08_t enable = 0;

u08_t i_reg = 0x01;							// interrupt register

u08_t irq_flag = 0x00;
u08_t rx_error_flag = 0x00;
s08_t rxtx_state = 1;							// used for transmit recieve byte count

u08_t host_control_flag = 0;
u08_t stand_alone_flag = 1;

//===============================================================
//
//===============================================================

void main (void)	{
	
	// settings
	
	WDTCTL = WDTPW + WDTHOLD;			// stop watchdog timer
	
	#ifdef ENABLE_HOST
		UartSetup();					// settings for UART
	#endif
	
	ENABLE_SET;							// P1.0 is switched in output direction
	TRF_ENABLE;							// P1.0 = 1, EN pin on the TRF 796x
	McuDelayMillisecond(1);				// wait until system clock started
	
	Trf796xCommunicationSetup();		// settings for communication with TRF
		
	Trf796xInitialSettings();			// Set MCU Clock Frequency to 6.78 MHz and OOK Modulation
	
	// Now switch from DCO to external 6.78 MHz clock

	McuOscSel(0x00);       				// set the oscilator
	McuDelayMillisecond(10);
	
	// Re-configure the USART with this external clock

	Trf796xReConfig();
     	
	ENABLE_INTERRUPTS;					// General enable interrupts
    
	LED_POWER_ON;						// board ist powered, green LED
    
	OOK_DIR_IN;							// set OOK port tristate
	enable = 1;							// indicates, that setting are done

	stand_alone_flag = 1;				// stand alone mode
	host_control_flag = 0;					// host is not activ
    
    //	settings done
    
	while(1)							// infinite loop
	{	
		#if TRIGGER						// in msp430f2370.h
			LED_OPEN1_ON;
			McuDelayMillisecond(1);
			LED_OPEN1_OFF;
		#endif
		if(host_control_flag == 1) 			// if in remote mode
		{								// remote mode can be disabled in host.h
			#ifdef ENABLE_HOST
				buf[4] = 0xff;			// "TRF7960 EVM" message in GUI
				HostCommands();			// device woks according host commands
			#endif
		}
		else
		{	
			#ifdef ENABLE15693				// this standard can be disabled in ISO15693.h
				Iso15693FindTag();			// detects ISO15693 in stand-alone mode
			#endif
			if(host_control_flag == 0)
			{	
				#ifdef ENABLE14443A			// this standard can be disabled in ISO14443A.h 
					Iso14443aFindTag();		// detects ISO14443A in stand-alone mode
				#endif
			}
			if(host_control_flag == 0)
			{
				#ifdef ENABLE14443B			// this standard can be disabled in ISO14443B.h
					Iso14443bFindTag();		// detects ISO14443B in stand-alone mode
				#endif
			}
		}
	}
}
