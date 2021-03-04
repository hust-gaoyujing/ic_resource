/*
 * File Name: trf797x.h
 *
 * Description: Header file for all functions for trf797x.c and the
 * definitions for various TRF797x registers, settings, and commands.
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

#ifndef _TRF797x_H_
#define _TRF797x_H_

//===============================================================

#include "spi.h"
#include "types.h"
//#include "msp430.h"
#include <msp430f2370.h>
//===============================================================

#define	ENABLE14443A

//==== TRF796x definitions ======================================

//---- Direct commands ------------------------------------------

#define IDLE						0x00
#define SOFT_INIT					0x03
#define INITIAL_RF_COLLISION		0x04
#define RESPONSE_RF_COLLISION_N		0x05
#define RESPONSE_RF_COLLISION_0		0x06
#define	RESET						0x0F
#define TRANSMIT_NO_CRC				0x10
#define TRANSMIT_CRC				0x11
#define DELAY_TRANSMIT_NO_CRC		0x12
#define DELAY_TRANSMIT_CRC			0x13
#define TRANSMIT_NEXT_SLOT			0x14
#define CLOSE_SLOT_SEQUENCE			0x15
#define STOP_DECODERS				0x16
#define RUN_DECODERS				0x17
#define CHECK_INTERNAL_RF			0x18
#define CHECK_EXTERNAL_RF			0x19
#define ADJUST_GAIN					0x1A

//---- Reader register ------------------------------------------

#define TRF797x_STATUS_CONTROL				0x00
#define TRF797x_ISO_CONTROL					0x01
#define TRF797x_ISO_14443_TX_OPTIONS		0x02
#define TRF797x_ISO_14443_BITRATE_OPTIONS	0x03
#define TRF797x_TX_TIMER_EPC_HIGH			0x04
#define TRF797x_TX_TIMER_EPC_LOW			0x05
#define TRF797x_TX_PULSE_LENGTH_CONTROL		0x06
#define TRF797x_RX_NO_RESPONSE_WAIT_TIME	0x07
#define TRF797x_RX_WAIT_TIME				0x08
#define TRF797x_MODULATOR_CONTROL			0x09
#define TRF797x_RX_SPECIAL_SETTINGS			0x0A
#define TRF797x_REGULATOR_CONTROL			0x0B
#define TRF797x_IRQ_STATUS					0x0C	// IRQ Status Register
#define TRF797x_IRQ_MASK					0x0D	// Collision Position and Interrupt Mask Register
#define	TRF797x_COLLISION_POSITION			0x0E
#define TRF797x_RSSI_LEVELS					0x0F
#define TRF797x_SPECIAL_FUNCTION			0x10
#define TRF797x_RAM_START_ADDRESS			0x11	//RAM is 6 bytes long (0x11 - 0x16)
#define TRF797x_FIFO_IRQ_LEVELS				0x14
#define TRF797x_NFC_LOW_DETECTION			0x16
#define TRF797x_NFC_TARGET_LEVEL			0x18
#define TRF797x_NFC_TARGET_PROTOCOL			0x19
#define TRF797x_TEST_SETTINGS_1				0x1A
#define TRF797x_TEST_SETTINGS_2				0x1B
#define TRF797x_FIFO_STATUS					0x1C
#define TX_LENGTH_BYTE_1					0x1D
#define TX_LENGTH_BYTE_2					0x1E
#define FIFO								0x1F

//---- IRQ STATUS ------------------------------------------v

#define TRF797x_IRQ_STATUS_IDLE					0x00
#define TRF797x_IRQ_STATUS_NO_RESPONSE			0x01
#define TRF797x_IRQ_STATUS_COLLISION_ERROR		0x02
#define TRF797x_IRQ_STATUS_FRAMING_ERROR 		0x04
#define TRF797x_IRQ_STATUS_PARITY_ERROR 		0x08
#define TRF797x_IRQ_STATUS_CRC_ERROR	 		0x10
#define TRF797x_IRQ_STATUS_FIFO_HIGH_OR_LOW 	0x20
#define TRF797x_IRQ_STATUS_RX_COMPLETE 			0x40
#define TRF797x_IRQ_STATUS_TX_COMPLETE 			0x80

//===============================================================

////********* SDM************************************************
//#define SDM_TXENABLE	(BIT2)	// port 4.2
//#define SDM_DATA		(BIT3)	// port 4.3
//#define SDM_BITCLK_BIT 	(BIT5)  // port 4.5
//
//
//#define SDM_PORT_SET  	P4DIR |= SDM_TXENABLE; 		\
//						P4DIR &= ~SDM_BITCLK_BIT; 	\
//						P4DIR |= SDM_DATA;	//		\
////						P4IES &= ~SDM_BITCLK_BIT;	\
////						P4REN |= SDM_BITCLK_BIT;
//
//#define MOD_DIR_OUT	 P2DIR |= BIT0
//#define MOD_OFF		 P2OUT &= ~BIT0
//#define MOD_ON		 P2OUT |= BIT0
//
////#define SDM_PORT_DIR    P4DIR
////#define SDM_PORT_OUT    P4OUT
////#define SDM_PORT_IN   	P4IN
//
//#define SDM_TXENABLE_ON    	P4OUT |= SDM_TXENABLE
//#define SDM_TXENABLE_OFF   	P4OUT &= ~SDM_TXENABLE
//#define SDM_DATA_ON    		P4OUT |= SDM_DATA
//#define SDM_DATA_OFF   		P4OUT &= ~SDM_DATA
//#define SDM_BITCLK    		(P4IN & SDM_BITCLK_BIT)
//#define SDM_BITCLK_EDGE 	(P4IFG & SDM_BITCLK_BIT)
//#define SDM_BITCLK_EDGE_CLEAR 	(P4IFG &= ~SDM_BITCLK_BIT)
//#define SDM_BITCLK_ENT    	(P4IE |= SDM_BITCLK_BIT)
//#define SDM_BITCLK_DIS    	(P4IE &= ~SDM_BITCLK_BIT)
////#define SDM_BITCLK_LOW   	SDM_PORT_IN &= ~SDM_BITCLK_BIT
//
//#define MISO_DATA_SETUP		SPI_PORT_SEL1 &= ~MISO_PIN;  \
//                      	    /*SPI_PORT_SEL2 &= ~MISO_PIN;*/  \
//							SPI_PORT_DIR &= ~MISO_PIN;
//
//#define MISO_SPI_SETUP		SPI_PORT_SEL1 |= MISO_PIN;	\
//                      	  	/*SPI_PORT_SEL2 |= MISO_PIN;*/	\
//							SPI_PORT_DIR &= ~MISO_PIN;
//
//#define DM1_DATA_BIT		(SPI_PORT_IN & MISO_PIN)
////******************************************************************

