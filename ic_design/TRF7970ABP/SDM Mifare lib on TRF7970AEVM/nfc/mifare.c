//*****************************************************************************
//
// mifare.c - Implementation of Direct Mode communitation for Mifare Classic Support
//
// Copyright (c) 2014 Texas Instruments Incorporated.  All rights reserved.
// TI Information - Selective Disclosure
//
//*****************************************************************************

#include "mifare.h"

//*****************************************************************************
//
//! \addtogroup mifare_api Mifare API Functions
//! @{
//! Mifare is the RFID protocol layer used authenticate / read / write data 
//! from Mifare tags. 
//
//*****************************************************************************

//*****************************************************************************
//
// Stores the status of MIFARE communication
//
//*****************************************************************************
static tMifareConnectionStatus g_eMifareCommStatus;

//*****************************************************************************
//
// Stores the length of the data to be transmitted.
//
//*****************************************************************************
static uint8_t g_ui8TxLength;

//*****************************************************************************
//
// g_pui16TxData points to the first location of the data to be transmitted
//
//*****************************************************************************
static uint16_t * g_pui16TxData;

//*****************************************************************************
//
// Stores the length of the complete bytes received
//
//*****************************************************************************
static uint8_t g_ui8RxLengthBytes;

//*****************************************************************************
//
// Stores the length of the remaining bites (from an incomplete byte) received
//
//*****************************************************************************
static uint8_t g_ui8RxLengthBits;

//*****************************************************************************
//
// g_pui16RxData points to the first location of the recieved data
//
//*****************************************************************************
static uint16_t g_pui16RxData[40];

//*****************************************************************************
//
// Sets transmitting with parity
//
//*****************************************************************************
static bool g_bTxParity;

//*****************************************************************************
//
// Sets receiving with parity
//
//*****************************************************************************
static bool g_bRxParity;

//*****************************************************************************
//
// Need a way to pass in pins usage
//
//*****************************************************************************
uint32_t g_ui32SDMDataPortBase;
uint8_t g_ui8SDMDataPin;
//TODO Finish setting this part up

//*****************************************************************************
//
//!MIFARE_init - Initialize the MIFARE Classic driver.
//!
//! This function must be called prior to any other function offered by the
//! MIFARE classic driver.  This funciton initializes the MIFARE communication
//! status, tx/rx packet lengths, and parity flags.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_init(void)
{
	g_eMifareCommStatus = MIFARE_IDLE;
	g_ui8TxLength = 0x00;
	g_ui8RxLengthBytes = 0x00;
	g_ui8RxLengthBits = 0x00;
	g_bTxParity = false;
	g_bRxParity = false;
}

//*****************************************************************************
//
//!MIFARE_gpioInit - Initialize each GPIO for the MIFARE Classic driver.
//!
//! \param eMifareSdmGpio indicates which Direct Mode pin is being configured.
//!
//! \param ui32PortBase indicates which port should be used.
//!
//! \param ui8GpioPin indicates which pin should be used.
//!
//!// TODO put what functions end up calling this function
//! This function must be called before attempting to tx/rx to ensure the gpios
//! are correctly configured.  It initializes the gpio based on input from
//! the application layer, allowing user space to choose the correct pins.  The
//! function must be called for each GPIO needing to be configured.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_gpioInit(tMifareSdmGpio eMifareSdmGpio, uint32_t ui32PortBase, uint8_t ui8GpioPin)
{
	switch(eMifareSdmGpio){
	case MIFARE_SDM_TX_ENABLE:
		break;
	case MIFARE_SDM_DATA:
		g_ui32SDMDataPortBase = ui32PortBase;
		g_ui8SDMDataPin = ui8GpioPin;
		break;
//TODO Finalize
	}
}

