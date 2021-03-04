/*
 * mifare_sdm.h
 *
 *  Created on: Nov 7, 2014
 *      Author: a0273119
 */

#ifndef __MIFARE_H__
#define __MIFARE_H__

#include "trf797x.h"
#include "mcu.h"
//#include "iso_dep.h"
//#include "crypto1_mifare.h"a
//#include "crypto1.h"

#define SPECIAL_DIRECT_MODE_PARITY 1

//*****************************************************************************
//
// List of MIFARE states
//
//*****************************************************************************
typedef enum
{
    MIFARE_IDLE = 0x00,
    MIFARE_TRANSMIT,
    MIFARE_RECEIVE_ONGOING,
    MIFARE_RECEIVE_SUCCESS,
    MIFARE_RECEIVE_FAIL

}tMifareConnectionStatus;

//*****************************************************************************
//
// List of pins needed to touch in the MIFARE functions
//
//*****************************************************************************
typedef enum
{
    MIFARE_SDM_TX_ENABLE = 0x00,
    MIFARE_SDM_DATA,
    MIFARE_SDM_BIT_CLK,
    MIFARE_SDM_SPI_MISO,
    MIFARE_SDM_SPI_SLAVE_SELECT

}tMifareSdmGpio;


extern uint8_t g_ui8DirectMode;
extern uint8_t g_ui8TimeOutFlag;

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************
void MIFARE_init(void);

void MIFARE_gpioInit(tMifareSdmGpio eMifareSdmGpio, uint32_t pui32PortBase,
		uint8_t ui8GpioPin);

void MIFARE_stateMachine(void);

uint8_t MIFARE_sendRawData(uint16_t * pui16Data, uint8_t ui8Length,
		bool bTxParityEnable, bool bRxParityEnable);

tMifareConnectionStatus MIFARE_getCommStatus(void);

void MIFARE_sdmConfig(void);

void MIFARE_sdmEnter(void);

void MIFARE_sdmExit(void);

void MIFARE_sdmTransmit(uint16_t *pui16TxData, uint8_t ui8ByteCount, bool bSDMTxParity);

void MIFARE_dm1Enter(void);

void MIFARE_dm1Exit(void);

void MIFARE_dm1Receive(uint16_t *pui16RxData, uint8_t *pui8ByteCount,
		uint8_t *pui8BitCount, bool bParityFlag);

void MIFARE_getPacketStatus(uint16_t ** pui16RxData, uint8_t * pui8RxByteLength, uint8_t * pui8RxBitLength);


#endif /* __MIFARE_H__ */
