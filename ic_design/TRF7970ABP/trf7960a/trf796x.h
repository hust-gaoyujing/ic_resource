//***************************************************************
//------------<02.Dec.2010 by Peter Reiser>----------------------
//***************************************************************

#ifndef _TRF796X_H_
#define _TRF796X_H_

//===============================================================

#include <MSP430x23x0.h> 			// prozessor spezific header
#include <stdlib.h>					// general purpose standard library
//#include <stdio.h>					// standard input/output header
#include "iso14443a.h"
#include "mcu.h"
#include "parallel.h"
#include "spi.h"
#include "types.h"
#include "uart.h"

//===============================================================

#define DBG	0						// if DBG 1 interrupts are display

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

#define CHIP_STATE_CONTROL			0x00
#define ISO_CONTROL					0x01
#define ISO_14443B_OPTIONS			0x02
#define ISO_14443A_OPTIONS			0x03
#define TX_TIMER_EPC_HIGH			0x04
#define TX_TIMER_EPC_LOW			0x05
#define TX_PULSE_LENGTH_CONTROL		0x06
#define RX_NO_RESPONSE_WAIT_TIME	0x07
#define RX_WAIT_TIME				0x08
#define MODULATOR_CONTROL			0x09
#define RX_SPECIAL_SETTINGS			0x0A
#define REGULATOR_CONTROL			0x0B
#define IRQ_STATUS					0x0C	// IRQ Status Register
#define IRQ_MASK					0x0D	// Collision Position and Interrupt Mask Register
#define	COLLISION_POSITION			0x0E
#define RSSI_LEVELS					0x0F
#define SPECIAL_FUNCTION			0x10
#define RAM_START_ADDRESS			0x11	//RAM is 6 bytes long (0x11 - 0x16)
#define NFCID						0x17
#define NFC_TArGET_LEVEL			0x18
#define NFC_TARGET_PROTOCOL			0x19
#define TEST_SETTINGS_1				0x1A
#define TEST_SETTINGS_2				0x1B
#define FIFO_STATUS					0x1C
#define TX_LENGTH_BYTE_1			0x1D
#define TX_LENGTH_BYTE_2			0x1E
#define FIFO						0x1F

//===============================================================

void Trf796xCommunicationSetup(void);
void Trf796xDirectCommand(u08_t *pbuf);
void Trf796xDirectMode(void);
void Trf796xDisableSlotCounter(void);
void Trf796xEnableSlotCounter(void);
void Trf796xInitialSettings(void);
void Trf796xRawWrite(u08_t *pbuf, u08_t length);
void Trf796xReConfig(void);
void Trf796xReadCont(u08_t *pbuf, u08_t length);
void Trf796xReadIrqStatus(u08_t *pbuf);
void Trf796xReadSingle(u08_t *pbuf, u08_t length);
void Trf796xReset(void);
void Trf796xResetIrqStatus(void);
void Trf796xRunDecoders(void);
void Trf796xStopDecoders(void);
void Trf796xTransmitNextSlot(void);
void Trf796xTurnRfOff(void);
void Trf796xTurnRfOn(void);
void Trf796xWriteCont(u08_t *pbuf, u08_t length);
void Trf796xWriteIsoControl(u08_t iso_control);
void Trf796xWriteSingle(u08_t *pbuf, u08_t length);

//===============================================================

#endif