//*****************************************************************************
//
//!MIFARE_stateMachine - MIFARE Classic Communication state machine
//!
//! This function is called from the MIFARE_DEP state inside of
//! NFC_Initiator_State_Machine.  This state machines handles the direct Mode
//! communication for MIFARE classic tags.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_stateMachine(void)
{
	switch(g_eMifareCommStatus)
	{
	case MIFARE_IDLE:
		//TODO find out a way to do presence check while (encrypted read block?)
		break;
	case MIFARE_TRANSMIT:
		MIFARE_sdmConfig();
		MIFARE_sdmEnter();       	//Enter Special Direct Mode
		MIFARE_sdmTransmit(g_pui16TxData, g_ui8TxLength, g_bTxParity);
		MIFARE_sdmExit();

		//next state logic
		if(g_ui8TimeOutFlag){
			// Error State
			//TODO need to Clear IRQ on all paths
			g_eMifareCommStatus = MIFARE_RECEIVE_FAIL;
		}
		else if(g_ui8IrqFlag){
			g_eMifareCommStatus = MIFARE_RECEIVE_FAIL;
		}
		else{
			g_eMifareCommStatus = MIFARE_RECEIVE_ONGOING;
		}

		break;
	case MIFARE_RECEIVE_ONGOING:

		//***** MFC RX **********
		MIFARE_dm1Enter();
		MIFARE_dm1Receive(g_pui16RxData, &g_ui8RxLengthBytes, &g_ui8RxLengthBits, g_bRxParity);
		MIFARE_dm1Exit();
		//TODO need to handle IRQ error case.  :/
//		}
		//************************
		if(g_ui8TimeOutFlag){
			// Error State
			g_eMifareCommStatus = MIFARE_RECEIVE_FAIL;
		}
		else if(g_ui8IrqFlag){
			g_eMifareCommStatus = MIFARE_RECEIVE_FAIL;
		}
		else{
			g_eMifareCommStatus = MIFARE_RECEIVE_SUCCESS;
		}

		break;
	case MIFARE_RECEIVE_SUCCESS:
		break;
	case MIFARE_RECEIVE_FAIL:
		break;
	default:
		break;
	}
}

//*****************************************************************************
//
// g_pui16TxData points to the first location of the data to be transmitted
//
//*****************************************************************************

//*****************************************************************************
//
//! MIFARE_sendRawData - MIFARE Classic Transceive API
//!
//! \param pui16Data is the starting pointer for the data to be transmitted
//!
//! \param ui8Length is the number of bytes being transmitted
//!
//! \param bTxParityEnable sets transmitting parity
//!
//! \param bRxParityEnable sets receiving parity.
//!
//! This API should only be called after successful anticollision and selection
//! are completed.  This API transmits out the raw data passed in, allowing support
//! for some propreitary protocols such as MIFARE classic.
//!
//! \return ui8Status indicates if the MIFARE_stateMachine was ready
//
//*****************************************************************************
uint8_t MIFARE_sendRawData(uint16_t * pui16Data, uint8_t ui8Length, bool bTxParityEnable, bool bRxParityEnable)
{
	uint8_t ui8Status = STATUS_SUCCESS;

	if(g_eMifareCommStatus == MIFARE_IDLE)
	{
		g_bTxParity = bTxParityEnable;
		g_bRxParity = bRxParityEnable;

		g_ui8TxLength = ui8Length;
		g_pui16TxData = pui16Data;

		g_eMifareCommStatus = MIFARE_TRANSMIT;
	}
	else
	{
		ui8Status = STATUS_FAIL;
	}

	return ui8Status;
}

//*****************************************************************************
//
//!MIFARE_getCommStatus - API that returns MIFARE_stateMachine state
//!
//! This API is used to check final transceive status.  It also resets the
//! MIFARE state machine back to it's starting state.
//!
//! \return eStatusTemp which was the current state of the MIFARE_stateMachine
//
//*****************************************************************************
tMifareConnectionStatus MIFARE_getCommStatus(void)
{
	tMifareConnectionStatus eStatusTemp;

	// Save current state
	eStatusTemp = g_eMifareCommStatus;

	// Set current state to Mifare Idle
	g_eMifareCommStatus = MIFARE_IDLE;

	return eStatusTemp;
}