//typedef enum
//{
//    IRQ_STATUS_IDLE = 0x00,
//    IRQ_STATUS_COLLISION_ERROR = 0x01,
//    IRQ_STATUS_COLLISION_AVOID_FINISHED = 0x02,
//    IRQ_STATUS_RF_FIELD_CHANGE = 0x04,
//    IRQ_STATUS_SDD_COMPLETE = 0x08,
//    IRQ_STATUS_PROTOCOL_ERROR = 0x10,
//    IRQ_STATUS_FIFO_HIGH_OR_LOW  = 0x20,
//    IRQ_STATUS_RX_COMPLETE = 0x40,
//    IRQ_STATUS_TX_COMPLETE = 0x80,
//    IRQ_STATUS_OVERFLOW = 0xFC,
//    IRQ_STATUS_NO_RESPONSE = 0xFE,
//    IRQ_STATUS_TIME_OUT = 0xFF
//} tTRF797x_IRQFlag;

typedef enum
{
	TRF_IDLE,				// New
	TX_COMPLETE,			// Formally 0x00
	RX_COMPLETE,			// Formally 0xFF
	RX_WAIT,				// Formally 0x01
	PROTOCOL_ERROR,			// Formally 0x02
	NO_RESPONSE_RECEIVED,	// Formally 0x00
	NO_RESPONSE_RECEIVED_15693 	// Added for 15693 cases.
}tTRF797x_Status;

//===============================================================

void Trf797xCommunicationSetup(void);
void Trf797xDirectCommand(u08_t *pbuf);
void Trf797xInitialSettings(void);
void Trf797xRawWrite(u08_t *pbuf, u08_t length);
void Trf797xReConfig(void);
void Trf797xReadCont(u08_t *pbuf, u08_t length);
void Trf797xReadIrqStatus(u08_t *pbuf);
void Trf797xReadSingle(u08_t *pbuf, u08_t length);
void Trf797xReset(void);
void Trf797xResetIrqStatus(void);
void Trf797xRunDecoders(void);
void Trf797xStopDecoders(void);
void Trf797xTurnRfOff(void);
void Trf797xTurnRfOn(void);
void Trf797xWriteCont(u08_t *pbuf, u08_t length);
void Trf797xWriteIsoControl(u08_t iso_control);
void Trf797xWriteInitiatorSetup(void);
void Trf797xWriteSingle(u08_t *pbuf, u08_t length);
void Trf797xIrqWaitTimeout(u08_t txtimeout, u08_t rxtimeout);
void Trf797x_timeout_ms(u16_t ui16Delay_ms);
void Trf797xIrqWaitTimeoutTxOnly(u08_t txtimeout);

extern
tTRF797x_Status Trf797xGetTrfStatus(void);
extern
void Trf797xSetTrfStatus(tTRF797x_Status sTrfStatus);
extern
bool Trf797xGetRxErrorFlag(void);
extern
void Trf797xSetRxErrorFlag(bool bErrorFlag);
extern
void Trf797xSetRxTxState(s08_t RxTxState);
extern
void Trf797xSetCollisionPosition(u08_t ui8ColPos);
extern
u08_t Trf797xGetCollisionPosition(void);
u08_t Trf797xGetRxBytesReceived(void);

extern unsigned char g_ui8DirectMode;
//extern unsigned char g_ui8TimeoutFlag;
//===============================================================

#endif