//*****************************************************************************
//
//! MIFARE_sdmConfig - Configures MCU and TRF7970 for Special Direct Mode (SDM)
//!
//! This function is called from the MIFARE_stateMachine, just before using SDM
//! to transmit out the next packet.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_sdmConfig(void){
	uint8_t dummy_read;

	// TODO make these functions that pass in a variable
	// I can use Driver lib here
	SDM_TXENABLE_OFF;
	SDM_DATA_OFF;
	SDM_PORT_SET;

	TRF79x0_writeSingle(0x3E, TRF79X0_IRQ_MASK_REG); 		// Need to make sure IRQ ISR can handle this turning off Time out IRQ

	TRF79x0_writeSingle(0x88, TRF79X0_ISO_CONTROL_REG); 	// recieve with no CRC

	TRF79x0_writeSingle(0x21, TRF79X0_CHIP_STATUS_CTRL_REG);

	TRF79x0_directCommand(TRF79X0_STOP_DECODERS_CMD);   	//Disable decoder

	TRF79x0_readSingle(&dummy_read,TRF79X0_IRQ_STATUS_REG);  //Clear IRQStatus
	g_ui8IrqFlag = 0;
}

//*****************************************************************************
//
//! MIFARE_sdmEnter - Enters TRF7970A's Special Direct Mode (SDM)
//!
//! This function is called from the MIFARE_stateMachine, just before using SDM
//! to transmit out the next packet.  If configures the TRF7970 just to enter SDM.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_sdmEnter(void)
{
	uint8_t dummy_write = 0;

	TRF79x0_writeSingle(0x08, TRF79X0_SPECIAL_FUNC_1_REG);

	g_ui8DirectMode = 1;  // Enter Direct by keeping Slave Select Low
	TRF79x0_writeSingle(0x61, TRF79X0_CHIP_STATUS_CTRL_REG);
	TRF79x0_rawWrite(&dummy_write, 1);		// 8 dummy clocks

	g_ui8TimeOutFlag = 0;
	MCU_timerInit(40, (uint8_t*) &g_ui8TimeOutFlag);
}

//*****************************************************************************
//
//! MIFARE_sdmExit - Exit TRF7970A's Special Direct Mode (SDM)
//!
//! This function is called from the MIFARE_stateMachine, just after using SDM
//! to transmit out the packet.  If configures the TRF7970 back to SPI mode.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_sdmExit(void)
{
	//Time critical
	SLAVE_SELECT_HIGH;  // TODO: can I access this in this file...

//	MCU_timerDisable();

	g_ui8DirectMode = 0;
	TRF79x0_writeSingle(0x00, TRF79X0_SPECIAL_FUNC_1_REG);
}

//*****************************************************************************
//
//! MIFARE_sdmTransmit - Transmits Data using TRF7970A's Special Direct Mode (SDM)
//!
//! \param pui16TxData is a pointer to the beginning of the TX data
//!
//! \param ui8ByteCount is the length of the Tx Packet
//!
//! \param bSDMTxParity is the parity flag for Transmitting
//!
//! This function is called from the MIFARE_stateMachine. It takes in the TX data,
//! length, and parity information and transmits the packet out to the tag.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_sdmTransmit(uint16_t *pui16TxData, uint8_t ui8ByteCount, bool bSDMTxParity){
	uint8_t i, j, ui8ByteWidth;

	SDM_TXENABLE_ON;  //TODO Driverlib is fine here

	if(bSDMTxParity){
		ui8ByteWidth = 9;
	}
	else{
		ui8ByteWidth = 8;
	}

	for(i=0; i<ui8ByteCount; i++){
		for(j=0; j<ui8ByteWidth; j++){
			while(SDM_BITCLK && (!g_ui8IrqFlag) && (!g_ui8TimeOutFlag));  			// wait for Falling edge

			if(pui16TxData[i] & (0x01 << j)){  // Decides if next bit should be High or low.
				while((!SDM_BITCLK) && (!g_ui8IrqFlag) && (!g_ui8TimeOutFlag));		// wait for Rising edge
				SDM_DATA_ON;			// "1"
			}
			else{
				while((!SDM_BITCLK)  && (!g_ui8IrqFlag) && (!g_ui8TimeOutFlag));		// wait for Rising edge
				SDM_DATA_OFF;			// "0"
			}
		}
	}

	/********** EOF Sequence ********/
	if((!g_ui8IrqFlag) ){
		while(SDM_BITCLK && (!g_ui8TimeOutFlag));
//		__delay_cycles(15);
		while(!SDM_BITCLK && (!g_ui8TimeOutFlag));
//		__delay_cycles(40);
		SDM_TXENABLE_OFF;		// TODO Driverlib is fine here
		while(SDM_BITCLK && (!g_ui8TimeOutFlag));
		while(!SDM_BITCLK && (!g_ui8TimeOutFlag));
	}
}

//*****************************************************************************
//
//! MIFARE_dm1Enter - Enter Direct Mode 1 (DM1) for receiving
//!
//! This function is called from the MIFARE_stateMachine. It should be called as
//! fast as possible after completely a transmit to be listen when the response
//! starts to be received.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_dm1Enter(void)
{
	uint8_t dummy_buf[2];
	uint8_t dummy_write = 0;

	//todo CHECK THIS, NEW!!!!
	TRF79x0_readCont(dummy_buf, TRF79X0_IRQ_STATUS_REG, 2);


	TRF79x0_writeSingle(0x48, TRF79X0_ISO_CONTROL_REG);

	g_ui8DirectMode = 1;  // Enter Direct by keeping Slave Select Low
	TRF79x0_writeSingle(0x61, TRF79X0_CHIP_STATUS_CTRL_REG);
	TRF79x0_rawWrite(&dummy_write, 1);		// 8 dummy clocks
}

//*****************************************************************************
//
//! MIFARE_dm1Exit - Exit Direct Mode 1 (DM1)
//!
//! This function is called from the MIFARE_stateMachine. It should be called
//! after the the response packet has been received or a timeout occured.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_dm1Exit(void)
{
	uint8_t temp[1];

	SLAVE_SELECT_HIGH;

	g_ui8DirectMode = 0;

	MCU_timerDisable();

	TRF79x0_writeSingle(0x00, TRF79X0_SPECIAL_FUNC_1_REG);

	TRF79x0_writeSingle(0x88, TRF79X0_ISO_CONTROL_REG);

	g_ui8IrqFlag = 0;
	TRF79x0_readSingle(temp, TRF79X0_IRQ_STATUS_REG); // Clear IRQ
}

//*****************************************************************************
//  TODO Finish this last comment
//! MIFARE_dm1Receive - Builds rx packet as each bit is received
//!
//! \param pui16RxData is a pointer to where the receive packet is built and stored
//!
//! \param pui8ByteCount is a pointer for passing the received byte count
//!
//! \param pui8BitCount is a pointer for passing the received bit count
//!
//! \param bParityFlag indicates if rx with parity (needed for correctly building bytes)
//!
//! This function is called from the MIFARE_stateMachine. It is called after MIFARE_dm1Enter.
//! This funciton takes the rx bits, stores them into bytes, and passes this and their length
//! back up for decryption.  The Parity_Flag = 1 indicates rx with parity, meaning each byte
//! should be 9 bits wide.  IRQ goes high, to signal the end of rx.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_dm1Receive(uint16_t *pui16RxData, uint8_t *pui8ByteCount, uint8_t *pui8BitCount, bool bParityFlag){  // polling method

	uint8_t ui8TempByteCount = 0;
	uint8_t ui8TempBitCount = 0;
	uint8_t ui8ByteWidth = 0;

		/* define this to use MOSI Signal to display sample edges. */
 #define DM1_RX_DEBUGGING_SIGNAL

#ifdef DM1_RX_DEBUGGING_SIGNAL
	SDM_DATA_OFF;
	//SETUP MOSI PIN FOR DEBUGGING ********************
//	GPIO_setAsOutputPin(SPI_PORT, SPI_MOSI);
//	P3OUT &= ~BIT0;
//	SPI_PORT_SEL1 &= ~MOSI_PIN;
//    SPI_PORT_SEL2 &= ~MOSI_PIN;
//    SPI_PORT_DIR |= MOSI_PIN;
//    SPI_PORT_OUT &= ~MOSI_PIN;
    // ************************************************
#endif  /* DM1_RX_DEBUGGING_SIGNAL */

	MISO_DATA_SETUP; 	 // DATA signal

	ui8TempByteCount = 0;
	ui8TempBitCount = 0;
	pui16RxData[ui8TempByteCount] = 0; // initialize buffer as used

	if(bParityFlag){
		ui8ByteWidth = 9;
	}
	else{
		ui8ByteWidth = 8;
	}

	/*
	 * In Direct Mode 1 Receiving, RX_Data (MISO) is updated on the falling Bit_Clk edge (I/O_5), and latched on the rising Bit_Clk edge.
	 * To make this decode functionality work @ 8MHz DCO, I only check for the Bit_Clk falling edge, then sample the Data.
	 * This allows us to latch proper data, and gives us the best time flexibility.
	 *
	 * Currently, interrupts are disabled in Direct mode.  Instead of using the timer interrupt, the CCIFG bit is polled instead
	 */

	while( !(SDM_BITCLK || g_ui8TimeOutFlag) );  // wait for first rising edge: Start of RX

	do{

#ifdef DM1_RX_DEBUGGING_SIGNAL
		HWREGB(SDM_BASE + GPIO_O_PXOUT) ^= SDM_DATA_PIN;
//		P3OUT ^= BIT0;  // Toggle MOSI
#else
//		__no_operation();  // needed for timing....
//		__no_operation();
//		__no_operation();
#endif  /* DM1_RX_DEBUGGING_SIGNAL */

		// Need direct access to this pin....  Driver lib may be to slow
		//TODO THIS API MAY BE TO SLOW:  J/k works fine at 8 MHz
		if(DM1_DATA_BIT){  	// If RX_data == "1", OR in "1".  (Buffer is pre-cleared to all 0x00.)
			pui16RxData[ui8TempByteCount] |= (0x01 << ui8TempBitCount);
		}
		else{
//			__delay_cycles(20);
		}

		if(g_ui8IrqFlag){  	// need to break free before incrementing bit_count
			break;
		}

		ui8TempBitCount++;

		if(ui8TempBitCount == ui8ByteWidth){	// Store Data in buffer, check parity after RX complete
			ui8TempByteCount++;
			ui8TempBitCount = 0;
			pui16RxData[ui8TempByteCount] = 0; // Clear next Buffer byte
		}

#ifdef DM1_RX_DEBUGGING_SIGNAL
		HWREGB(SDM_BASE + GPIO_O_PXOUT) ^= SDM_DATA_PIN;
//		GPIO_toggleOutputOnPin(SPI_PORT, SPI_MOSI);
//		P3OUT ^= BIT0;  // Toggle MOSI
//		SPI_PORT_OUT ^= MOSI_PIN;		// MOSI TOGGLE FOR DEBUGGING
#else
//		__no_operation();
//		__no_operation();
//		__no_operation();
#endif  /* DM1_RX_DEBUGGING_SIGNAL */

		while(SDM_BITCLK){ 				//wait for next falling edge
			if((g_ui8IrqFlag) || (g_ui8TimeOutFlag)){ // have breakouts for IRQ (EOF) and timeout (TA1CCTL0 & CCIFG)
				break;
			}
		}

	}while( (!g_ui8IrqFlag) && (!g_ui8TimeOutFlag) ); // IRQ will go high when receive is complete;  irq_flag is TIMEOUT Flag

	*pui8ByteCount = ui8TempByteCount;
	*pui8BitCount = ui8TempBitCount;

	MISO_SPI_SETUP;  	// Switch MISO back to SPI pin

#ifdef DM1_RX_DEBUGGING_SIGNAL
	//SETUP MOSI PIN FOR DEBUGGING ********************
//	GPIO_setAsPeripheralModuleFunctionOutputPin(SPI_PORT, SPI_MOSI);
	// ************************************************
#endif  /* DM1_RX_DEBUGGING_SIGNAL */
}

//*****************************************************************************
//
//! MIFARE_getPacketStatus - API fetches received data and length
//!
//! \param pui16RXData is a pointer to the RX Data buffer.
//!
//! \param pui8RxLength is a pointer to the received length
//!
//! This function is called from anywhere in the User space.  It simply
//! updates the globals for RX Data and RX Length.
//!
//! \return None.
//
//*****************************************************************************
void MIFARE_getPacketStatus(uint16_t ** pui16RxData, uint8_t * pui8RxByteLength, uint8_t * pui8RxBitLength)
{
	*pui16RxData = g_pui16RxData;
	*pui8RxByteLength = g_ui8RxLengthBytes;
	*pui8RxBitLength = g_ui8RxLengthBits;
}

